#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTcpServer>
#include <QTcpSocket>

class Server: public QObject
{
    Q_OBJECT
public:
    Server();

private slots:
    void newConnection();
private:
    QTcpServer* server;
    QTcpSocket* client;
};

#endif // SERVER_H
