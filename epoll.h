#ifndef EPOLL_H
#define EPOLL_H

#include <sys/epoll.h>

#define E_POLL_QUEUE_LEN 65535

int Epoll_create(int queue);
int Epoll_ctl(int epfd, int op, int sockfd, struct epoll_event *event);
int Epoll_wait(int epollfd, struct epoll_event *event, int queue, int timeout);

#endif // EPOLL_H
