#include "httpprotocol.h"

/*------HTTP Protocol Item-------*/

HTTPProtocolItem::HTTPProtocolItem(QTcpSocket *sock, HTTPParser *parser, const QString &oper)
    : client(sock), parser(parser),oper(oper)
{

}

bool HTTPProtocolItem::operator<<(const QString &msg)
{
    bool result;
    if(oper == "STATUS")
    {
        result = Send((*parser)["VERSION"]+" "+msg);//sends the version and the code (HTTP/1.1 200 OK)
    }
    else
    {
        result = Send(oper + ": "+msg);
    }
    return result;
}

bool HTTPProtocolItem::Send(QString s,std::function<QByteArray(QString)>encoder)
{
    client->write(encoder(s)+"\r\n");
    return client->waitForBytesWritten(5000);
}

/*------HTTP Protocol-------*/

/*
 *********************
    QByteArray version = parser["VERSION"].toUtf8();
 *********************
            client->write(version+" 200 Ok\r\n");
            client->write("Connection: close\r\n");
            client->write("Content-Type: text/html\r\n");
            client->write(("Content-Length: "+ QString::number(dwn_page.length()) +"\r\n").toUtf8());
            client->write("Content-Type: text/html\r\n");
            client->write("\r\n");
 */

HTTPProtocol::HTTPProtocol(QTcpSocket* sock,HTTPParser* parser)
    : client(sock),parser(parser)
{

}

HTTPProtocolItem HTTPProtocol::operator[](const QString &oper)
{
    return HTTPProtocolItem(client,parser,oper);
}

bool HTTPProtocol::End()
{
    client->write("\r\n");
    return client->waitForBytesWritten(5000);
}

