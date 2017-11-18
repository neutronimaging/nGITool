#include <QtWidgets/QApplication>
#include <QSplashScreen>
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap(a.applicationDirPath()+"/../Resources/RT2D_splash.png");
    QSplashScreen splash(pixmap);

    splash.show();

    a.processEvents();

    MainWindow w;
    w.show();
    splash.finish(&w);
    
    return a.exec();
}
