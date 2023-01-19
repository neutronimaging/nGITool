#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T20:37:06
#
#-------------------------------------------------

QT       += core widgets charts concurrent # printsupport

TARGET = nGITool
TEMPLATE = app
CONFIG += c++11

REPOS = $$PWD/../../../..
CONFIG(release, debug|release): DESTDIR = $$REPOS/Applications
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/Applications/debug

SOURCES += main.cpp\
        ngimainwindow.cpp #\
#    ngireport.cpp

HEADERS  += ngimainwindow.h #\
#    ngireport.h

FORMS    += ngimainwindow.ui

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
    QMAKE_LFLAGS += /MACHINE:X64
    }

    QMAKE_LIBDIR += $$REPOS/ExternalDependencies/windows/lib
    INCLUDEPATH  += $$REPOS/ExternalDependencies/windows/include/cfitsio
    INCLUDEPATH  += $$REPOS/ExternalDependencies/windows/include/libxml2

    INCLUDEPATH  += $$REPOS/imagingsuite/external/include
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2 -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2

    DEFINES += NOMINMAX
}

unix {
    QMAKE_CXXFLAGS += -fPIC -O2

    INCLUDEPATH += /usr/include/libxml2
    unix:macx {
        QMAKE_CXXFLAGS += -fPIC -O2
        INCLUDEPATH += /opt/local/include

        INCLUDEPATH += /opt/local/include/libxml2
        QMAKE_LIBDIR += /opt/local/lib $$REPOS/lib
        ICON = $$PWD/ngi_icon.icns
    }
    else {
        QMAKE_CXXFLAGS += -fPIC -fopenmp -O2
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
        INCLUDEPATH += /usr/include/libxml2
    }
    LIBS +=  -lxml2 -ltiff
}

ICON = ../Resources/ngi_icon.icns
RC_ICONS = ../Resources/ngi_icon.ico

CONFIG(release, debug|release): LIBS += -L$$REPOS/lib/
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug

LIBS += -lkipl -lQtAddons -lModuleConfig -lQtModuleConfigure -lnGIFramework

INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtAddons/
DEPENDPATH += $$REPOS/imagingsuite/GUI/qt/QtAddons/

INCLUDEPATH += $$PWD/../../../frameworks/ngi/nGIFramework/include/
DEPENDPATH += $$PWD/../../../frameworks/ngi/nGIFramework/src

INCLUDEPATH += $$REPOS/imagingsuite/GUI/qt/QtModuleConfigure
DEPENDPATH += $$REPOS/imagingsuite/GUI/qt/QtModuleConfigure

INCLUDEPATH += $$PWD/../../../../imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH += $$PWD/../../../../imagingsuite/core/modules/ModuleConfig/src

INCLUDEPATH += $$PWD/../../../../imagingsuite/core/kipl/kipl/include/
DEPENDPATH += $$PWD/../../../../imagingsuite/core/kipl/kipl/include/

OTHER_FILES += \
    FeatureWishList.txt
