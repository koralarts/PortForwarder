#ifndef FORWARDING_H
#define FORWARDING_H

#include "socket.h"
#include <QString>
#include <QObject>

class Forwarding : public QObject
{
    Q_OBJECT
public:
    Forwarding(int port, int listenPort, QString target);
    ~Forwarding();

public slots:
    void startListening(int checkPort);

private:
    int sockfd;
    int port;
    int listenPort;
    QString target;

signals:
    void isRunning(QString port, bool running);
    void sendToExternal(int);
};

#endif // FORWARDING_H
