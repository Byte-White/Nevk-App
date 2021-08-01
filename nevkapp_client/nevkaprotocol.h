#ifndef NEVKAPROTOCOL_H
#define NEVKAPROTOCOL_H

#include<QTcpSocket>

class NevkaProtocol
{
public:
enum Version
{
    NONE=0,
    NEVKA_1_0
};
    QTcpSocket* client;
    Version version;
    QString hostname="localhost";
private:
    enum Mode
    {
      UNKNOWN=0,
      REQUEST
    };
public:
    ///default encode function used in NevkaProtocol::Write
    static QByteArray DefaultEncodeFunction(QString s)
    {
        return s.toUtf8();
    }
public:
    NevkaProtocol(QTcpSocket* sock,Version version=Version::NEVKA_1_0);
    ///Sends file request
    QString SendRequest(const QString& file);
    bool Connect(int ms=3000);
    void Disconnect();
    QByteArray Receive();
    bool Write(QString s,std::function<QByteArray(QString)>encoder=NevkaProtocol::DefaultEncodeFunction);
private:
    ///version 1.0 communication
    QString Ver_1_0(const Mode &mode,const QString& s);
};

#endif // NEVKAPROTOCOL_H
