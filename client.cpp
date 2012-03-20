#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <QDebug>

Client::Client(int clifd, QString target, int port) :
    clifd(clifd), target(target), port(port)
{
}

Client::~Client()
{
    close(clifd);
}

int Client::startTargetConnection()
{
    struct sockaddr_in server;
    struct hostent *hp;

    int optlen = 1;

    // Create socket that will connect to the target machine
    if((sockfd = Socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket(): ");
        return -1;
    }

    // Set reuse
    if(Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optlen, sizeof(optlen)) < 0) {
        perror("Setsockopt(): ");
        return -1;
    }

    // Clear the sockaddr_in structure
    bzero((char*)&server, sizeof(struct sockaddr_in));

    // Populate sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    // Check if host is available
    if((hp = Gethostbyname(target.toLatin1().data())) == NULL) {
        perror("Gethostbyname(): ");
        return -1;
    }

    bcopy(hp->h_addr, (char *)&server.sin_addr, hp->h_length);

    // Connect to target machine
    if(Connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect(): ");
        return -1;
    }

    qDebug() << "Connected to: " << target;

    return 0;
}

void Client::sendToInternal(char *buff, int len)
{
    // Write all of it to the internal machine
    write(this->sockfd, buff, len);
}

void Client::sendToExternal(int sockfd)
{
    char buff[BUFF_LEN];
    int n;

    if(clifd != sockfd) {
        return;
    }

    qDebug() << "Internal listener started";

    while(1) {
        // Read all the data
        if((n = read(this->sockfd, buff, BUFF_LEN)) < 0) {
            if(errno == EINTR) {
                return;
            } else {
                perror("Read(): ");
                return;
            }
        }

        if(n == 0) {
            close(this->sockfd);
            close(clifd);

            return;
        }

        write(clifd, buff, n);
    }
}
