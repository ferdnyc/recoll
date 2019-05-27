#!/usr/bin/python3
#################################
# Copyright (C) 2019 J.F.Dockes
#   This program is free software; you can redistribute it and/or modify
#   it under the terms of the GNU General Public License as published by
#   the Free Software Foundation; either version 2 of the License, or
#   (at your option) any later version.
#
#   This program is distributed in the hope that it will be useful,
#   but WITHOUT ANY WARRANTY; without even the implied warranty of
#   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
#   GNU General Public License for more details.
#
#   You should have received a copy of the GNU General Public License
#   along with this program; if not, write to the
#   Free Software Foundation, Inc.,
#   51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA.
########################################################

#
# Process the stream produced by a modified pffexport:
# https://github.com/libyal/libpff
# The modification allows producing a data stream instead of a file tree
#

import sys
import os
import posixpath
import pathlib
import tempfile
import shutil
import getopt
import traceback
import email.parser
import email.policy
import mailbox
import subprocess

import rclexecm
import rclconfig
import conftree


# The pffexport stream yields the email in several pieces, with some
# data missing (e.g. attachment MIME types). We rebuild a complete
# message for parsing by the Recoll email handler
class EmailBuilder(object):
    def __init__(self, logger, mimemap):
        self.log = logger
        self.reset()
        self.mimemap = mimemap
        self.parser = email.parser.Parser(policy = email.policy.default)

    def reset(self):
        self.headers = ''
        self.body = ''
        self.bodymimemain = ''
        self.bodymimesub = ''
        self.attachments = []

    def setheaders(self, h):
        self.headers = h

    def setbody(self, body, main, sub):
        self.body = body
        self.bodymimemain = main
        self.bodymimesub = sub

    def addattachment(self, att, filename):
        #self.log("Adding attachment")
        self.attachments.append((att, filename))

    def flush(self):
        if not (self.headers and (self.body or self.attachments)):
            self.log("Not flushing because no headers or no body/attach")
            self.reset()
            return None

        newmsg = email.message.EmailMessage(policy=email.policy.default)
        headerstr = self.headers.decode('utf-8')
        # print("%s" % headerstr)
        headers = self.parser.parsestr(headerstr, headersonly=True)
        #self.log("EmailBuilder: content-type %s" % headers['content-type'])
        for nm in ('from', 'subject'):
            if nm in headers:
                newmsg.add_header(nm, headers[nm])

        tolist = headers.get_all('to')
        alldests = ""
        for toheader in tolist:
            for dest in toheader.addresses:
                sd = str(dest).replace('\n', '').replace('\r','')
                #self.log("EmailBuilder: dest %s" % sd)
                alldests += sd + ", "
            alldests = alldests.rstrip(", ")
            newmsg.add_header('to', alldests)

        # Also: CC
            
        if self.body:
            newmsg.set_content(self.body, maintype = self.bodymimemain,
                               subtype = self.bodymimesub)
                
        for att in self.attachments:
            fn = att[1]
            ext = os.path.splitext(fn)[1]
            mime = self.mimemap.get(ext)
            if not mime:
                mime = 'application/octet-stream'
            #self.log("Attachment: filename %s MIME %s" % (fn, mime))
            mt,st = mime.split('/')
            newmsg.add_attachment(att[0], maintype=mt, subtype=st,
                                  filename=fn)

        #newmsg.set_unixfrom("From some@place.org Sun Jan 01 00:00:00 2000")
        #print("%s\n" % newmsg.as_string(unixfrom=True, maxheaderlen=80))

        ret = newmsg.as_string(maxheaderlen=100)
        self.reset()
        return ret
    

