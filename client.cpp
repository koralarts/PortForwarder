#include "client.h"
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <signal.h>
#include <QDebug>

Client::Client(int clifd, QString ip, QString target, int port, int cliPort) :
    clifd(clifd), ip(ip), target(target), port(port), cliPort(cliPort)
{
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

    // Connect to target machine
    if(Connect(sockfd, (struct sockaddr *)&server, sizeof(server)) < 0) {
        perror("Connect(): ");
        return -1;
    }

    return 0;
}

void Client::sendToInternal(int sockfd)
{
    char buff[BUFF_LEN];
    int n;
    int size;
    int hlen1;
    int tcplen;
    socklen_t len;
    socklen_t tempLen;
    struct sockaddr src;
    struct sockaddr dst;
    struct sockaddr_in tempAddr;
    struct ipheader *iphdr;
    struct tcpheader *tcphdr;

    if(clifd != sockfd) {
        return;
    }

    size = 60 * 1024;
    Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    // Read all the
    if((n = Recvfrom(sockfd, buff, sizeof(buff), 0, &src, &len)) < 0) {
        if(errno == EINTR) {
            return;
        } else {
            perror("Recvfrom(): ");
            return;
        }
    }

    if(n == 0) {
        close(this->sockfd);
        close(sockfd);
    }

    // Grab IP Header
    iphdr = (struct ipheader *)buff;
    hlen1 = iphdr->iph_len << 2;

    // Grab TCP Header
    tcphdr = (struct tcpheader *)(buff + hlen1);
    if((tcplen = len - hlen1) < 20) {
        qDebug() << "tcplen < 20: " << tcplen;
    }

    tempLen = sizeof(tempAddr);
    Getsockname(this->sockfd, (struct sockaddr*)&tempAddr, &tempLen);

    // Change Dest and Source IP
    iphdr->iph_destip = inet_addr(target.toLatin1().data());
    iphdr->iph_sourceip = tempAddr.sin_addr.s_addr;

    // Change src port
    tcphdr->tcph_srcport = tempAddr.sin_port;

    tempLen = tcplen + 20 + len;

    if(Sendto(this->sockfd, buff, size, 0, &dst, len) != tempLen) {
        perror("Send to(): ");
        return;
    }

    sendToExternal(sockfd);
}

void Client::sendToExternal(int sockfd)
{
    char buff[BUFF_LEN];
    int n;
    int size;
    int hlen1;
    int tcplen;
    socklen_t len;
    socklen_t tempLen;
    struct sockaddr src;
    struct sockaddr dst;
    struct sockaddr_in tempAddr;
    struct ipheader *iphdr;
    struct tcpheader *tcphdr;

    size = 60 * 1024;
    Setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &size, sizeof(size));

    // Read all the
    if((n = Recvfrom(this->sockfd, buff, sizeof(buff), 0, &src, &len)) < 0) {
        if(errno == EINTR) {
            return;
        } else {
            perror("Recvfrom(): ");
            return;
        }
    }

    if(n == 0) {
        close(this->sockfd);
        close(sockfd);
    }

    // Grab IP Header
    iphdr = (struct ipheader *)buff;
    hlen1 = iphdr->iph_len << 2;

    // Grab TCP Header
    tcphdr = (struct tcpheader *)(buff + hlen1);
    if((tcplen = len - hlen1) < 20) {
        qDebug() << "tcplen < 20: " << tcplen;
    }

    tempLen = sizeof(tempAddr);
    Getsockname(sockfd, (struct sockaddr*)&tempAddr, &tempLen);

    // Change Dest and Source IP
    iphdr->iph_destip = inet_addr(target.toLatin1().data());
    iphdr->iph_sourceip = tempAddr.sin_addr.s_addr;

    // Change src port
    tcphdr->tcph_srcport = tempAddr.sin_port;

    tempLen = tcplen + 20 + len;

    if(Sendto(sockfd, buff, size, 0, &dst, len) != tempLen) {
        perror("Send to(): ");
        return;
    }
}
