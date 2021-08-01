#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMessageBox>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent),client(new QTcpSocket(this)),protocol(client)
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

        QString msg = protocol.SendRequest("/");//Gets all files list.
        if(msg != "")
        {
            auto list = msg.split('\n');
            for(auto i:list)
            {
                if(i !="")
                ui->listWidget->addItem(i);
            }
        }
        else
        {
            QMessageBox::critical(this,"Error", "Could not get pages.");
        }
}

QString MainWindow::ReceivePage(QString page,bool write)
{
    QString l = protocol.SendRequest(page);
    if(l=="")
        QMessageBox::critical(this,"Error", "No response. (\n"+page+")");
    else
    {
        if(write)
        ui->textBrowser->setHtml(l);
        return l;
    }
    return "";
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_listWidget_itemDoubleClicked(QListWidgetItem *item)
{
    ReceivePage(item->text());
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

    str = ReceivePage(itemText,false);

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

        str = ReceivePage(itemText,false);

        file.write(str.toUtf8());
        file.close();
    }
}

