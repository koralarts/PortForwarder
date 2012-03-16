#include "epoll.h"

int Epoll_create(int queue)
{
    return epoll_create(queue);
}

int Epoll_ctl(int epfd, int op, int sockfd, struct epoll_event *event)
{
    return epoll_ctl(epfd, op, sockfd, event);
}

int Epoll_wait(int epollfd, struct epoll_event *event, int queue, int timeout)
{
    return epoll_wait(epollfd, event, queue, timeout);
}