class PFFReader(object):
    def __init__(self, logger, infile=sys.stdin):
        self.log = logger
        config = rclconfig.RclConfig()
        dir1 = os.path.join(config.getConfDir(), "examples")
        dir2 = os.path.join(config.datadir, "examples")
        self.mimemap = conftree.ConfStack('mimemap', [dir1, dir2])
        self.infile = infile
        self.fields = {}
        self.msg = EmailBuilder(self.log, self.mimemap)
        
    # Read single parameter from process input: line with param name and size
    # followed by data. The param name is returned as str/unicode, the data
    # as bytes
    def readparam(self):
        inf = self.infile
        s = inf.readline()
        if s == b'':
            return ('', b'')
        s = s.rstrip(b'\n')
        if s == b'':
            return ('', b'')
        l = s.split()
        if len(l) != 2:
            self.log(b'bad line: [' + s + b']', 1, 1)
            return ('', b'')
        paramname = l[0].decode('ASCII').rstrip(':')
        paramsize = int(l[1])
        if paramsize > 0:
            paramdata = inf.read(paramsize)
            if len(paramdata) != paramsize:
                self.log("Bad read: wanted %d, got %d" %
                      (paramsize, len(paramdata)), 1, 1)
                return('', b'')
        else:
            paramdata = b''
        return (paramname, paramdata)

    def mainloop(self):
        basename = ''
        fullpath = ''
        ipath = ''
        while 1:
            name, data = self.readparam()
            if name == "":
                break
            try:
                paramstr = data.decode('utf-8')
            except:
                paramstr = ''

            if name == 'filename':
                #self.log("filename: %s" %  paramstr)
                fullpath = paramstr
                basename = os.path.basename(fullpath)
                parentdir = os.path.basename(os.path.dirname(fullpath))
            elif name == 'data':
                if parentdir == 'Attachments':
                    #self.log("Attachment: %s" % basename)
                    self.msg.addattachment(data, basename)
                else:
                    if basename == 'OutlookHeaders.txt':
                        doc = self.msg.flush()
                        if doc:
                            yield((doc, ipath))
                    elif basename == 'InternetHeaders.txt':
                        #self.log("name: [%s] data: %s" % (name, paramstr))
                        # This part is the indispensable one. Record
                        # the ipath at this point: 
                        p = pathlib.Path(fullpath)
                        # Strip the top dir (/nonexistent.export/)
                        p = p.relative_to(*p.parts[:2])
                        # We use the parent directory as ipath: all
                        # the message parts are in there
                        ipath = str(p.parents[0])
                        self.msg.setheaders(data)
                    elif os.path.splitext(basename)[0] == 'Message':
                        ext = os.path.splitext(basename)[1]
                        if ext == '.txt':
                            self.msg.setbody(data, 'text', 'plain')
                        elif ext == '.html':
                            self.msg.setbody(data, 'text', 'html')
                        elif ext == '.rtf':
                            self.msg.setbody(data, 'text', 'rtf')
                        else:
                            raise Exception("PST: Unknown body type %s"%ext)
                    elif basename == 'ConversationIndex.txt':
                        pass
                    elif basename == 'Recipients.txt':
                        pass
            else:
                raise Exception("Unknown param name: %s" % name)

        self.log("Out of loop")
        doc = self.msg.flush()
        if doc:
            yield((doc, ipath))
        return


class PstExtractor(object):
    def __init__(self, em):
        self.generator = None
        self.em = em
        self.target = "/nonexistent"
        self.cmd = ["pffexport", "-q", "-t", self.target, "-s"]

    def startCmd(self, filename, ipath=None):
        fullcmd = self.cmd + [rclexecm.subprocfile(filename)]
        if ipath:
            fullcmd += ["-p", ipath]
        try:
            self.proc = subprocess.Popen(fullcmd, stdout=subprocess.PIPE)
        except subprocess.CalledProcessError as err:
            self.em.rclog("Pst: Popen(%s) error: %s" % (fullcmd, err))
            return False
        except OSError as err:
            self.em.rclog("Pst: Popen(%s) OS error: %s" % (fullcmd, err))
            return (False, "")
        self.filein = self.proc.stdout
        return True

    ###### File type handler api, used by rclexecm ---------->
    def openfile(self, params):
        self.filename = params["filename:"]
        self.em.rclog("openfile: %s" % self.filename)
        return True

    def getipath(self, params):
        ipath = posixpath.join(self.target + ".export",
                               params["ipath:"].decode("UTF-8"))
        self.em.rclog("getipath: [%s]" % ipath)
        if not self.startCmd(self.filename, ipath=ipath):
            return (False, "", "", rclexecm.RclExecM.eofnow) 
        reader = PFFReader(self.em.rclog, infile=self.filein)
        self.generator = reader.mainloop()
        try:
            doc, ipath = next(self.generator)
            self.em.setmimetype("message/rfc822")
            self.em.rclog("getipath doc len %d [%s] ipath %s" %
                          (len(doc), doc[:20], ipath))
            f = open("/tmp/document", "wb")
            f.write(doc.encode('utf-8'))
        except StopIteration:
            self.em.rclog("getipath: StopIteration")
            return(False, "", "", rclexecm.RclExecM.eofnow)
        return (True, doc, ipath, False)
        
    def getnext(self, params):
        self.em.rclog("getnext:")
        if not self.generator:
            if not self.startCmd(self.filename):
                return False
            reader = PFFReader(self.em.rclog, infile=self.filein)
            self.generator = reader.mainloop()
        try:
            doc, ipath = next(self.generator)
            self.em.setmimetype("message/rfc822")
            self.em.rclog("getnext: ipath %s\ndoc\n%s" % (ipath, doc))
        except StopIteration:
            return(False, "", "", rclexecm.RclExecM.eofnow)
        return (True, doc, ipath, False)
    

if True:
    # Main program: create protocol handler and extractor and run them
    proto = rclexecm.RclExecM()
    extract = PstExtractor(proto)
    rclexecm.main(proto, extract)
else:
    def _deb(s):
        print("%s" % s, file=sys.stderr)
    reader = PFFReader(_deb, infile=sys.stdin.buffer)
    generator = reader.mainloop()
    for doc, ipath in generator:
        _deb("Got %s data len %d" % (ipath, len(doc)))