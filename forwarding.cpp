#include "forwarding.h"
#include "epoll.h"
#include "socket.h"
#include "client.h"
#include <stdio.h>
#include <assert.h>
#include <errno.h>
#include <QDebug>
#include <QString>
#include <QThread>

Forwarding::Forwarding(int port, QString target) :
    port(port), target(target)
{
}

Forwarding::~Forwarding()
{
    close(sockfd);
}

void Forwarding::startListening(int checkPort)
{
    static struct epoll_event epEvents[E_POLL_QUEUE_LEN];
    static struct epoll_event epEvent;

    struct sockaddr_in server;
    struct sockaddr_in client;
    socklen_t cliLen = sizeof(&client);
    socklen_t optlen = 1;
    int epfd;
    int numfd;
    int clifd;

    QThread *thread;
    Client *clientCon;

    if(checkPort != port) {
        return;
    }

    qDebug() << "Starting listening for port: " << port;

    // Create new socket
    if((sockfd = Socket(AF_INET, SOCK_STREAM, 0)) < 0) {
        perror("Socket(): ");
    }

    // Set REUSE address
    if(Setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &optlen, sizeof(optlen)) < 0) {
        perror("Setsockopt(): ");
    }

    // Set NONBLOCKING
    if(Fcntl(sockfd, F_SETFL) < 0) {
        perror("Fcntl(): ");
    }

    bzero((char*)&server, sizeof(struct sockaddr_in));
    server.sin_family = AF_INET;
    server.sin_port = htons(port);
    server.sin_addr.s_addr = htonl(INADDR_ANY);

    // Bind socket
    if(Bind(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Bind(): ");
        return;
    }

    // Set Listen
    if(Listen(sockfd, LISTEN_QUEUE) < 0) {
        perror("Listen(): ");
        return;
    }

    // Create epoll file descriptor
    if((epfd = Epoll_create(E_POLL_QUEUE_LEN)) < 0) {
        perror("Epoll_create(): ");
        return;
    }

    epEvent.events = EPOLLIN | EPOLLERR | EPOLLHUP | EPOLLET;
    epEvent.data.fd = sockfd;

    // Add main sockfd
    if(Epoll_ctl(epfd, EPOLL_CTL_ADD, sockfd, &epEvent) < 0) {
        perror("Epoll_ctl(): ");
        return;
    }

    emit isRunning(QString::number(port), true);

    while(1) {
        // Wait for connections
        if((numfd = Epoll_wait(epfd, epEvents, E_POLL_QUEUE_LEN, -1)) < 0) {
            if(errno == EINTR) {
                continue;
            }
            perror("Epoll_wait(): ");
            emit isRunning(QString::number(port), false);
            return;
        }

        for(int it = 0; it < numfd; it++) {
            // Check for errors
            if(epEvents[it].events & (EPOLLHUP | EPOLLERR)) {
                qDebug() << "epoll:EPOLLERR";
                Close(epEvents[it].data.fd);
                emit isRunning(QString::number(port), false);
                return;
            }

            assert(epEvents[it].events & EPOLLIN);

            // Connection Request
            if(epEvents[it].data.fd == sockfd) {
                if((clifd = Accept(sockfd, (struct sockaddr *)&client, &cliLen)) < 0) {
                    if(errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("Accept(): ");
                        emit isRunning(QString::number(port), false);
                        return;
                    }
                    continue;
                }

                // Set to nonblocking
                if(Fcntl(clifd, F_SETFL) < 0) {
                    perror("Fcntl(): ");
                    emit isRunning(QString::number(port), false);
                    return;
                }

                epEvent.data.fd = clifd;

                // Add to clifd to epoll
                if(Epoll_ctl(clifd, EPOLL_CTL_ADD,epfd, &epEvent) < 0) {
                    perror("Epoll_ctl(): ");
                    emit isRunning(QString::number(port), false);
                    return;
                }

                thread = new QThread();
                thread->start();
                clientCon = new Client(clifd, inet_ntoa(client.sin_addr),
                                       target, port, ntohs(client.sin_port));
                if(clientCon->startTargetConnection() == -1) {
                    emit isRunning(QString::number(port), false);
                    return;
                }
                connect(this, SIGNAL(sendToInternal(int)), clientCon,
                        SLOT(sendToInternal(int)));
                clientCon->moveToThread(thread);
            }
            // Process Data
            emit sendToInternal(epEvents[it].data.fd);
        }
    }
}
