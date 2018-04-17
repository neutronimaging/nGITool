#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T20:37:06
#
#-------------------------------------------------

QT       += core widgets printsupport

TARGET = QtnGITool
TEMPLATE = app
CONFIG += c++11

CONFIG(release, debug|release): DESTDIR = $$PWD/../../../../../Applications
else:CONFIG(debug, debug|release): DESTDIR = $$PWD/../../../../Applications/debug

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
    unix:!macx {

        QMAKE_CXXFLAGS += -fopenmp
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
    }

    unix:macx {
      #  QMAKE_MAC_SDK = macosx10.12
        INCLUDEPATH += /opt/local/include
        QMAKE_LIBDIR += /opt/local/lib $$PWD/../../../../../lib
        DEFINES += QT_NO_PRINTER
        ICON = $$PWD/ngi_icon.icns
    }

    LIBS +=  -lxml2 -ltiff

}

ICON = ../Resources/ngi_icon.icns

win32:CONFIG(release, debug|release):     LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/build-kipl-Qt5-Release/release -lkipl
else:win32:CONFIG(debug, debug|release):  LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/build-kipl-Qt5-Debug/debug -lkipl
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../lib -lkipl -lModuleConfig -lQtAddons -lQtModuleConfigure -lnGIFramework
else:unix:CONFIG(debug, debug|release):   LIBS += -L$$PWD/../../../../../lib/debug/ -lkipl -lModuleConfig -lQtAddons -lQtModuleConfigure -lnGIFramework

win32:CONFIG(release, debug|release):     LIBS += -L$$PWD/../../../../../gui/trunk/qt/build-QtAddons-Qt5-Release/release -lQtAddons
else:win32:CONFIG(debug, debug|release):  LIBS += -L$$PWD/../../../../../gui/trunk/qt/build-QtAddons-Qt5-Debug/debug -lQtAddons

win32:CONFIG(release, debug|release):     LIBS += -L$$PWD/../../../build-nGIFramework-Qt5-Release/release -lnGIFramework
else:win32:CONFIG(debug, debug|release):  LIBS += -L$$PWD/../../../build-nGIFramework-Qt5-Debug/debug -lnGIFramework

win32:CONFIG(release, debug|release):     LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Release/release -lModuleConfig
else:win32:CONFIG(debug, debug|release):  LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Debug/debug -lModuleConfig

win32:CONFIG(release, debug|release):     LIBS += -L$$PWD/../../../../../gui/trunk/qt/build-QtModuleConfigure-Qt5-Release/release -lQtModuleConfigure
else:win32:CONFIG(debug, debug|release):  LIBS += -L$$PWD/../../../../../gui/trunk/qt/build-QtModuleConfigure-Qt5-Debug/debug -lQtModuleConfigure


INCLUDEPATH += $$PWD/../../../../../gui/trunk/qt/QtAddons
DEPENDPATH += $$PWD/../../../../../gui/trunk/qt/QtAddons

INCLUDEPATH += $$PWD/../../../core/nGIFramework/include
DEPENDPATH += $$PWD/../../../core/nGIFramework/include

INCLUDEPATH += $$PWD/../../../../../gui/trunk/qt/QtModuleConfigure
DEPENDPATH += $$PWD/../../../../../gui/trunk/qt/QtModuleConfigure

INCLUDEPATH += $$PWD/../../../../../modules/trunk/ModuleConfig/include
DEPENDPATH += $$PWD/../../../../../modules/trunk/ModuleConfig/include

INCLUDEPATH += $$PWD/../../../../../kipl/trunk/kipl/include
DEPENDPATH += $$PWD/../../../../../kipl/trunk/kipl/include

OTHER_FILES += \
    FeatureWishList.txt
