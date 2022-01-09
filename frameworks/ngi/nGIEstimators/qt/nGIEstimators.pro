#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T18:04:16
#
#-------------------------------------------------

QT       -= gui

TARGET = nGIEstimators
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

    INCLUDEPATH += $$REPOS/imagingsuite/external/include
    INCLUDEPATH += $$REPOS/imagingsuite/external/include/cfitsio
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

    QMAKE_LIBDIR += $$REPOS/ExternalDependencies/windows/lib
    INCLUDEPATH  += $$REPOS/ExternalDependencies/windows/include/cfitsio
    INCLUDEPATH  += $$REPOS/ExternalDependencies/windows/include/libxml2

    INCLUDEPATH  += $$REPOS/imagingsuite/external/include
    QMAKE_LIBDIR += $$REPOS/imagingsuite/external/lib64

    LIBS += -llibxml2 -llibtiff -lcfitsio
    LIBS += -llibopenblas

    QMAKE_CXXFLAGS += /openmp /O2
}
else {
    LIBS += -L/usr/lib -lxml2 -ltiff -larmadillo
    INCLUDEPATH += /usr/include/libxml2
}

CONFIG(release, debug|release): LIBS += -L$$REPOS/lib
else:CONFIG(debug, debug|release): LIBS += -L$$REPOS/lib/debug

LIBS += -lkipl -lModuleConfig -lnGIFramework

INCLUDEPATH += $$REPOS/imagingsuite/core/kipl/kipl/include
DEPENDPATH += $$REPOS/imagingsuite/core/kipl/kipl/include

INCLUDEPATH += $$PWD/../../nGIFramework/include
DEPENDPATH += $$PWD/../../nGIFramework/include

INCLUDEPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/include
DEPENDPATH += $$REPOS/imagingsuite/core/modules/ModuleConfig/src
