#include "server.h"
#include <QDebug>
#include <QFile>
#include <QDir>
#include "httpparser.h"
#include "httpprotocol.h"

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
    QString req = client->readAll();//requests files
    //send files
    qDebug()<<req;
    auto list = req.split("\r\n");//splits the request by "\r\n" (has an empty item)
//    auto arg1 = list[0].split(" ");//Get request
//    auto arg_connection = (list.length()<6?QStringList(""):list[6].split(":")) ;//Connection state
//    qDebug()<<arg_connection[0]<<"::"<<arg_connection[1];
    HTTPParser parser(list);
    HTTPProtocol protocol(client,&parser);
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
            protocol["STATUS"]<<"200 Ok";
            protocol["Connection"]<<"close";
            protocol["Content-Type"]<<"text/html";
            protocol["Content-Length"]<<QString::number(dwn_page.length());
            protocol.End();
            client->write(dwn_page.toUtf8());
        }
        else if(parser["GET"] == "/installer.rar")
        {
            qDebug()<<"sending installer.rar";
            //Browser request. Download installer.
            QFile file("installer.rar");
            file.open(QFile::ReadOnly);
            QByteArray dwn_page;
            if(file.isOpen())
            {
                dwn_page = file.readAll();
                file.close();
            }
            else
            {
                dwn_page = "";
            }
            if(dwn_page != "")
            protocol["STATUS"]<<"200 Ok";
            else protocol["STATUS"]<<"404 Not Found";
            protocol["Connection"]<<"close";
            protocol["Content-Type"]<<"text/rar";
            protocol["Content-Length"]<<QString::number(dwn_page.length());
            protocol.End();
            client->write(dwn_page);
        }
        else
        {
            QString dwn_page = "no...";
            protocol["STATUS"]<<"200 Ok";
            protocol["Connection"]<<"close";
            protocol["Content-Type"]<<"text/html";
            protocol["Content-Length"]<<QString::number(dwn_page.length());
            protocol.End();
            client->write(dwn_page.toUtf8());
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
