#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T18:04:16
#
#-------------------------------------------------

QT       -= gui

TARGET = nGIEstimators
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
    #    QMAKE_MAC_SDK = macosx10.11
        INCLUDEPATH += /opt/local/include
        QMAKE_LIBDIR += /opt/local/lib
    }

    LIBS +=  -lxml2 -ltiff

}



DEFINES += NGIESTIMATORS_LIBRARY

SOURCES += \
    ../src/stdafx.cpp \
    ../src/nGIStandardEstimator.cpp \
    ../src/nGIPenalizedMLEstimator.cpp \
    ../src/nGILSEstimator.cpp \
    ../src/nGIEstimators.cpp \
    ../src/nGIBLUEEstimator.cpp \
    ../src/dllmain.cpp

HEADERS += \
    ../include/stdafx.h \
    ../include/nGIStandardEstimator.h \
    ../include/nGIPenalizedMLEstimator.h \
    ../include/nGILSEstimator.h \
    ../include/nGIEstimators.h \
    ../include/nGIBLUEEstimator.h \
    ../src/stdafx.h \
    ../include/ngiestimators_global.h

symbian {
    MMP_RULES += EXPORTUNFROZEN
    TARGET.UID3 = 0xEA3C8943
    TARGET.CAPABILITY = 
    TARGET.EPOCALLOWDLLDATA = 1
    addFiles.sources = nGIEstimators.dll
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
}
else {
    LIBS += -L/usr/lib -lxml2 -ltiff
    INCLUDEPATH += /usr/include/libxml2
}

INCLUDEPATH += $$PWD/../../../../../external/src/linalg


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../build-nGIFramework-Qt5-Release/release -lnGIFramework
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../build-nGIFramework-Qt5-Debug/debug -lnGIFramework
else:symbian: LIBS += -lnGIFramework
else:unix:CONFIG(release, debug|release) LIBS += -L$$PWD/../../../../../lib -lkipl -lModuleConfig -lnGIFramework
else:unix:CONFIG(debug, debug|release) LIBS += -L$$PWD/../../../../../lib/debug -lkipl -lModuleConfig -lnGIFramework

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Release/release -lModuleConfig
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Debug/debug -lModuleConfig

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/build-kipl-Qt5-Release/release -lkipl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/build-kipl-Qt5-Debug/debug -lkipl


INCLUDEPATH += $$PWD/../../../../../kipl/trunk/kipl/include
DEPENDPATH += $$PWD/../../../../../kipl/trunk/kipl/include

INCLUDEPATH += $$PWD/../../nGIFramework/include
DEPENDPATH += $$PWD/../../nGIFramework/include

INCLUDEPATH += $$PWD/../../../../../modules/trunk/ModuleConfig/include
DEPENDPATH += $$PWD/../../../../../modules/trunk/ModuleConfig/src
