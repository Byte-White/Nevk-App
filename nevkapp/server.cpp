#include "server.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include "httpparser.h"

Server::Server()
{
    client = nullptr;
    server = new QTcpServer();
    connect(server,SIGNAL(newConnection()),this,SLOT(newConnection()));

    if(!server->listen(QHostAddress::Any,80))
    {
        qDebug()<<"Couldn't listen";
        return;
    }
    else
    {
        QString s="Listening on port "+ QString::number(server->serverPort())+".";
        qDebug()<<s;
    }
}

// https://developer.mozilla.org/en-US/docs/Web/HTTP/Status
void Server::newConnection()
{
    client = server->nextPendingConnection();
    QString ip = QString::number(client->localAddress().toIPv4Address());
    QString str="New connection ("+ip+")!";
    qDebug()<<str;
    if(!client->waitForReadyRead(3000))
    {
        qDebug()<<"Time out! no response.";
        return;
    }
    read_request:
    QString req = client->readAll();//requests files
    //send files
    qDebug()<<req;
    auto list = req.split("\r\n");//splits the request by "\r\n" (has an empty item)
//    auto arg1 = list[0].split(" ");//Get request
//    auto arg_connection = (list.length()<6?QStringList(""):list[6].split(":")) ;//Connection state
//    qDebug()<<arg_connection[0]<<"::"<<arg_connection[1];
    HTTPParser parser(list);
    QByteArray version = parser["VERSION"].toUtf8();
    if(parser.map.contains("GET"))
    {
        if(parser["GET"] == "/")
        {
            //Browser request. Get Index page
            QFile file("download.html");
            file.open(QFile::ReadOnly);
            QString dwn_page;
            if(file.isOpen())
            {
               dwn_page = file.readAll();
            }
            else
            {
                dwn_page = "<h1>There was some kind of error.<h1><br><h6>we are working on it.</h6>";
            }
            client->write(version+" 200 Ok\r\n");
            client->write("Content-Type: text/html\r\n");
            client->write(("Content-Length: "+ QString::number(dwn_page.length()) +"\r\n").toUtf8());
            client->write("Content-Type: text/html\r\n");
            client->write("\r\n");
            client->write(dwn_page.toUtf8());
            if(parser["Connection"]=="keep-alive")goto read_request;
        }
        else if(parser["GET"] == "/installer.rar")
        {
            qDebug()<<"sending installer.rar";
            //Browser request. Download installer.
            QFile file("installer.rar");
            file.open(QFile::ReadOnly);
            QByteArray dwn;
            if(file.isOpen())
            {
                dwn = file.readAll();
                file.close();
            }
            else
            {
                dwn = "";
            }
            if(dwn != "") client->write(version+" 200 Ok\r\n");
            else client->write(version+" 404 Not Found\r\n");
            client->write("Content-Type: text/html\r\n");
            client->write(("Content-Length: "+ QString::number(dwn.length()) +"\r\n").toUtf8());
            client->write("Content-Type: text/rar\r\n");
            client->write("\r\n");
            client->write(dwn);
        }
        else
        {
            QString msg = "no...";
            client->write(version+" 200 Ok\r\n");
            client->write("Content-Type: text/html\r\n");
            client->write(("Content-Length: "+ QString::number(msg.length()) +"\r\n").toUtf8());
            client->write("Content-Type: text/html\r\n");
            client->write("\r\n");
            client->write(msg.toUtf8());
        }
    }
    else if(parser.map.contains("REQUEST"))
    {
        //qDebug()<<"msg:"<<parser["REQUEST"];
        if(parser["REQUEST"] == "/")
        {
            QString list;
            //assume the directory exists and contains some files and you want all jpg and JPG files
            QDir directory("database");
            QStringList pages= directory.entryList(QStringList() << "*.html",QDir::Files);
            for(QString filename: pages)
            {
                auto f = filename.split('.');
                list += f[0]+"\n";
                //qDebug()<<list;
            }
            client->write(list.toUtf8());
            client->flush();
        }
        else
        {
            QString filename = parser["REQUEST"];
            QFile file("database/"+filename+".html");
            file.open(QFile::ReadOnly);
            QString msg = file.readAll();

            client->write(msg.toUtf8());
            client->flush();
            file.close();
        }
    }
}
