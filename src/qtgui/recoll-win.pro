# Note this is generated by configure on Linux (see recoll.pro.in).
# recoll-win.pro is under version control anyway and used on Windows

TEMPLATE        = app
LANGUAGE        = C++
TARGET          = recoll

DEFINES += BUILDING_RECOLL
DEFINES += BUILDING_RECOLLGUI

# QT += webkit webkitwidgets
# DEFINES += USING_WEBKIT

QT += widgets webenginewidgets
DEFINES += USING_WEBENGINE

# QT += dbus
# QMAKE_CXXFLAGS += -DUSE_ZEITGEIST

QT += xml widgets printsupport

CONFIG  += qt warn_on thread release
# CONFIG += force_debug_info


HEADERS += \
        actsearch_w.h \
        advsearch_w.h \
        advshist.h \
        confgui/confgui.h \
        confgui/confguiindex.h \
        firstidx.h \
        fragbuts.h \
        idxmodel.h \
        idxsched.h \
        preview_load.h \
        preview_plaintorich.h \
        preview_w.h \
        ptrans_w.h \
        rclhelp.h \
        rclmain_w.h \
        reslist.h \
        restable.h \
        scbase.h \
        searchclause_w.h \
        snippets_w.h \
        specialindex.h \
        spell_w.h \
        ssearch_w.h \
        systray.h \
        uiprefs_w.h \
        viewaction_w.h \
        webcache.h \
        widgets/editdialog.h \
        widgets/listdialog.h \
        widgets/qxtconfirmationmessage.h

SOURCES += \
        actsearch_w.cpp \
        advsearch_w.cpp \
        advshist.cpp \
        confgui/confgui.cpp \
        confgui/confguiindex.cpp \
        fragbuts.cpp \
        guiutils.cpp \
        idxmodel.cpp \
        main.cpp \
        multisave.cpp \
        preview_load.cpp \
        preview_plaintorich.cpp \
        preview_w.cpp \
        ptrans_w.cpp \
        rclhelp.cpp \
        rclm_idx.cpp \
        rclm_menus.cpp \
        rclm_preview.cpp \
        rclm_saveload.cpp \
        rclm_sidefilters.cpp \
        rclm_view.cpp \
        rclm_wins.cpp \
        rclmain_w.cpp \
        rclzg.cpp \
        reslist.cpp \
        respopup.cpp \
        restable.cpp \
        scbase.cpp \
        searchclause_w.cpp \
        snippets_w.cpp \
        spell_w.cpp \
        ssearch_w.cpp \
        systray.cpp \
        uiprefs_w.cpp \
        viewaction_w.cpp \
        webcache.cpp \
        widgets/qxtconfirmationmessage.cpp \
        xmltosd.cpp

FORMS   = \
        actsearch.ui \
        advsearch.ui \
        firstidx.ui \
        idxsched.ui \
        preview.ui \
        ptrans.ui \
        rclmain.ui \
        restable.ui \
        snippets.ui \
        specialindex.ui \
        spell.ui \
        ssearchb.ui \
        uiprefs.ui \
        viewaction.ui \
        webcache.ui \
        widgets/editdialog.ui \
        widgets/listdialog.ui
        
RESOURCES = recoll.qrc

INCLUDEPATH += ../common ../index ../internfile ../query ../unac \
              ../utils ../aspell ../rcldb ../qtgui ../xaposix \
              confgui widgets
