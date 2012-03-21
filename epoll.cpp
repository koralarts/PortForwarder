/***********************************************************
 * SOURCE FILE: epoll.cpp
 *
 * PROGRAM: PortForwarder
 *
 * FUNCTIONS:
 * int Epoll_create(int queue);
 * int Epoll_ctl(int epfd, int op, int sockfd, struct epoll_event *event);
 * int Epoll_wait(int epollfd, struct epoll_event *event, int queue, int timeout);
 *
 * DATE: MARCH 11, 2012
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * NOTES:
 * Contains wrapper functions for the Linux ePoll API.
 ***********************************************************/

#include "epoll.h"

/***********************************************************
 * FUNCTION: Epoll_create()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Epoll_create(int queue)
 *			   queue: the maximum number of connections ePoll will handle
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the epoll_create system call.
 ***********************************************************/
int Epoll_create(int queue)
{
    return epoll_create(queue);
}

/***********************************************************
 * FUNCTION: Epoll_ctl()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Epoll_ctl(int epfd, int op, int sockfd, struct epoll_event *event)
 *			   epfd: the epoll file descriptor
 *             op: the task you want epoll_ctl to do
 *             sockfd: the data that will be used to do the specified task
 *             event: the epoll event structure that will hold the events for
 *                      that socket descriptor (ie. error, data, etc)
 *
 * RETURN: int
 * 		-1: failure
 *
 * NOTES:
 * A wrapper function for the epoll_ctl system call.
 ***********************************************************/
int Epoll_ctl(int epfd, int op, int sockfd, struct epoll_event *event)
{
    return epoll_ctl(epfd, op, sockfd, event);
}

/***********************************************************
 * FUNCTION: Epoll_wait()
 *
 * DATE: March 11, 2012
 *
 * REVISIONS: (Date and Description)
 *
 * DESIGNER: Karl Castillo
 *
 * PROGRAMMER: Karl Castillo
 *
 * INTERFACE: int Epoll_wait(int epollfd, struct epoll_event *event, int queue, int timeout)
 *			   epollfd: the epoll file descriptor
 *             event: array of epoll events where the sockets that need
 *                      attention will be stored
 *             queue: the maximum number of possible events to be returned
 *             timeout: the amount of time to wait before returning (-1 if
 *                      indefinite)
 *
 * RETURN: int
 * 		-1: failure
 *      n: number of sockets that need processing
 *
 * NOTES:
 * A wrapper function for the epoll_wait system call.
 ***********************************************************/
int Epoll_wait(int epollfd, struct epoll_event *event, int queue, int timeout)
{
    return epoll_wait(epollfd, event, queue, timeout);
}
