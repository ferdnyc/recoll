
CXXFLAGS ?= @CXXFLAGS@
LIBXAPIAN=@LIBXAPIAN@
XAPIANCXXFLAGS=@XAPIANCXXFLAGS@
LIBICONV=@LIBICONV@
INCICONV=@INCICONV@
LIBFAM = @LIBFAM@
RCLLIBVERSION=@RCLLIBVERSION@
X_CFLAGS=@X_CFLAGS@
X_PRE_LIBS=@X_PRE_LIBS@
X_LIBS=@X_LIBS@
X_EXTRA_LIBS=@X_EXTRA_LIBS@
X_LIBX11=@X_LIBX11@
DEFS=@DEFS@

COMMONCPPFLAGS = -I. \
    -I$(top_srcdir)/aspell \
    -I$(top_srcdir)/bincimapmime \
    -I$(top_srcdir)/common \
    -I$(top_srcdir)/index \
    -I$(top_srcdir)/internfile \
    -I$(top_srcdir)/rcldb \
    -I$(top_srcdir)/unac \
    -I$(top_srcdir)/utils \
    -I$(top_srcdir)/xaposix

AM_CPPFLAGS = -Wall -Wno-unused \
    $(COMMONCPPFLAGS) \
    $(INCICONV) \
    $(XAPIANCXXFLAGS) \
    $(X_CFLAGS) \
    -DRECOLL_DATADIR=\"${pkgdatadir}\" \
    -D_GNU_SOURCE \
    $(DEFS)

ACLOCAL_AMFLAGS = -I m4

if NOTHREADS
    LIBTHREADS=
else
    LIBTHREADS= $(LIBSYSTHREADS)
endif

librcldir = $(libdir)/recoll
librcl_LTLIBRARIES = librecoll.la

