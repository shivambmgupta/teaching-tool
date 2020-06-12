#include "mainwindow.h"

#include <QApplication>
#include <QTimer>
#include <QSplashScreen>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    QPixmap pixmap(":/icons/icons/splashScreen.png");
    QSplashScreen splash(pixmap, Qt::WindowStaysOnTopHint);
    splash.show();

    MainWindow w;

    QTimer::singleShot(2000, &splash, SLOT(close()));
    QTimer::singleShot(2000, &w, SLOT(showMaximized()));

    return a.exec();
}
