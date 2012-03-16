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
#include <unistd.h>
#include <fcntl.h>

#define LISTEN_QUEUE    5
#define PACKET_LEN      8192
#define BUFF_LEN        65535

int Socket(int sock_fam, int sock_type, int proto);
int Setsockopt(int sockfd, int level, int optname, const void *optval, socklen_t optlen);
int Getsockopt(int sockfd, int level, int optname, void *optval, socklen_t *optlen);
int Bind(int sockfd, const struct sockaddr *addr, socklen_t addrlen);
int Fcntl(int sockfd, int cmd);
int Listen(int sockfd, int backlog);
int Accept(int sockfd, struct sockaddr *addr, socklen_t *addrlen);
int Connect(int sockfd, struct sockaddr *addr, socklen_t addrlen);
ssize_t Recv(int sockfd, void *buf, size_t len, int flags);
ssize_t Recvfrom(int sockfd, void *buf, size_t len, int flags,
                 struct sockaddr *src, socklen_t *addrlen);
ssize_t Send(int sockfd, void *buf, size_t len, int flags);
ssize_t Sendto(int sockfd, void *buf, size_t len, int flags,
               struct sockaddr *dest, socklen_t addrlen);
int Close(int fd);
struct hostent* Gethostbyname(char *host);
void Getsockname(int sockfd, sockaddr *addr, socklen_t *len);
unsigned short checksum(unsigned short *buf, int nwords);

struct ipheader {
    unsigned char      iph_ihl:5, iph_ver:4;
    unsigned char      iph_tos;
    unsigned short int iph_len;
    unsigned short int iph_ident;
    unsigned char      iph_flag;
    unsigned short int iph_offset;
    unsigned char      iph_ttl;
    unsigned char      iph_protocol;
    unsigned short int iph_chksum;
    unsigned int       iph_sourceip;
    unsigned int       iph_destip;
};

struct tcpheader {
    unsigned short int   tcph_srcport;
    unsigned short int   tcph_destport;
    unsigned int         tcph_seqnum;
    unsigned int         tcph_acknum;
    unsigned char        tcph_reserved:4, tcph_offset:4;
    unsigned int
        tcp_res1:4,       /*little-endian*/
        tcph_hlen:4,      /*length of tcp header in 32-bit words*/
        tcph_fin:1,       /*Finish flag "fin"*/
        tcph_syn:1,       /*Synchronize sequence numbers to start a connection*/
        tcph_rst:1,       /*Reset flag */
        tcph_psh:1,       /*Push, sends data to the application*/
        tcph_ack:1,       /*acknowledge*/
        tcph_urg:1,       /*urgent pointer*/
        tcph_res2:2;
    unsigned short int   tcph_win;
    unsigned short int   tcph_chksum;
    unsigned short int   tcph_urgptr;
};

#endif // SOCKET_H
