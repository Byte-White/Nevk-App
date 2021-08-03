#ifndef HTTPPROTOCOL_H
#define HTTPPROTOCOL_H

#include <QString>
#include <QTcpSocket>
#include <httpparser.h>

/*------HTTP Protocol Item-------*/
class HTTPProtocolItem
{
    QTcpSocket* client;
    HTTPParser* parser;
    QString oper;
    static QByteArray DefaultEncodeFunction(QString s)
    {
        return s.toUtf8();
    }
public:
    ///Sends file request with "\\r\\n"
    bool Send(QString s,std::function<QByteArray(QString)>encoder=HTTPProtocolItem::DefaultEncodeFunction);

public:
    HTTPProtocolItem(QTcpSocket* sock,HTTPParser* parser,const QString& oper);
    bool operator<<(const QString& msg);
};
/*------HTTP Protocol-------*/
class HTTPProtocol
{
    QTcpSocket* client;
    HTTPParser* parser;
public:
    HTTPProtocol(QTcpSocket* sock, HTTPParser* parser);
public:
    ///default encode function used in NevkaProtocol::Write

    HTTPProtocolItem operator[](const QString& oper);

    bool End();
};

#endif // HTTPPROTOCOL_H
