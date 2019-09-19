#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T20:37:06
#
#-------------------------------------------------

QT       += core widgets printsupport charts concurrent

TARGET = QtnGITool
TEMPLATE = app
CONFIG += c++11

CONFIG(release, debug|release): DESTDIR = $$PWD/../../../../Applications
else:CONFIG(debug, debug|release): DESTDIR = $$PWD/../../../Applications/debug

SOURCES += main.cpp\
        ngimainwindow.cpp \
    ngireport.cpp

HEADERS  += ngimainwindow.h \
    ngireport.h

FORMS    += ngimainwindow.ui

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
    QMAKE_LFLAGS += /MACHINE:X64
    }
    INCLUDEPATH += ../../../../../external/src/linalg ../../../../../external/include ../../../../../external/include/cfitsio
    QMAKE_LIBDIR += ../../../../../external/lib64

    LIBS += -llibxml2_dll -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2

    DEFINES += NOMINMAX
}

unix {
    INCLUDEPATH += "../../../../external/src/linalg"
    QMAKE_CXXFLAGS += -fPIC -O2

    INCLUDEPATH += /usr/include/libxml2
    unix:macx {
        QMAKE_CXXFLAGS += -fPIC -O2
        INCLUDEPATH += /opt/local/include

        INCLUDEPATH += /opt/local/include/libxml2
        QMAKE_LIBDIR += /opt/local/lib $$PWD/../../../../../lib
        DEFINES += QT_NO_PRINTER
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

CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../lib/
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../lib/debug

LIBS += -lkipl -lQtAddons -lModuleConfig -lQtModuleConfigure -lnGIFramework

INCLUDEPATH += $$PWD/../../../../imagingsuite/GUI/qt/QtAddons/
DEPENDPATH += $$PWD/../../../../imagingsuite/GUI/qt/QtAddons/

INCLUDEPATH += $$PWD/../../../frameworks/ngi/nGIFramework/include/
DEPENDPATH += $$PWD/../../../frameworks/ngi/nGIFramework/src

INCLUDEPATH += $$PWD/../../../../imagingsuite/GUI/qt/QtModuleConfigure
DEPENDPATH += $$PWD/../../../../imagingsuite/GUI/qt/QtModuleConfigure

INCLUDEPATH += $$PWD/../../../../imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH += $$PWD/../../../../imagingsuite/core/modules/ModuleConfig/src

INCLUDEPATH += $$PWD/../../../../imagingsuite/core/kipl/kipl/include/
DEPENDPATH += $$PWD/../../../../imagingsuite/core/kipl/kipl/include/

OTHER_FILES += \
    FeatureWishList.txt
