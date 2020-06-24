#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T17:56:10
#
#-------------------------------------------------

QT       -= gui

TARGET = nGIFramework
TEMPLATE = lib
CONFIG += c++11

CONFIG(release, debug|release): DESTDIR = $$PWD/../../../../../lib
else:CONFIG(debug, debug|release): DESTDIR = $$PWD/../../../../../lib/debug

DEFINES += NGIFRAMEWORK_LIBRARY

SOURCES += \
    ../src/stdafx.cpp \
    ../src/ProjectionReader.cpp \
    ../src/PreprocModuleBase.cpp \
    ../src/nGIProcessor.cpp \
    ../src/nGIFactory.cpp \
    ../src/nGIException.cpp \
    ../src/nGIEngine.cpp \
    ../src/nGIConfig.cpp \
    ../src/ModuleItem.cpp \
    ../src/InteractionBase.cpp \
    ../src/EstimatorBase.cpp \
    ../src/ngigenerator.cpp

HEADERS += \
    ../include/targetver.h \
    ../include/stdafx.h \
    ../include/ProjectionReader.h \
    ../include/PreprocModuleBase.h \
    ../include/nGIProcessor.h \
    ../include/nGIFactory.h \
    ../include/nGIException.h \
    ../include/nGIEngine.h \
    ../include/nGIConfig.h \
    ../include/ModuleItem.h \
    ../include/InteractionBase.h \
    ../include/EstimatorBase.h \
    ../src/stdafx.h \
    ../include/nGIFramework_global.h \
    ../include/ngigenerator.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE85D0E0A
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = nGIFramework.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {

    QMAKE_CXXFLAGS += -fPIC -O2

    unix:!macx {
        QMAKE_CXXFLAGS += -fopenmp
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
       INCLUDEPATH += /usr/include/libxml2
    }

    unix:macx {
        INCLUDEPATH += /opt/local/include/ /opt/local/include/libxml2
        QMAKE_LIBDIR += /opt/local/lib
    }

    LIBS += -lxml2 -ltiff -lcfitsio


    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
    QMAKE_LFLAGS += /MACHINE:X64
    }
    INCLUDEPATH  += ../../../../../imagingsuite/external/src/linalg
    INCLUDEPATH  += ../../../../../imagingsuite/external/include
    INCLUDEPATH  += ../../../../../imagingsuite/external/include/cfitsio
    QMAKE_LIBDIR += $$PWD/../../../../../imagingsuite/external/lib64

    LIBS += -llibxml2_dll -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2
}

CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../lib -lkipl -lModuleConfig
else:CONFIG(debug, debug|release):   LIBS += -L$$PWD/../../../../../lib/debug -lkipl -lModuleConfig

INCLUDEPATH += $$PWD/../../../../../imagingsuite/core/kipl/kipl/include
DEPENDPATH += $$PWD/../../../../../imagingsuite/core/kipl/kipl/src

INCLUDEPATH += $$PWD/../../../../../imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH += $$PWD/../../../../../imagingsuite/core/modules/ModuleConfig/src


