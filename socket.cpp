/***********************************************************
 * SOURCE FILE: socket.cpp
 *
 * PROGRAM: PortForwarder
 *
 * FUNCTIONS:
 * int Socket(int sock_fam, int sock_type, int proto);
 * int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
 * int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
 * int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
 * int Listen(int sockfd, int backlog);
 * int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
 * ssize_t Recv(int sockfd, void *buf, size_t len, int flags);
 * ssize_t Send(int sockfd, void *buf, size_t len, int flags);
 * int Close(int fd);
 *
 * DATE: MARCH 11, 2012
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * NOTES:
 * Contains wrapper functions for the Linux Socket API.
 ***********************************************************/

#include "socket.h"

/***********************************************************
 * FUNCTION: Socket()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Socket(int sock_fam, int sock_type, int proto)
 *			   sock_fam: the family of the socket that will be made (UDP, TCP)
 *             sock_type: (SOCK_STREAM or SOCK_DGRAM)
 *             proto: protocol (0 - TCP, 1 - UDP)
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the socket system call.
 ***********************************************************/
int Socket(int sock_fam, int sock_type, int proto)
{
    return socket(sock_fam, sock_type, proto);
}

/***********************************************************
 * FUNCTION: Setsockopt()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
 *             sockfd: socket descriptor
 *             level: level at where the option is
 *             optname: name of the option to be set
 *             optval: the option to be set
 *             optlen: the number of options to be set
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the setsockopt system call.
 ***********************************************************/
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen)
{
    return setsockopt(sockfd, level, optname, optval, optlen);
}

/***********************************************************
 * FUNCTION: Getsockopt()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
 *             sockfd: socket descriptor
 *             level: level at where the option is
 *             optname: name of the option to be set
 *             optval: the option to be set
 *             optlen: the number of options to be set
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the setsockopt system call.
 ***********************************************************/
int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen)
{
    return getsockopt(sockfd, level, optname, optval, optlen);
}

int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return bind(sockfd, addr, addrlen);
}

int Fcntl(int sockfd, int cmd)
{
    return fcntl(sockfd, cmd, O_NONBLOCK | fcntl(sockfd, F_GETFL, 0));
}

int Listen(int sockfd, int backlog)
{
    return listen(sockfd, backlog);
}

int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return accept(sockfd, addr, addrlen);
}

int Connect(int sockfd, struct sockaddr *addr, socklen_t addrlen)
{
    return connect(sockfd, addr, addrlen);
}

ssize_t Recv(int sockfd, void *buf, size_t len, int flags)
{
    return recv(sockfd, buf, len, flags);
}

ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src, socklen_t *addrlen)
{
    return recvfrom(sockfd, buf, len, flags, src, addrlen);
}

ssize_t Send(int sockfd, void *buf, size_t len, int flags)
{
    return send(sockfd, buf, len, flags);
}

ssize_t Sendto(int sockfd, void *buf, size_t len, int flags,
               struct sockaddr *dest, socklen_t addrlen)
{
    return sendto(sockfd, buf, len, flags, dest, addrlen);
}

int Close(int fd)
{
    return close(fd);
}

struct hostent* Gethostbyname(char *host)
{
    return gethostbyname(host);
}

void Getsockname(int sockfd, sockaddr *addr, socklen_t *len)
{
   getsockname(sockfd, addr, len);
}

/*
 * http://www.tenouk.com/Module43a.html
 */

unsigned short checksum(unsigned short *buf, int nwords)
{
    unsigned long sum;
    for(sum=0; nwords>0; nwords--)
            sum += *buf++;
    sum = (sum >> 16) + (sum &0xffff);
    sum += (sum >> 16);
    return (unsigned short)(~sum);
}
