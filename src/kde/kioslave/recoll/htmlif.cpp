#ifndef lint
static char rcsid[] = "@(#$Id: htmlif.cpp,v 1.11 2008-12-16 17:28:10 dockes Exp $ (C) 2005 J.F.Dockes";
#endif
/*
 *   This program is free software; you can redistribute it and/or modify
 *   it under the terms of the GNU General Public License as published by
 *   the Free Software Foundation; either version 2 of the License, or
 *   (at your option) any later version.
 *
 *   This program is distributed in the hope that it will be useful,
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *   GNU General Public License for more details.
 *
 *   You should have received a copy of the GNU General Public License
 *   along with this program; if not, write to the
 *   Free Software Foundation, Inc.,
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h> 
#include <sys/stat.h>

#include <string>

using namespace std;

#include <kdebug.h>
#include <kstandarddirs.h>

#include "rclconfig.h"
#include "rcldb.h"
#include "rclinit.h"
#include "pathut.h"
#include "searchdata.h"
#include "rclquery.h"
#include "wasastringtoquery.h"
#include "wasatorcl.h"
#include "kio_recoll.h"
#include "docseqdb.h"
#include "readfile.h"
#include "smallut.h"
#include "plaintorich.h"
#include "internfile.h"
#include "wipedir.h"

using namespace KIO;

bool RecollKioPager::append(const string& data)
{
    if (!m_parent) 
	return false;
    m_parent->data(QByteArray(data.c_str()));
    return true;
}
#include <sstream>
string RecollProtocol::makeQueryUrl(int page, bool isdet)
{
    ostringstream str;
    str << "recoll://search/query?q=" << 
	url_encode((const char*)m_query.query.toUtf8()) <<
	"&qtp=" << (const char*)m_query.opt.toUtf8();
    if (page >= 0)
	str << "&p=" << page;
    if (isdet)
	str << "&det=1";
    return str.str();
}

string RecollKioPager::detailsLink()
{
    string chunk = string("<a href=\"") + 
	m_parent->makeQueryUrl(m_parent->m_pager.pageNumber(), true) + "\">"
	+ "(show query)" + "</a>";
    return chunk;
}

static string parformat;
const string& RecollKioPager::parFormat()
{
    // Need to escape the % inside the query url
    string qurl = m_parent->makeQueryUrl(-1, false), escurl;
    for (string::size_type pos = 0; pos < qurl.length(); pos++) {
	switch(qurl.at(pos)) {
	case '%':
	    escurl += "%%";
	    break;
	default:
	    escurl += qurl.at(pos);
	}
    }

    ostringstream str;
    str << 
	"<a href=\"%U\"><img src=\"%I\" align=\"left\"></a>" 
	"%R %S "
	"<a href=\"" << escurl << "&cmd=pv&dn=%N\">Preview</a>&nbsp;&nbsp;" <<
	"<a href=\"%U\">Open</a><br>" <<
	"<b>%T</b><br>"
	"%M&nbsp;%D&nbsp;&nbsp; <i>%U</i>&nbsp;&nbsp;%i<br>"
	"%A %K";
    return parformat = str.str();
}

string RecollKioPager::pageTop()
{
    string pt = "<p align=\"center\"> <a href=\"recoll:///search.html?q=";
    pt += url_encode(string(m_parent->m_query.query.toUtf8()));
    pt += "\">New Search</a>";
    return pt;
// Would be nice to have but doesnt work because the query may be executed
// by another kio instance which has no idea of the current page o
#if 0 && KDE_IS_VERSION(4,1,0)
	" &nbsp;&nbsp;&nbsp;<a href=\"recoll:///" + 
	url_encode(string(m_parent->m_query.query.toUtf8())) +
	"/\">Directory view</a> (you may need to reload the page)"
#endif
}

string RecollKioPager::nextUrl()
{
    int pagenum = pageNumber();
    if (pagenum < 0)
	pagenum = 0;
    else
	pagenum++;
    return m_parent->makeQueryUrl(pagenum);
}

string RecollKioPager::prevUrl()
{
    int pagenum = pageNumber();
    if (pagenum <= 0)
	pagenum = 0;
    else
	pagenum--;
    return m_parent->makeQueryUrl(pagenum);
}

static string welcomedata;

void RecollProtocol::searchPage()
{
    kDebug();
    mimeType("text/html");
    if (welcomedata.empty()) {
	QString location = 
	    KStandardDirs::locate("data", "kio_recoll/welcome.html");
	string reason;
	if (location.isEmpty() || 
	    !file_to_string((const char *)location.toUtf8(), 
			    welcomedata, &reason)) {
	    welcomedata = "<html><head><title>Recoll Error</title></head>"
		"<body><p>Could not locate Recoll welcome.html file: ";
	    welcomedata += reason;
	    welcomedata += "</p></body></html>";
	}
    }    

    string catgq;
#if 0
    // Catg filtering. A bit complicated to do because of the
    // stateless thing (one more thing to compare to check if same
    // query) right now. Would be easy by adding to the query
    // language, but not too useful in this case, so scrap it for now.
    list<string> cats;
    if (o_rclconfig->getMimeCategories(cats) && !cats.empty()) {
	catgq = "<p>Filter on types: "
	    "<input type=\"radio\" name=\"ct\" value=\"All\" checked>All";
	for (list<string>::iterator it = cats.begin(); it != cats.end();it++) {
	    catgq += "\n<input type=\"radio\" name=\"ct\" value=\"" +
		*it + "\">" + *it ;
	}
    }
#endif 

    string tmp;
    map<char, string> subs;
    subs['Q'] = (const char *)m_query.query.toUtf8();
    subs['C'] = catgq;
    subs['S'] = "";
    pcSubst(welcomedata, tmp, subs);
    data(tmp.c_str());
}

void RecollProtocol::queryDetails()
{
    kDebug();
    mimeType("text/html");
    QByteArray array;
    QTextStream os(&array, QIODevice::WriteOnly);

    os << "<html><head>" << endl;
    os << "<meta http-equiv=\"Content-Type\" content=\"text/html;"
	"charset=utf-8\">" << endl;
    os << "<title>" << "Recoll query details" << "</title>\n" << endl;
    os << "</head>" << endl;
    os << "<body><h3>Query details:</h3>" << endl;
    os << "<p>" << m_pager.queryDescription().c_str() <<"</p>"<< endl;
    os << "<p><a href=\"" << makeQueryUrl(m_pager.pageNumber()).c_str() << 
	"\">Return to results</a>" << endl;
    os << "</body></html>" << endl;
    data(array);
}


class PlainToRichKio : public PlainToRich {
public:
    PlainToRichKio(const string& nm) 
	: PlainToRich() , m_name(nm)
    {
    }    
    virtual ~PlainToRichKio() {}
    virtual string header() {
	if (m_inputhtml) {
	    return snull;
	} else {
	    return
		string("<html><head>"
		       "<META http-equiv=\"Content-Type\""
		       "content=\"text/html;charset=UTF-8\"><title>") + 
		m_name + 
		string("</title></head><body><p>");
	}
    }
    virtual string startMatch() {return string("<font color=\"blue\">");}
    virtual string endMatch() {return string("</font>");}
    const string &m_name;
};

void RecollProtocol::showPreview(const Rcl::Doc& doc)
{
    string fn = doc.url.substr(7);
    kDebug() << fn.c_str();

    struct stat st;
    if (::stat(fn.c_str(), &st) != 0) {
	string reason = string("File not found: ") + fn.c_str();
	error(KIO::ERR_SLAVE_DEFINED, reason.c_str());
	return;
    } 
    string tmpdir;
    string reason;
    if (!maketmpdir(tmpdir, reason)) {
	error(KIO::ERR_SLAVE_DEFINED, "Cannot create temporary directory");
	return;
    }
    o_rclconfig->setKeyDir(path_getfather(fn));
    FileInterner interner(fn, &st, o_rclconfig, tmpdir, &doc.mimetype);
    Rcl::Doc fdoc;
    string ipath = doc.ipath;
    if (!interner.internfile(fdoc, ipath)) {
	wipedir(tmpdir);
	rmdir(tmpdir.c_str());
	error(KIO::ERR_SLAVE_DEFINED, "Cannot convert file to internal format");
	return;
    }
    wipedir(tmpdir);
    rmdir(tmpdir.c_str());

    if (!interner.get_html().empty()) {
	fdoc.text = interner.get_html();
	fdoc.mimetype = "text/html";
    }

    mimeType("text/html");

    string fname =  path_getsimple(doc.url).c_str();
    PlainToRichKio ptr(fname);
    ptr.set_inputhtml(!fdoc.mimetype.compare("text/html"));
    list<string> otextlist;
    HiliteData hdata;
    if (!m_source.isNull())
	m_source->getTerms(hdata.terms, hdata.groups, hdata.gslks);
    ptr.plaintorich(fdoc.text, otextlist, hdata);

    QByteArray array;
    QTextStream os(&array, QIODevice::WriteOnly);
    for (list<string>::iterator it = otextlist.begin(); 
	 it != otextlist.end(); it++) {
	os << (*it).c_str();
    }
    os << "</body></html>" << endl;
    data(array);
}

void RecollProtocol::htmlDoSearch(const QueryDesc& qd)
{
    kDebug() << "q" << qd.query << "opt" << qd.opt << "page" << qd.page <<
	"isdet" << qd.isDetReq;

    mimeType("text/html");

    if (!syncSearch(qd))
	return;
    // syncSearch/doSearch do the setDocSource when needed
    if (m_pager.pageNumber() < 0) {
	m_pager.resultPageNext();
    }
    if (qd.isDetReq) {
	queryDetails();
	return;
    }

    // Check / adjust page number
    if (qd.page > m_pager.pageNumber()) {
	int npages = qd.page - m_pager.pageNumber();
	for (int i = 0; i < npages; i++)
	    m_pager.resultPageNext();
    } else if (qd.page < m_pager.pageNumber()) {
	int npages = m_pager.pageNumber() - qd.page;
	for (int i = 0; i < npages; i++) 
	    m_pager.resultPageBack();
    }
    // Display
    m_pager.displayPage();
}
