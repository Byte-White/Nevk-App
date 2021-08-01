#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    this->setWindowTitle("Nevk-App");
    //this->setMinimumWidth(this->width());
    //this->setMaximumWidth(this->width());
    //this->setMinimumHeight(this->height());
    //this->setMaximumHeight(this->height());
    //ui->dockWidget->setWindowTitle("Pages");


    QIcon icon("icon.jpg");
    setWindowIcon(icon);

    client = new QTcpSocket(this);
        client->connectToHost("localhost",80);
        if(client->waitForConnected(3000))
        {
            client->write("REQUEST / NEVKA/1.0\r\n");
            client->waitForBytesWritten();

            if(client->waitForReadyRead(3000))
            {
                auto list = client->readAll().split('\n');
                for(auto i:list)
                {
                    if(i !="")
                    ui->listWidget->addItem(i);
                }
                client->close();
            }
            else
            {
                QMessageBox::critical(this,"Error", "No response.");
            }
        }
}

QString MainWindow::ReceivePage(QString page,bool write)
{
    client->write("REQUEST "+page.toUtf8()+" NEVKA/1.0\r\n");
    if(!client->waitForBytesWritten(3000))QMessageBox::critical(this,"Error", "Couldn't write to server.\n"+page);

    if(client->waitForReadyRead(3000))
    {
        QString l=client->readAll();
        if(write)
        ui->textBrowser->setHtml(l);
        return l;
    }
    else
    {
        QMessageBox::critical(this,"Error", "No response.\n"+page);
    }
    return "";
}

void MainWindow::SafeWrite(std::function<void()> func)
{
    if(client->isOpen())
    {
        client->close();
    }
    client->connectToHost("localhost",80);
    if(client->waitForConnected(3000))func();
    else QMessageBox::critical(this,"Error", "could not connect.");
    client->close();
}


MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    auto l = [item,this](){ReceivePage(item->text());};
    SafeWrite(l);
}


void MainWindow::on_actionShow_Pages_View_triggered()
{

    ui->dockWidget->setHidden(!ui->dockWidget->isHidden());
}


void MainWindow::on_actionExit_triggered()
{
    exit(0);
}


void MainWindow::on_actionDownload_triggered()
{
    QModelIndex index = ui->listWidget->currentIndex();
    QString itemText = index.data(Qt::DisplayRole).toString();
    if(itemText == "")
    {
        QMessageBox::critical(this,"Error","Select a page.");
        return;
    }
    QFileDialog dialog;
    QString filename = dialog.getSaveFileName(this,"Save File - "+itemText,QCoreApplication::applicationDirPath()+"/saved","HTML (*.html)");
    QFile file(filename);
    file.open(QFile::WriteOnly);
    if(!file.isOpen())
    {
        QMessageBox::critical(this,"Error", "Could not save file.");
        return;
    }
    QString str;


    auto l = [&str,itemText,this](){str = ReceivePage(itemText,false);};
    SafeWrite(l);

    file.write(str.toUtf8());
    file.close();
}


void MainWindow::on_actionSave_All_triggered()
{

    for (int i = 0; i<ui->listWidget->count();i++)
    {
        QString itemText = ui->listWidget->item(i)->text();
        QFileDialog dialog;
        QString filename = dialog.getSaveFileName(this,"Save File - " + itemText,QCoreApplication::applicationDirPath()+"/saved","HTML (*.html)");
        QFile file(filename);
        file.open(QFile::WriteOnly);
        if(!file.isOpen())
        {
            QMessageBox::critical(this,"Error", "Could not save file.");
            continue;
        }
        QString str;


        auto l = [&str,itemText,this](){str = ReceivePage(itemText,false);};
        SafeWrite(l);

        file.write(str.toUtf8());
        file.close();
    }
}

