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
 * int Fcntl(int sockfd, int cmd);
 * int Listen(int sockfd, int backlog);
 * int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
 * int Connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);
 * ssize_t Recv(int sockfd, void *buf, size_t len, int flags);
 * ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
 *                struct sockaddr *src, socklen_t *addrlen);
 * ssize_t Send(int sockfd, void *buf, size_t len, int flags);
 * ssize_t Sendto(int sockfd, void *buf, size_t len, int flags,
 *               struct sockaddr *dest, socklen_t addrlen);
 * int Close(int fd);
 * struct hostent* Gethostbyname(char *host);
 * void Getsockname(int sockfd, sockaddr *addr, socklen_t *len);
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

/***********************************************************
 * FUNCTION: Bind()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
 *             sockfd: socket descriptor
 *             addr: sockaddr structure containing the server information
 *             addlen: length of the sockaddr; usually sizeof(sockaddr)
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the bind system call.
 ***********************************************************/
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen)
{
    return bind(sockfd, addr, addrlen);
}

/***********************************************************
 * FUNCTION: Fcntl()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Fcntl(int sockfd, int cmd)
 *             sockfd: socket descriptor
 *             cmd: the command that will be initiated
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the fcntl system call to a set a socket to non-blocking.
 ***********************************************************/
int Fcntl(int sockfd, int cmd)
{
    return fcntl(sockfd, cmd, O_NONBLOCK | fcntl(sockfd, F_GETFL, 0));
}

/***********************************************************
 * FUNCTION: Listen()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Listen(int sockfd, int backlog)
 *             sockfd: socket descriptor
 *             backlog: number of enqueued connection
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the listen system call.
 ***********************************************************/
int Listen(int sockfd, int backlog)
{
    return listen(sockfd, backlog);
}

/***********************************************************
 * FUNCTION: Listen()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
 *             sockfd: socket descriptor
 *             backlog: number of enqueued connection
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the accept system call.
 ***********************************************************/
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen)
{
    return accept(sockfd, addr, addrlen);
}

/***********************************************************
 * FUNCTION: Connect()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Connect(int sockfd, struct sockaddr *addr, socklen_t addrlen)
 *             sockfd: socket descriptor
 *             addr: sockaddr structure that will hold the client host data
 *             addrlen: size of the sockaddr structure usually sizeof(sockaddr)
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the connect system call.
 ***********************************************************/
int Connect(int sockfd, struct sockaddr *addr, socklen_t addrlen)
{
    return connect(sockfd, addr, addrlen);
}

/***********************************************************
 * FUNCTION: Recv()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: ssize_t Recv(int sockfd, void *buf, size_t len, int flags)
 *             sockfd: socket descriptor
 *             buf: the buffer that will contain the received data
 *             len: the maximum size that the buffer can hold
 *             flags: options during receiving
 *
 * RETURN: ssize_t
 * 		-1: failure
 *      0: connection closed
 *
 * NOTES:
 * A wrapper function for the recv system call.
 ***********************************************************/
ssize_t Recv(int sockfd, void *buf, size_t len, int flags)
{
    return recv(sockfd, buf, len, flags);
}

/***********************************************************
 * FUNCTION: Recvfrom()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
 *               struct sockaddr *src, socklen_t *addrlen)
 *             sockfd: socket descriptor
 *             buf: the buffer that will contain the received data
 *             len: the maximum size that the buffer can hold
 *             flags: options during receiving
 *             src: the sockaddr struct that contains information about where the
 *                  data will be coming from
 *             addrlen: size of the sockaddr structure usually sizeof(sockaddr)
 *
 * RETURN: ssize_t
 * 		-1: failure
 *      0: connection closed
 *
 * NOTES:
 * A wrapper function for the recvfrom system call.
 ***********************************************************/
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src, socklen_t *addrlen)
{
    return recvfrom(sockfd, buf, len, flags, src, addrlen);
}

/***********************************************************
 * FUNCTION: Send()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: ssize_t Send(int sockfd, void *buf, size_t len, int flags)
 *             sockfd: socket descriptor
 *             buf: the buffer that contains the data to be sent
 *             len: the size of the data in the buffer
 *             flags: options during receiving
 *
 * RETURN: ssize_t
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the send system call.
 ***********************************************************/
ssize_t Send(int sockfd, void *buf, size_t len, int flags)
{
    return send(sockfd, buf, len, flags);
}

/***********************************************************
 * FUNCTION: Sendto()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: ssize_t Sendto(int sockfd, void *buf, size_t len, int flags,
 *               struct sockaddr *src, socklen_t *addrlen)
 *             sockfd: socket descriptor
 *             buf: the buffer that will contain the received data
 *             len: the maximum size that the buffer can hold
 *             flags: options during receiving
 *             src: the sockaddr struct that contains information about where the
 *                  data will be sent to
 *             addrlen: size of the sockaddr structure usually sizeof(sockaddr)
 *
 * RETURN: ssize_t
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the sendto system call.
 ***********************************************************/
ssize_t Sendto(int sockfd, void *buf, size_t len, int flags,
               struct sockaddr *dest, socklen_t addrlen)
{
    return sendto(sockfd, buf, len, flags, dest, addrlen);
}

/***********************************************************
 * FUNCTION: Close()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Close(int fd)
 *             fd: socket descriptor to be closed
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the close system call.
 ***********************************************************/
int Close(int fd)
{
    return close(fd);
}

/***********************************************************
 * FUNCTION: Gethostbyname()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: struct hostent* Gethostbyname(char *host)
 *             host: the IP address
 *
 * RETURN: struct hostent*
 * 		null: failure
 *
 * NOTES:
 * A wrapper function for the gethostbyname system call.
 ***********************************************************/
struct hostent* Gethostbyname(char *host)
{
    return gethostbyname(host);
}

/***********************************************************
 * FUNCTION: Getsockname()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: void Getsockname(int sockfd, sockaddr *addr, socklen_t *len)
 *             sockfd: socket descriptor
 *             addr: sockaddr structure where the ip information will be stored
 *             len: length of the sockaddr structure usually sizeof(sockaddr)
 *
 * RETURN: void
 *
 * NOTES:
 * A wrapper function for the getsockname system call.
 ***********************************************************/
void Getsockname(int sockfd, sockaddr *addr, socklen_t *len)
{
   getsockname(sockfd, addr, len);
}
