#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T20:12:02
#
#-------------------------------------------------

QT       -= gui
QT       += core

TARGET = nGIPreprocessing
TEMPLATE = lib
CONFIG += c++11

CONFIG(release, debug|release): DESTDIR = $$PWD/../../../../../lib
else:CONFIG(debug, debug|release): DESTDIR = $$PWD/../../../../../lib/debug

unix {

    QMAKE_CXXFLAGS += -fPIC -O2


    unix:!macx {
        INCLUDEPATH += /usr/include/libxml2
        QMAKE_CXXFLAGS += -fopenmp
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
    }

    unix:macx {
        INCLUDEPATH += /opt/local/include /opt/local/include/libxml2
        QMAKE_LIBDIR += /opt/local/lib
    }

    LIBS += -lxml2 -ltiff
}

win32 {
    contains(QMAKE_HOST.arch, x86_64):{
    QMAKE_LFLAGS += /MACHINE:X64
    }
    INCLUDEPATH  += $$PWD/../../../../../imagingsuite/external/include
    INCLUDEPATH  += $$PWD/../../../../../imagingsuite/external/include/cfitsio
    QMAKE_LIBDIR += $$PWD/../../../../../imagingsuite/external/lib64

    LIBS += -llibxml2_dll -llibtiff -lcfitsio
    QMAKE_CXXFLAGS += /openmp /O2

    DEFINES += NOMINMAX
}
else {
    LIBS += -L/usr/lib -lxml2 -ltiff
    INCLUDEPATH += /usr/include/libxml2
}


DEFINES += NGIPREPROCESSING_LIBRARY

SOURCES += \
    ../src/nGISpotClean.cpp \
    ../src/nGIPreprocessing.cpp \
    ../src/nGILogNorm.cpp \
    ../src/nGIISSfilter.cpp

HEADERS += \
    ../include/stdafx.h \
    ../include/nGISpotClean.h \
    ../include/nGIPreprocessing.h \
    ../include/nGILogNorm.h \
    ../include/nGIISSfilter.h \
    ../src/stdafx.h \
    ../include/ngipreprocessing_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xE8CDC354
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = nGIPreprocessing.dll
    addFiles.path = !:/sys/bin
    DEPLOYMENT += addFiles
}

unix:!symbian {
    maemo5 {
        target.path = /opt/usr/lib
    } else {
        target.path = /usr/lib
    }
    INSTALLS += target
}




CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../lib -lModuleConfig
else:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../lib/debug -lModuleConfig
LIBS +=  -lkipl -lModuleConfig -lImagingAlgorithms -lnGIFramework

INCLUDEPATH += $$PWD/../../../../../imagingsuite/core/algorithms/ImagingAlgorithms/include

INCLUDEPATH += $$PWD/../../nGIFramework/include
DEPENDPATH += $$PWD/../../nGIFramework/include

INCLUDEPATH += $$PWD/../../../../../imagingsuite/core/kipl/kipl/include
DEPENDPATH += $$PWD/../../../../../imagingsuite/core/kipl/kipl/src

INCLUDEPATH += $$PWD/../../../../../imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH += $$PWD/../../../../../imagingsuite/core/modules/ModuleConfig/
