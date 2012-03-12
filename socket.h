#ifndef SOCKET_H
#define SOCKET_H

#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <fcntl.h>
#include <string.h>

int Socket(int sock_fam, int sock_type, int proto);
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
ssize_t Recv(int sockfd, void *buf, size_t len, int flags);
ssize_t Send(int sockfd, void *buf, size_t len, int flags);
int Close(int fd);

#endif // SOCKET_H
