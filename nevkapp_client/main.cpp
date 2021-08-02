#include "mainwindow.h"
#include <QFile>
#include <QApplication>
#include <QSplashScreen>

#include<thread>
#include <QMessageBox>

#include "appdata.h"

AppData AppData::appdata_inst;

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QPixmap pixmap("resources/splash.png");//875,500
    QIcon icon("resources/icon.jpg");
    QSplashScreen splash(pixmap);
    MainWindow w;

    splash.show();
    a.processEvents();

    QTcpSocket* check = new QTcpSocket();
    check->connectToHost("localhost",80);

    if(!check->waitForConnected(10000))
    {
        //QMessageBox::critical(nullptr,"Error","Could not connect to the server.");
        AppData::Mode()=false;
        w.setWindowTitle(w.windowTitle()+" - offline");
    }
    else check->close();
    QFile theme("resources/theme.qss");
    theme.open(QFile::ReadOnly);
    QString theme_s = theme.readAll();
    w.setWindowIcon(icon);
    w.setStyleSheet(theme_s);

    theme.close();
    w.show();
    splash.finish(&w);
    w.LoadPagesInfo();
    return a.exec();
}
