#-------------------------------------------------
#
# Project created by QtCreator 2014-03-16T20:21:34
#
#-------------------------------------------------

QT       += core gui

TARGET = RootTracker2D
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    RootTrackConfig.cpp \
    rootfinder.cpp

HEADERS  += mainwindow.h \
    RootTrackConfig.h \
    rootfinder.h

FORMS    += mainwindow.ui

LIBS += -ltiff -lz
LIBS += -L/usr/lib -lxml2

unix:!macx {
    QMAKE_CXXFLAGS += -fopenmp
    QMAKE_LFLAGS += -lgomp
    LIBS += -lgomp
}

INCLUDEPATH += /usr/include/libxml2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../src/libs/kipl/trunk/kipl/kipl-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/release/ -lkipl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../src/libs/kipl/trunk/kipl/kipl-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/debug/ -lkipl
else:symbian: LIBS += -lkipl
else:unix: LIBS += -L$$PWD/../../../../../src/libs/kipl/trunk/kipl/kipl-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/ -lkipl

INCLUDEPATH += $$PWD/../../../../../src/libs/kipl/trunk/kipl/include
DEPENDPATH += $$PWD/../../../../../src/libs/kipl/trunk/kipl/include

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/QtAddons-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/release/ -lQtAddons
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/QtAddons-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/debug/ -lQtAddons
else:symbian: LIBS += -lQtAddons
else:unix: LIBS += -L$$PWD/../../../../../gui/trunk/qt/QtAddons-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/ -lQtAddons

INCLUDEPATH += $$PWD/../../../../../gui/trunk/qt/QtAddons
DEPENDPATH += $$PWD/../../../../../gui/trunk/qt/QtAddons

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../src/libs/modules/trunk/ModuleConfig/ModuleConfig-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/release/ -lModuleConfig
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../src/libs/modules/trunk/ModuleConfig/ModuleConfig-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/debug/ -lModuleConfig
else:symbian: LIBS += -lModuleConfig
else:unix: LIBS += -L$$PWD/../../../../../src/libs/modules/trunk/ModuleConfig/ModuleConfig-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/ -lModuleConfig

INCLUDEPATH += $$PWD/../../../../../src/libs/modules/trunk/ModuleConfig/include
DEPENDPATH += $$PWD/../../../../../src/libs/modules/trunk/ModuleConfig/include
