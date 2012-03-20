#ifndef CLIENT_H
#define CLIENT_H

#include <QObject>
#include <QString>
#include "socket.h"

class Client : public QObject
{
    Q_OBJECT 
public:
    explicit Client(int clifd, QString target, int port);
    ~Client();
    int startTargetConnection();

public slots:
    void sendToInternal(char *buff, int len);
    void sendToExternal(int sockfd);

private:
    int clifd;
    QString ip;
    QString target;
    int port;
    int cliPort;
    int sockfd;

signals:

public slots:
    //void sendToTarget();
};

#endif // CLIENT_H
