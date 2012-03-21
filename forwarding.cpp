/***********************************************************
 * SOURCE FILE: fowrarding.cpp
 *
 * PROGRAM: PortForwarder
 *
 * FUNCTIONS:
 * Forwarding(int port, int listenPort, QString target);
 * ~Forwarding();
 * void startListening(int checkPort)
 *
 * DATE: MARCH 11, 2012
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * NOTES:
 * The Forwarding class is the class that will listen for
 * connections from clients and will do the forwarding for a
 * specific service.
 ***********************************************************/
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

/***********************************************************
 * FUNCTION: Forwarding()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: Forwarding(int port, int listenPort, QString target)
 *			   port: the service port
 *             listenPort: the listening port
 *             target: target ip address
 *
 * RETURN: void
 *
 * NOTES:
 * The Forwarding class constructor.
 ***********************************************************/
Forwarding::Forwarding(int port, int listenPort, QString target) :
    port(port), listenPort(listenPort), target(target)
{
}

/***********************************************************
 * FUNCTION: ~Forwarding()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: ~Forwarding()
 *
 * RETURN: void
 *
 * NOTES:
 * The Forwarding class destructor.
 ***********************************************************/
Forwarding::~Forwarding()
{
    close(sockfd);
}

/***********************************************************
 * FUNCTION: startListening()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void startListening(int checkPort)
 *                 checkPort: the service that will be forwarded
 *
 * RETURN: int
 *          -1: failure
 *
 * NOTES:
 * This function acts as an ePoll server that will receive
 * connection requests from external machines(clients)
 ***********************************************************/
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
    int n;
    char buff[BUFF_LEN];

    QThread *thread;
    Client *clientCon;
    QMap <int, Client*> clientMap;

    if(checkPort != port) {
        return;
    }

    qDebug() << "Starting listening on port: " << listenPort;

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
    server.sin_port = htons(listenPort);
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
            if(errno != EINTR) {
                perror("Epoll_wait(): ");
                emit isRunning(QString::number(port), false);
                return;
            }
            continue;
        }

        for(int it = 0; it < numfd; it++) {
            // Check for errors
            if(epEvents[it].events & (EPOLLHUP | EPOLLERR)) {
                qDebug() << "epoll:EPOLLERR";
                Close(epEvents[it].data.fd);
                //emit isRunning(QString::number(port), false);
                return;
            }

            assert(epEvents[it].events & EPOLLIN);

            // Connection Request
            if(epEvents[it].data.fd == sockfd) {
                if((clifd = Accept(sockfd, (struct sockaddr *)&client, &cliLen)) < 0) {
                    if(errno != EAGAIN && errno != EWOULDBLOCK) {
                        perror("Accept(): ");
                        //emit isRunning(QString::number(port), false);
                        return;
                    }
                    continue;
                }

                qDebug() << "Connected: " << inet_ntoa(client.sin_addr);

                // Set to nonblocking
                if(Fcntl(clifd, F_SETFL) < 0) {
                    perror("Fcntl(): ");
                    //emit isRunning(QString::number(port), false);
                    return;
                }

                epEvent.data.fd = clifd;

                // Add to clifd to epoll
                if(Epoll_ctl(epfd, EPOLL_CTL_ADD, clifd, &epEvent) < 0) {
                    perror("Epoll_ctl(): ");
                    //emit isRunning(QString::number(port), false);
                    return;
                }

                thread = new QThread();
                thread->start();
                clientCon = new Client(clifd, target, port);
                clientMap.insert(clifd, clientCon);
                if(clientCon->startTargetConnection() == -1) {
                    emit isRunning(QString::number(port), false);
                    return;
                }
                connect(this, SIGNAL(sendToExternal(int)), clientCon,
                        SLOT(sendToExternal(int)));
                clientCon->moveToThread(thread);
                emit sendToExternal(clifd);

                continue;
            }

            clifd = epEvents[it].data.fd;
            // Process Data
            if((n = read(clifd, buff, BUFF_LEN)) < 0) {
                if(errno == EINTR) {
                    return;
                } else {
                    perror("Read(): ");
                    return;
                }
            }
            if(n == 0) {
                close(clifd);
                delete clientMap[clifd];
                clientMap.remove(clifd);
                continue;
            }
            clientMap[clifd]->sendToInternal(buff, n);
        }
    }
}
