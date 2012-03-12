#ifndef FORWARDING_H
#define FORWARDING_H

#include "socket.h"
#include <QString>

class Forwarding
{
public:
    Forwarding(int port, QString target, int proto = 0);

private:
    int port;
    QString target;
    int sockfd;
};

#endif // FORWARDING_H
