/***********************************************************
 * SOURCE FILE: client.cpp
 *
 * PROGRAM: PortForwarder
 *
 * FUNCTIONS:
 * Client(int clifd, QString target, int port);
 * ~Client();
 * int startTargetConnection();
 * void sendToInternal(char *buff, int len);
 * void sendToExternal(int sockfd);
 *
 * DATE: MARCH 11, 2012
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * NOTES:
 * The Client class is where messages from the client will be
 * sent to the target machine, and this is also where
 * messages from the target machine will be sent to the client.
 ***********************************************************/

#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <QDebug>

/***********************************************************
 * FUNCTION: Client()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: Client(int clifd, QString target, int port);
 *			   clifd: the client's socket descriptor
 *             target: the target IP address
 *             port: the port of the service
 *
 * RETURN: void
 *
 * NOTES:
 * The Client class constructor.
 ***********************************************************/
Client::Client(int clifd, QString target, int port) :
    clifd(clifd), target(target), port(port)
{
}

/***********************************************************
 * FUNCTION: ~Client()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: ~Client()
 *
 * RETURN: void
 *
 * NOTES:
 * The Client class destructor.
 ***********************************************************/
Client::~Client()
{
    close(clifd);
}

/***********************************************************
 * FUNCTION: startTargetConnection()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int startTargetConnection()
 *
 * RETURN: int
 *          -1: failure
 *
 * NOTES:
 * This function initializes a connection to the target machine.
 ***********************************************************/
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

/***********************************************************
 * FUNCTION: sendToInternal()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void sendToInternal(char *buff, int len)
 *                 buff: data that will be sent to the internal machine
 *                 len: length of the data that will be sent to the internal
 *                      machine
 *
 * RETURN: int
 *          -1: failure
 *
 * NOTES:
 * This function sends data to the target machine from the
 * external client.
 ***********************************************************/
void Client::sendToInternal(char *buff, int len)
{
    // Write all of it to the internal machine
    write(this->sockfd, buff, len);
}

/***********************************************************
 * FUNCTION: sendToExternal()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void sendToExternal(int sockfd)
 *                 sockfd: the client's socket descriptor
 *
 * RETURN: int
 *          -1: failure
 *
 * NOTES:
 * This function sends data from the target machine to the
 * external client.
 ***********************************************************/
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