librecoll_la_SOURCES = \
aspell/aspell-local.h \
aspell/rclaspell.cpp \
aspell/rclaspell.h \
bincimapmime/config.h \
bincimapmime/convert.cc \
bincimapmime/convert.h \
bincimapmime/mime-inputsource.h \
bincimapmime/mime-parsefull.cc \
bincimapmime/mime-parseonlyheader.cc \
bincimapmime/mime-printbody.cc \
bincimapmime/mime-utils.h \
bincimapmime/mime.cc \
bincimapmime/mime.h \
common/beaglequeuecache.cpp \
common/beaglequeuecache.h \
common/conf_post.h \
common/cstr.cpp \
common/cstr.h \
common/rclconfig.cpp \
common/rclconfig.h \
common/rclinit.cpp \
common/rclinit.h \
common/syngroups.cpp \
common/syngroups.h \
common/textsplit.cpp \
common/textsplit.h \
common/unacpp.cpp \
common/unacpp.h \
common/uproplist.h \
index/beaglequeue.cpp \
index/beaglequeue.h \
index/bglfetcher.cpp \
index/bglfetcher.h \
index/checkretryfailed.cpp \
index/checkretryfailed.h \
index/fetcher.cpp \
index/fetcher.h \
index/fsfetcher.cpp \
index/fsfetcher.h \
index/fsindexer.cpp \
index/fsindexer.h \
index/indexer.cpp \
index/indexer.h \
index/mimetype.cpp \
index/mimetype.h \
index/rclmon.h \
index/recollindex.h \
index/subtreelist.cpp \
index/subtreelist.h \
internfile/Filter.h \
internfile/extrameta.cpp \
internfile/extrameta.h \
internfile/htmlparse.cpp \
internfile/htmlparse.h \
internfile/indextext.h \
internfile/internfile.cpp \
internfile/internfile.h \
internfile/mh_exec.cpp \
internfile/mh_exec.h \
internfile/mh_execm.cpp \
internfile/mh_execm.h \
internfile/mh_html.cpp \
internfile/mh_html.h \
internfile/mh_mail.cpp \
internfile/mh_mail.h \
internfile/mh_mbox.cpp \
internfile/mh_mbox.h \
internfile/mh_symlink.h \
internfile/mh_text.cpp \
internfile/mh_text.h \
internfile/mh_unknown.h \
internfile/mimehandler.cpp \
internfile/mimehandler.h \
internfile/myhtmlparse.cpp \
internfile/myhtmlparse.h \
internfile/txtdcode.cpp \
internfile/uncomp.cpp \
internfile/uncomp.h \
query/docseq.cpp \
query/docseq.h \
query/docseqdb.cpp \
query/docseqdb.h \
query/docseqdocs.h \
query/docseqhist.cpp \
query/docseqhist.h \
query/dynconf.cpp \
query/dynconf.h \
query/filtseq.cpp \
query/filtseq.h \
query/plaintorich.cpp \
query/plaintorich.h \
query/recollq.cpp \
query/recollq.h \
query/reslistpager.cpp \
query/reslistpager.h \
query/sortseq.cpp \
query/sortseq.h \
query/wasaparse.ypp \
query/wasaparseaux.cpp \
query/wasaparserdriver.h \
query/wasatorcl.h \
rcldb/daterange.cpp \
rcldb/daterange.h \
rcldb/expansiondbs.cpp \
rcldb/expansiondbs.h \
rcldb/rclabstract.cpp \
rcldb/rcldb.cpp \
rcldb/rcldb.h \
rcldb/rcldb_p.h \
rcldb/rcldoc.cpp \
rcldb/rcldoc.h \
rcldb/rcldups.cpp \
rcldb/rclquery.cpp \
rcldb/rclquery.h \
rcldb/rclquery_p.h \
rcldb/rclterms.cpp \
rcldb/searchdata.cpp \
rcldb/searchdata.h \
rcldb/searchdatatox.cpp \
rcldb/searchdataxml.cpp \
rcldb/stemdb.cpp \
rcldb/stemdb.h \
rcldb/stoplist.cpp \
rcldb/stoplist.h \
rcldb/synfamily.cpp \
rcldb/synfamily.h \
rcldb/termproc.h \
rcldb/xmacros.h \
unac/unac.cpp \
unac/unac.h \
unac/unac_version.h \
utils/appformime.cpp \
utils/appformime.h \
utils/base64.cpp \
utils/base64.h \
utils/cancelcheck.h \
utils/circache.cpp \
utils/circache.h \
utils/closefrom.cpp \
utils/closefrom.h \
utils/conftree.cpp \
utils/conftree.h \
utils/copyfile.cpp \
utils/copyfile.h \
utils/cpuconf.cpp \
utils/cpuconf.h \
utils/debuglog.cpp \
utils/debuglog.h \
utils/ecrontab.cpp \
utils/ecrontab.h \
utils/execmd.cpp \
utils/execmd.h \
utils/fileudi.cpp \
utils/fileudi.h \
utils/fstreewalk.cpp \
utils/fstreewalk.h \
utils/hldata.h \
utils/idfile.cpp \
utils/idfile.h \
utils/md5.cpp \
utils/md5.h \
utils/md5ut.cpp \
utils/md5ut.h \
utils/mimeparse.cpp \
utils/mimeparse.h \
utils/netcon.cpp \
utils/netcon.h \
utils/pathut.cpp \
utils/pathut.h \
utils/ptmutex.h \
utils/pxattr.cpp \
utils/pxattr.h \
utils/rclionice.cpp \
utils/rclionice.h \
utils/readfile.cpp \
utils/readfile.h \
utils/refcntr.h \
utils/smallut.cpp \
utils/smallut.h \
utils/strmatcher.cpp \
utils/strmatcher.h \
utils/transcode.cpp \
utils/transcode.h \
utils/utf8iter.h \
utils/wipedir.cpp \
utils/wipedir.h \
utils/workqueue.h \
utils/x11mon.cpp \
utils/x11mon.h

BUILT_SOURCES = query/wasaparse.cpp
AM_YFLAGS = -d

# We use -release: the lib is only shared
# between recoll programs from the same release.
# -version-info $(VERSION_INFO)
librecoll_la_LDFLAGS = -release $(VERSION) \
    -Wl,--no-undefined -Wl,--warn-unresolved-symbols

librecoll_la_LIBADD = $(LIBXAPIAN) $(LIBICONV) $(X_LIBX11) $(LIBTHREADS)

# There is probably a better way to do this. The KIO needs to be linked
# with librecoll, but librecoll is installed into a non-standard place
# (/usr/lib/recoll). Debian packaging has something against setting an
# rpath on the kio (cause it's not the same package as the lib), so I don't
# know how to link it dynamically. The other thing I don't know is how to
# force automake to build a static lib with the PIC objects. So the
# following target, which is only used from the KIO build, deletes any .a
# and .so and rebuilds the .a with the pic objs (the kio build calls
# configured --disable-static).
# Of course this is very uncomfortably close to automake/libtool internals
# and may not work on all systems.
PicStatic: $(librecoll_la_OBJECTS)
	rm -f .libs/librecoll.a
	rm -f .libs/librecoll.so
	libtool --mode=link gcc -g -O -o librecoll.la \
                $(librecoll_la_OBJECTS)

