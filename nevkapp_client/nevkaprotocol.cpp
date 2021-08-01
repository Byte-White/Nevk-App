#include "nevkaprotocol.h"

NevkaProtocol::NevkaProtocol(QTcpSocket* sock,Version version)
    : version(version)
{
    client=sock;
}

QString NevkaProtocol::SendRequest(const QString &file)
{
    QString result="";
    switch(version)
    {
        case Version::NEVKA_1_0:
        result = Ver_1_0(Mode::REQUEST,file);
        break;
    default:
        break;
    }
    return result;
}

bool NevkaProtocol::Connect(int ms)
{
    if(!client->isOpen())
    client->connectToHost(hostname,80);
    return client->waitForConnected(ms);
}

void NevkaProtocol::Disconnect()
{
    client->close();
}

QByteArray NevkaProtocol::Receive()
{
    QByteArray result = "";
    if(client->waitForReadyRead(3000))
        result = client->readAll();

    return result;
}

bool NevkaProtocol::Write(QString s, std::function<QByteArray (QString)> encoder)
{
    client->write(encoder(s));
    return client->waitForBytesWritten(5000);
}



/*private communication server functions*/
QString NevkaProtocol::Ver_1_0(const Mode &mode,const QString &s)//s is instruction
{
    QString result;
    switch(mode)
    {
        case Mode::REQUEST:
            if(Connect())
            {
               Write("REQUEST "+s+" NEVKA/1.1\r\n");
               result = Receive();
               Disconnect();
            }
        break;
    default:
        break;
    }
    return result;
}
