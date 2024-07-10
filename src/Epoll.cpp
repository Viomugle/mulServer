#include "Epoll.h"
#include "tools.h"
#include <unistd.h>
#include <vector>
Epoll::Epoll()
{
    epfd = epoll_create(1);
    tools::assert(epfd == -1, "epoll creat error");
}

Epoll::~Epoll()
{
    if (epfd != -1)
    {
        ::close(epfd);
        epfd = -1;
    }
}

void Epoll::addFd(int _fd)
{
    struct epoll_event ev;
    ev.events = EPOLLET | EPOLLIN | EPOLLERR | EPOLLHUP;
    ev.data.fd = _fd;
    epoll_ctl(epfd, EPOLL_CTL_ADD, _fd, &ev);
}

void Epoll::removeFd(int _fd)
{
    auto res = epoll_ctl(epfd, EPOLL_CTL_DEL, _fd, 0);
}

std::vector<std::pair<int, uint32_t>> Epoll::getRevents()
{
    std::vector<std::pair<int, uint32_t>> revents;
    struct epoll_event events[MAX_EVENTS];
    int n = epoll_wait(epfd, events, MAX_EVENTS, -1);
    for (int i = 0; i < n; i++)
    {
        auto fd = events[i].data.fd;
        auto revent = events[i].events;
        revents.push_back(std::make_pair(fd, revent));
    }
    return revents;
}