bin_PROGRAMS = recollindex
if MAKECMDLINE
    bin_PROGRAMS += recollq
endif

if MAKEXADUMP
   bin_PROGRAMS += xadump
endif

recollindex_SOURCES = \
                    index/recollindex.cpp \
                    index/rclmonprc.cpp \
                    index/rclmonrcv.cpp 
recollindex_LDADD = librecoll.la

recollq_SOURCES = query/recollqmain.cpp
recollq_LDADD = librecoll.la

xadump_SOURCES = query/xadump.cpp
xadump_LDADD = librecoll.la $(LIBXAPIAN) $(LIBICONV)

# Note: I'd prefer the generated query parser files not to be distributed
# at all, but failed to achieve this
EXTRA_DIST = \
bincimapmime/00README.recoll bincimapmime/AUTHORS bincimapmime/COPYING \
\
desktop/hotrecoll.py \
desktop/recoll.appdata.xml \
desktop/recollindex.desktop \
desktop/recoll_index_on_ac.sh \
desktop/recoll-searchgui.desktop \
desktop/recoll.png desktop/recoll.svg desktop/recoll.xcf \
\
doc/man \
doc/prog \
doc/user/*.html doc/user/*.css doc/user/*.txt doc/user/*.xml doc/user/Makefile \
\
filters \
\
index/rclmon.sh \
\
kde/kioslave/kio_recoll/00README.txt \
kde/kioslave/kio_recoll/CMakeLists.txt \
kde/kioslave/kio_recoll/data/help.html \
kde/kioslave/kio_recoll/data/searchable.html \
kde/kioslave/kio_recoll/data/welcome.html \
kde/kioslave/kio_recoll/dirif.cpp \
kde/kioslave/kio_recoll/htmlif.cpp \
kde/kioslave/kio_recoll/kio_recoll.cpp \
kde/kioslave/kio_recoll/kio_recoll.h \
kde/kioslave/kio_recoll/recollf.protocol \
kde/kioslave/kio_recoll/recollnolist.protocol \
kde/kioslave/kio_recoll/recoll.protocol \
\
query/location.hh  query/position.hh  query/stack.hh \
\
qtgui/advsearch.ui \
qtgui/advsearch_w.cpp \
qtgui/advsearch_w.h \
qtgui/advshist.cpp \
qtgui/advshist.h \
qtgui/confgui/confgui.cpp \
qtgui/confgui/confgui.h \
qtgui/confgui/confguiindex.cpp \
qtgui/confgui/confguiindex.h \
qtgui/confgui/conflinkrcl.h \
qtgui/confgui/main.cpp \
qtgui/crontool.cpp \
qtgui/crontool.h \
qtgui/crontool.ui \
qtgui/editdialog.h \
qtgui/editdialog.ui \
qtgui/firstidx.h \
qtgui/firstidx.ui \
qtgui/fragbuts.cpp \
qtgui/fragbuts.h \
qtgui/guiutils.cpp \
qtgui/guiutils.h \
qtgui/i18n/recoll_cs.qm qtgui/i18n/recoll_cs.ts \
qtgui/i18n/recoll_da.qm qtgui/i18n/recoll_da.ts \
qtgui/i18n/recoll_de.qm qtgui/i18n/recoll_de.ts \
qtgui/i18n/recoll_el.qm qtgui/i18n/recoll_el.ts \
qtgui/i18n/recoll_es.qm qtgui/i18n/recoll_es.ts \
qtgui/i18n/recoll_fr.qm qtgui/i18n/recoll_fr.ts \
qtgui/i18n/recoll_it.qm qtgui/i18n/recoll_it.ts \
qtgui/i18n/recoll_lt.qm qtgui/i18n/recoll_lt.ts \
qtgui/i18n/recoll_pl.qm qtgui/i18n/recoll_pl.ts \
qtgui/i18n/recoll_ru.qm qtgui/i18n/recoll_ru.ts \
qtgui/i18n/recoll_tr.qm qtgui/i18n/recoll_tr.ts \
qtgui/i18n/recoll_uk.qm qtgui/i18n/recoll_uk.ts \
qtgui/i18n/recoll_xx.qm qtgui/i18n/recoll_xx.ts \
qtgui/i18n/recoll_zh_CN.qm qtgui/i18n/recoll_zh_CN.ts \
qtgui/i18n/recoll_zh.qm qtgui/i18n/recoll_zh.ts \
qtgui/idxsched.h \
qtgui/idxsched.ui \
qtgui/images/asearch.png \
qtgui/images/cancel.png \
qtgui/images/close.png \
qtgui/images/code-block.png \
qtgui/images/down.png \
qtgui/images/firstpage.png \
qtgui/images/history.png \
qtgui/images/nextpage.png \
qtgui/images/prevpage.png \
qtgui/images/recoll.icns \
qtgui/images/recoll.png \
qtgui/images/sortparms.png \
qtgui/images/spell.png \
qtgui/images/table.png \
qtgui/images/up.png \
qtgui/listdialog.h \
qtgui/listdialog.ui \
qtgui/main.cpp \
qtgui/mtpics/aptosid-book.png \
qtgui/mtpics/aptosid-manual-copyright.txt \
qtgui/mtpics/aptosid-manual.png \
qtgui/mtpics/archive.png \
qtgui/mtpics/bookchap.png \
qtgui/mtpics/book.png \
qtgui/mtpics/document.png \
qtgui/mtpics/drawing.png \
qtgui/mtpics/emblem-symbolic-link.png \
qtgui/mtpics/folder.png \
qtgui/mtpics/html.png \
qtgui/mtpics/image.png \
qtgui/mtpics/License_sidux.txt \
qtgui/mtpics/message.png \
qtgui/mtpics/mozilla_doc.png \
qtgui/mtpics/pdf.png \
qtgui/mtpics/pidgin.png \
qtgui/mtpics/postscript.png \
qtgui/mtpics/presentation.png \
qtgui/mtpics/README \
qtgui/mtpics/sidux-book.png \
qtgui/mtpics/soffice.png \
qtgui/mtpics/source.png \
qtgui/mtpics/sownd.png \
qtgui/mtpics/spreadsheet.png \
qtgui/mtpics/text-x-python.png \
qtgui/mtpics/txt.png \
qtgui/mtpics/video.png \
qtgui/mtpics/wordprocessing.png \
qtgui/multisave.cpp \
qtgui/multisave.h \
qtgui/preview_w.cpp \
qtgui/preview_w.h \
qtgui/ptrans.ui \
qtgui/ptrans_w.cpp \
qtgui/ptrans_w.h \
qtgui/rclhelp.cpp \
qtgui/rclhelp.h \
qtgui/rclmain.ui \
qtgui/rclmain_w.cpp \
qtgui/rclmain_w.h \
qtgui/rclm_idx.cpp \
qtgui/rclm_preview.cpp \
qtgui/rclm_saveload.cpp \
qtgui/rclm_view.cpp \
qtgui/rclm_wins.cpp \
qtgui/rclzg.cpp \
qtgui/rclzg.h \
qtgui/recoll.h \
qtgui/recoll.pro.in \
qtgui/recoll.qrc \
qtgui/reslist.cpp \
qtgui/reslist.h \
qtgui/respopup.cpp \
qtgui/respopup.h \
qtgui/restable.cpp \
qtgui/restable.h \
qtgui/restable.ui \
qtgui/rtitool.cpp \
qtgui/rtitool.h \
qtgui/rtitool.ui \
qtgui/searchclause_w.cpp \
qtgui/searchclause_w.h \
qtgui/snippets.ui \
qtgui/snippets_w.cpp \
qtgui/snippets_w.h \
qtgui/spell.ui \
qtgui/spell_w.cpp \
qtgui/spell_w.h \
qtgui/ssearchb.ui \
qtgui/ssearch_w.cpp \
qtgui/ssearch_w.h \
qtgui/systray.cpp \
qtgui/systray.h \
qtgui/uiprefs.ui \
qtgui/uiprefs_w.cpp \
qtgui/uiprefs_w.h \
qtgui/ui_rclmain.h-4.5 \
qtgui/viewaction.ui \
qtgui/viewaction_w.cpp \
qtgui/viewaction_w.h \
qtgui/xmltosd.cpp \
qtgui/xmltosd.h \
\
python/README.txt \
python/recoll/Makefile \
python/recoll/pyrclextract.cpp \
python/recoll/pyrecoll.cpp \
python/recoll/pyrecoll.h \
python/recoll/recoll/__init__.py \
python/recoll/recoll/rclconfig.py \
python/recoll/setup.py.in \
python/samples/docdups.py \
python/samples/mutt-recoll.py \
python/samples/rcldlkp.py \
python/samples/rclmbox.py \
python/samples/recollgui/Makefile \
python/samples/recollgui/qrecoll.py \
python/samples/recollgui/rclmain.ui \
python/samples/recollq.py \
python/samples/recollqsd.py \
\
 \
sampleconf/fields sampleconf/fragbuts.xml sampleconf/mimeconf \
sampleconf/mimemap sampleconf/mimeview sampleconf/mimeview.mac \
sampleconf/recoll.conf.in sampleconf/recoll.qss \
\
unac/AUTHORS unac/COPYING unac/README unac/README.recoll \
\
VERSION

# EXTRA_DIST: The Php Code does not build anymore. No need to ship it until
# someone fixes it:
# php/00README.txt php/recoll/config.m4 php/recoll/make.sh 
# php/recoll/php_recoll.h php/recoll/recoll.cpp php/sample/shell.php 

if MAKEPYTHON
all-local: recollpython
recollpython: librecoll.la
	${MAKE} -C python/recoll libdir=$(libdir)
install-exec-local: recollpython-install
recollpython-install:
	(cd python/recoll; \
        if test -f /etc/debian_version ; then \
            OPTSFORPYTHON=--install-layout=deb; \
        fi; \
        set -x; \
        python setup.py install \
         --prefix=${prefix} --root=$${DESTDIR:-/} $${OPTSFORPYTHON})
clean-local: recollpython-clean
recollpython-clean:
	rm -rf python/recoll/build
endif

if MAKEQT
all-local: recollqt
recollqt: librecoll.la
	(cd $(QTGUI); ${QMAKE} PREFIX=${prefix} recoll.pro)
	$(MAKE) -C $(QTGUI) LFLAGS="$(LDFLAGS)" prefix=$(prefix) \
                exec_prefix=$(exec_prefix) libdir=$(libdir)
clean-local: recollqt-clean
recollqt-clean:
	-$(MAKE) -C $(QTGUI) clean
install-exec-local: recollqt-install
recollqt-install:
	$(MAKE) -C $(QTGUI) LFLAGS="$(LDFLAGS)" prefix=$(prefix) \
                exec_prefix=$(exec_prefix) libdir=$(libdir) install
endif

defconfdir = $(pkgdatadir)/examples
defconf_DATA = \
desktop/recollindex.desktop \
index/rclmon.sh \
sampleconf/fragbuts.xml \
sampleconf/fields \
sampleconf/recoll.conf \
sampleconf/mimeconf \
sampleconf/recoll.qss \
sampleconf/mimemap \
sampleconf/mimeview 

filterdir = $(pkgdatadir)/filters
filter_DATA = \
desktop/hotrecoll.py \
filters/rcl* \
filters/ppt-dump.py \
filters/xls-dump.py \
filters/xlsxmltocsv.py \
filters/msodump.zip \
python/recoll/recoll/rclconfig.py 

install-data-hook: 
	(cd $(DESTDIR)/$(filterdir); \
	chmod a+x rcl* ppt-dump.py xls-dump.py xlsxmltocsv.py hotrecoll.py; \
	chmod 0644 msodump.zip rclexecm.py rcllatinstops.zip rclconfig.py) 

if MAKEUSERDOC
rdocdir = $(pkgdatadir)/doc
rdoc_DATA = doc/user/usermanual.html doc/user/docbook-xsl.css 
doc/user/usermanual.html: doc/user/usermanual.xml
	$(MAKE) -C doc/user
endif

dist_man1_MANS = doc/man/recoll.1 doc/man/recollq.1 doc/man/recollindex.1
dist_man5_MANS = doc/man/recoll.conf.5

dist-hook:
	modified=`hg status | grep -v /$(distdir)/`; \
	if test ! -z "$$modified"; then \
            echo Local directory is modified: $$modified ; exit 1; fi
	hg tag -f -m "Release $(VERSION) tagged" RECOLL-$(VERSION)