windows {
  DEFINES += PSAPI_VERSION=1
  DEFINES += __WIN32__
  DEFINES += UNICODE
  RC_FILE = recoll.rc

  HEADERS += \
    winschedtool.h
  SOURCES += \
    winschedtool.cpp
  FORMS += \
    winschedtool.ui
    
  contains(QMAKE_CC, gcc){
    # MingW
    QMAKE_CXXFLAGS += -std=c++11 -Wno-unused-parameter
    LIBS += \
    C:/recoll/src/windows/build-librecoll-Desktop_Qt_5_8_0_MinGW_32bit-Release/release/librecoll.dll
  }

  contains(QMAKE_CC, cl){
    # MSVC
    RECOLLDEPS = ../../../recolldeps/msvc
    DEFINES += USING_STATIC_LIBICONV
    PRE_TARGETDEPS = \
      ../windows/build-librecoll-Desktop_Qt_5_14_2_MSVC2017_32bit-Release/release/librecoll.lib
    LIBS += \
      -L../windows/build-librecoll-Desktop_Qt_5_14_2_MSVC2017_32bit-Release/release -llibrecoll \
      $$RECOLLDEPS/libxml2/libxml2-2.9.4+dfsg1/win32/bin.msvc/libxml2.lib \
      $$RECOLLDEPS/libxslt/libxslt-1.1.29/win32/bin.msvc/libxslt.lib \
      -L../windows/build-libxapian-Desktop_Qt_5_14_2_MSVC2017_32bit-Release/release -llibxapian \
      -L$$RECOLLDEPS/build-libiconv-Desktop_Qt_5_14_2_MSVC2017_32bit-Release/release -llibiconv \
      $$RECOLLDEPS/zlib-1.2.11/zdll.lib \
      -lrpcrt4 -lws2_32 -luser32 -lshell32 -lshlwapi -lpsapi -lkernel32
  }
}

