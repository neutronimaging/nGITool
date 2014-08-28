#-------------------------------------------------
#
# Project created by QtCreator 2014-04-29T20:37:06
#
#-------------------------------------------------

QT       += core widgets printsupport


TARGET = QtnGITool
TEMPLATE = app


SOURCES += main.cpp\
        ngimainwindow.cpp \
    ngireport.cpp

HEADERS  += ngimainwindow.h \
    ngireport.h

FORMS    += ngimainwindow.ui

LIBS += -L/usr/lib -L/usr/local/lib -lxml2 -ltiff
INCLUDEPATH += /usr/include/libxml2

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/QtAddons-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/release/ -lQtAddons
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/QtAddons-build_Qt_4_8_1_for_GCC__Qt_SDK__Release/debug/ -lQtAddons
else:symbian: LIBS += -lQtAddons
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/build-QtAddons-Qt5-Release/ -lQtAddons
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/build-QtAddons-Qt5-Debug/ -lQtAddons

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../nGIFramework/nGIFramework-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Release/release/ -lnGIFramework
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../nGIFramework/nGIFramework-build-desktop-Desktop_Qt_4_8_1_for_GCC__Qt_SDK__Release/debug/ -lnGIFramework
else:symbian: LIBS += -lnGIFramework
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../core/nGIFramework/build-nGIFramework-Qt5-Release/ -lnGIFramework
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../core/nGIFramework/build-nGIFramework-Qt5-Debug/ -lnGIFramework

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/ModuleConfig-build-Qt_4_8_1_Release/release/ -lModuleConfig
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/ModuleConfig-build-Qt_4_8_1_Release/debug/ -lModuleConfig
else:symbian: LIBS += -lModuleConfig
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Release/ -lModuleConfig
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../modules/trunk/ModuleConfig/build-ModuleConfig-Qt5-Debug/ -lModuleConfig

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/QtModuleConfigure-build-Qt_4_8_1_for_GCC__Qt_SDK__Release/release/ -lQtModuleConfigure
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/QtModuleConfigure-build-Qt_4_8_1_for_GCC__Qt_SDK__Release/debug/ -lQtModuleConfigure
else:symbian: LIBS += -lQtModuleConfigure
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/build-QtModuleConfigure-Qt5-Release/ -lQtModuleConfigure
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../gui/trunk/qt/build-QtModuleConfigure-Qt5-Debug/ -lQtModuleConfigure

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/kipl-build-Qt_4_8_1_Release/release/ -lkipl
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/kipl-build-Qt_4_8_1_Release/debug/ -lkipl
else:symbian: LIBS += -lkipl
else:unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/build-kipl-Qt5-Release/ -lkipl
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../../../../../kipl/trunk/kipl/build-kipl-Qt5-Debug/ -lkipl

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
