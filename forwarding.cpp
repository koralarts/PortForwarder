#include "forwarding.h"
#include "stdio.h"

Forwarding::Forwarding(int port, QString target, int proto)
{
    this->port = port;
    this->target = target;

    if(proto == 0) {
        if((sockfd = Socket(AF_INET, SOCK_STREAM, 0)) < 0) {
            perror("Socket(): ");
        }
    }
}
