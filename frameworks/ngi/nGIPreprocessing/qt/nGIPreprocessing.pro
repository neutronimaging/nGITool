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
    INCLUDEPATH += "../../../../external/src/linalg"
    QMAKE_CXXFLAGS += -fPIC -O2

    INCLUDEPATH += /usr/include/libxml2
    unix:!macx {

        QMAKE_CXXFLAGS += -fopenmp
        QMAKE_LFLAGS += -lgomp
        LIBS += -lgomp
    }

    unix:macx {
  #      QMAKE_MAC_SDK = macosx10.11
        INCLUDEPATH += /opt/local/include
        QMAKE_LIBDIR += /opt/local/lib
    }

    LIBS += -lxml2 -ltiff
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
else {
    LIBS += -L/usr/lib -lxml2 -ltiff
    INCLUDEPATH += /usr/include/libxml2
}


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Release/release -lModuleConfig
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Debug/debug -lModuleConfig
else:unix:CONFIG(release, debug|release) LIBS += -L$$PWD/../../../../../lib -lkipl -lModuleConfig -lImagingAlgorithms -lnGIFramework
else:unix:CONFIG(debug, debug|release) LIBS += -L$$PWD/../../../../../lib/debug -lkipl -lModuleConfig -lImagingAlgorithms -lnGIFramework


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/build-kipl-Qt5-Release/release -lkipl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/build-kipl-Qt5-Debug/debug -lkipl

win32:CONFIG(release, debug|release):     LIBS += -L$$PWD/../../../../../qni/trunk/src/ImagingAlgorithms/build-ImagingAlgorithms-Qt5-Release/release -lImagingAlgorithms
else:win32:CONFIG(debug, debug|release):  LIBS += -L$$PWD/../../../../../qni/trunk/src/ImagingAlgorithms/build-ImagingAlgorithms-Qt5-Debug/debug/ -lImagingAlgorithms

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../build-nGIFramework-Qt5-Release/release -lnGIFramework
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../build-nGIFramework-Qt5-Debug/debug -lnGIFramework


INCLUDEPATH += $$PWD/../../../../../qni/trunk/src/ImagingAlgorithms/include
DEPENDPATH += $$PWD/../../../../../qni/trunk/src/ImagingAlgorithms/include

INCLUDEPATH += $$PWD/../../nGIFramework/include
DEPENDPATH += $$PWD/../../nGIFramework/include

INCLUDEPATH += $$PWD/../../../../../kipl/trunk/kipl/include
DEPENDPATH += $$PWD/../../../../../kipl/trunk/kipl/include

INCLUDEPATH += $$PWD/../../../../../modules/trunk/ModuleConfig/include
DEPENDPATH += $$PWD/../../../../../modules/trunk/ModuleConfig/src
