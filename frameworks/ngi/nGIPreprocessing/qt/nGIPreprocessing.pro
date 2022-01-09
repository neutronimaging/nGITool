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

REPOS = $$PWD/../../../../..
CONFIG(release, debug|release): DESTDIR = $$REPOS/lib
else:CONFIG(debug, debug|release): DESTDIR = $$REPOS/lib/debug

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

    QMAKE_LIBDIR += $$REPOS/ExternalDependencies/windows/lib
    INCLUDEPATH  += $$REPOS/ExternalDependencies/windows/include/cfitsio
    INCLUDEPATH  += $$REPOS/ExternalDependencies/windows/include/libxml2

    INCLUDEPATH  += $$REPOS/imagingsuite/external/include
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2 -llibtiff -lcfitsio
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

CONFIG(release, debug|release): LIBS += -L$$REPOS/lib
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug

LIBS +=  -lkipl -lModuleConfig -lImagingAlgorithms -lnGIFramework

INCLUDEPATH += $$REPOS/imagingsuite/core/algorithms/ImagingAlgorithms/include

INCLUDEPATH += $$PWD/../../nGIFramework/include
DEPENDPATH += $$PWD/../../nGIFramework/include

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH += $$REPOS/imagingsuite/core/kipl/kipl/src

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/
