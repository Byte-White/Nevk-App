#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTcpSocket>
#include <QListWidget>
#include <QListWidgetItem>

#include "nevkaprotocol.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
private slots:
    void on_listWidget_itemDoubleClicked(QListWidgetItem *item);

    void on_actionShow_Pages_View_triggered();

    void on_actionExit_triggered();

    void on_actionDownload_triggered();

    void on_actionSave_All_triggered();

private:
    QString ReceivePage(QString page,bool write = true);
private:
    Ui::MainWindow *ui;
    QTcpSocket* client;
    NevkaProtocol protocol;
};
#endif // MAINWINDOW_H
