#pragma once
#include <sys/epoll.h>
#include <vector>
#include <unistd.h>
#include <errno.h>
#define MAX_EVENTS 10000
class Epoll
{
    public:
        Epoll();
        int epfd;
        void addFd(int _fd);
        void removeFd(int _fd);
        ~Epoll();
        std::vector<std::pair<int,uint32_t>> getRevents();
};