mac {
  QMAKE_APPLE_DEVICE_ARCHS = x86_64 arm64
  QMAKE_CXXFLAGS += -std=c++11 -pthread -Wno-unused-parameter

  HEADERS += \
    crontool.h \
    rtitool.h
    
  SOURCES += \
    ../utils/closefrom.cpp \
    ../utils/execmd.cpp \
    ../utils/netcon.cpp \
    ../utils/rclionice.cpp \
    crontool.cpp \
    rtitool.cpp

  FORMS += \
    crontool.ui \
    rtitool.ui

  LIBS += \
    ../windows/build-librecoll-Qt_6_2_4_for_macOS-Release/liblibrecoll.a \
    ../windows/build-libxapian-Qt_6_2_4_for_macOS-Release/liblibxapian.a \
    -lxslt -lxml2 -liconv -lz

  ICON = images/recoll.icns

  APP_EXAMPLES.files = \
    ../sampleconf/fragment-buttons.xml \
    ../sampleconf/fields \
    ../sampleconf/recoll.conf \
    ../sampleconf/mimeconf \
    ../sampleconf/mimeview \
    ../sampleconf/mimemap \
    ../sampleconf/recoll.qss \
    ../sampleconf/recoll-dark.qss \
    ../sampleconf/recoll-dark.css 
  APP_EXAMPLES.path = Contents/Resources/examples

  APP_EXAMPLES_MAC.files = \
    ../sampleconf/macos/mimeview 
  APP_EXAMPLES_MAC.path = Contents/Resources/examples/macos

  APP_FILTERS.files = \
  ../filters/abiword.xsl \
  ../filters/cmdtalk.py \
  ../filters/fb2.xsl \
  ../filters/gnumeric.xsl \
  ../filters/kosplitter.py \
  ../filters/msodump.zip \
  ../filters/okular-note.xsl \
  ../filters/opendoc-body.xsl \
  ../filters/opendoc-flat.xsl \
  ../filters/opendoc-meta.xsl \
  ../filters/openxml-xls-body.xsl \
  ../filters/openxml-word-body.xsl \
  ../filters/openxml-meta.xsl \
  ../filters/ppt-dump.py \
  ../filters/rcl7z.py \
  ../filters/rclaptosidman \
  ../filters/rclaudio.py \
  ../filters/rclbasehandler.py \
  ../filters/rclbibtex.sh \
  ../filters/rclcheckneedretry.sh \
  ../filters/rclchm.py \
  ../filters/rcldia.py \
  ../filters/rcldjvu.py \
  ../filters/rcldoc.py \
  ../filters/rcldvi \
  ../filters/rclepub.py \
  ../filters/rclepub1.py \
  ../filters/rclexec1.py \
  ../filters/rclexecm.py \
  ../filters/rclfb2.py \
  ../filters/rclgaim \
  ../filters/rclgenxslt.py \
  ../filters/rclhwp.py \
  ../filters/rclics.py \
  ../filters/rclimg \
  ../filters/rclimg.py \
  ../filters/rclinfo.py \
  ../filters/rclkar.py \
  ../filters/rclkwd \
  ../filters/rcllatinclass.py \
  ../filters/rcllatinstops.zip \
  ../filters/rcllyx \
  ../filters/rclman \
  ../filters/rclmidi.py \
  ../filters/rclocrcache.py \
  ../filters/rclocr.py \
  ../filters/rclocrabbyy.py \
  ../filters/rclocrtesseract.py \
  ../filters/rclopxml.py \
  ../filters/rclpdf.py \
  ../filters/rclppt.py \
  ../filters/rclps \
  ../filters/rclpst.py \
  ../filters/rclpurple \
  ../filters/rclpython.py \
  ../filters/rclrar.py \
  ../filters/rclrtf.py \
  ../filters/rclscribus \
  ../filters/rclshowinfo \
  ../filters/rcltar.py \
  ../filters/rcltex \
  ../filters/rcltext.py \
  ../filters/rcluncomp \
  ../filters/rcluncomp.py \
  ../filters/rclwar.py \
  ../filters/rclxls.py \
  ../filters/rclxml.py \
  ../filters/rclxmp.py \
  ../filters/rclxslt.py \
  ../filters/rclzip.py \
  ../filters/recoll-we-move-files.py \
  ../filters/recollepub.zip \
  ../filters/svg.xsl \
  ../filters/xls-dump.py \
  ../filters/xlsxmltocsv.py \
  ../filters/xml.xsl \
  ../python/recoll/recoll/conftree.py \
  ../python/recoll/recoll/rclconfig.py
  APP_FILTERS.path = Contents/Resources/filters

  APP_IMAGES.files = \
  images/asearch.png \
  images/cancel.png \
  images/close.png \
  images/clock.png \
  images/menu.png \
  images/code-block.png \
  images/down.png \
  images/firstpage.png \
  images/history.png \
  images/interro.png \
  images/nextpage.png \
  images/prevpage.png \
  images/recoll.icns \
  images/recoll.png \
  images/sortparms.png \
  images/spell.png \
  images/table.png \
  images/up.png \
  mtpics/License_sidux.txt \
  mtpics/README \
  mtpics/aptosid-book.png \
  mtpics/aptosid-manual-copyright.txt \
  mtpics/aptosid-manual.png \
  mtpics/archive.png \
  mtpics/book.png \
  mtpics/bookchap.png \
  mtpics/document.png \
  mtpics/drawing.png \
  mtpics/emblem-symbolic-link.png \
  mtpics/folder.png \
  mtpics/html.png \
  mtpics/image.png \
  mtpics/message.png \
  mtpics/mozilla_doc.png \
  mtpics/pdf.png \
  mtpics/pidgin.png \
  mtpics/postscript.png \
  mtpics/presentation.png \
  mtpics/sidux-book.png \
  mtpics/soffice.png \
  mtpics/source.png \
  mtpics/sownd.png \
  mtpics/spreadsheet.png \
  mtpics/text-x-python.png \
  mtpics/txt.png \
  mtpics/video.png \
  mtpics/wordprocessing.png
  APP_IMAGES.path = Contents/Resources/images

  QMAKE_BUNDLE_DATA = APP_EXAMPLES APP_FILTERS APP_IMAGES
}

TRANSLATIONS = \
i18n/recoll_cs.ts \
i18n/recoll_da.ts \
i18n/recoll_de.ts \
i18n/recoll_el.ts \
i18n/recoll_es.ts \
i18n/recoll_fr.ts \
i18n/recoll_hu.ts \
i18n/recoll_it.ts \
i18n/recoll_ja.ts \
i18n/recoll_ko.ts \
i18n/recoll_lt.ts \
i18n/recoll_nl.ts \
i18n/recoll_pl.ts \
i18n/recoll_ru.ts \
i18n/recoll_sv.ts \
i18n/recoll_tr.ts \
i18n/recoll_uk.ts \
i18n/recoll_xx.ts \
i18n/recoll_zh_CN.ts \
i18n/recoll_zh.ts
