#include "Socket.h"
#include "tools.h"
#include <unistd.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <iostream>
#include <thread>

Socket::Socket()
{
    fd = socket(AF_INET, SOCK_STREAM, 0);
    tools::assert(fd == -1, "socket create error");
}
Socket::Socket(int _fd) : fd(_fd)
{
    tools::assert(fd == -1, "socket create error");
}

Socket::~Socket()
{
    printf("socket des invoke\n");
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
}

void Socket::bind(InetAddress *_addr)
{
    struct sockaddr_in addr = _addr->getAddr();
    tools::assert(::bind(fd, (sockaddr *)&addr, sizeof(addr)) == -1, "socket bind error");
}

void Socket::listen()
{
    tools::assert(::listen(fd, SOMAXCONN) == -1, "socket listen error");
}
void Socket::setnonblocking()
{
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}

int Socket::accept(InetAddress *_addr)
{
    // for server socket
    int clnt_sockfd = -1;
    struct sockaddr_in addr;
    bzero(&addr, sizeof(addr));
    socklen_t addr_len = sizeof(addr);
    if (fcntl(fd, F_GETFL) & O_NONBLOCK)
    {
        while (true)
        {
            clnt_sockfd = ::accept(fd, (sockaddr *)&addr, &addr_len);
            if (clnt_sockfd == -1 && ((errno == EAGAIN) || (errno == EWOULDBLOCK)))
            {
                // printf("no connection yet\n");
                continue;
            }
            else if (clnt_sockfd == -1)
            {
                tools::assert(true, "socket accept error");
            }
            else
            {
                break;
            }
        }
    }
    else
    {
        clnt_sockfd = ::accept(fd, (sockaddr *)&addr, &addr_len);
        tools::assert(clnt_sockfd == -1, "socket accept error");
    }
    _addr->setInetAddr(addr);
    return clnt_sockfd;
}

void Socket::connect(InetAddress *_addr)
{
    // for client socket
    struct sockaddr_in addr = _addr->getAddr();
    if (fcntl(fd, F_GETFL) & O_NONBLOCK)
    {
        while (true)
        {
            int ret = ::connect(fd, (sockaddr *)&addr, sizeof(addr));
            if (ret == 0)
            {
                break;
            }
            else if (ret == -1 && (errno == EINPROGRESS))
            {
                continue;
            }
            else if (ret == -1)
            {
                tools::assert(true, "socket connect error");
            }
        }
    }
    else
    {
        tools::assert(::connect(fd, (sockaddr *)&addr, sizeof(addr)) == -1, "socket connect error");
    }
}

int Socket::getFd()
{
    return fd;
}

InetAddress::InetAddress()
{
    bzero(&addr, sizeof(addr));
}
InetAddress::InetAddress(const char *_ip, uint16_t _port)
{
    bzero(&addr, sizeof(addr));
    addr.sin_family = AF_INET;
    addr.sin_addr.s_addr = inet_addr(_ip);
    addr.sin_port = htons(_port);
}

InetAddress::~InetAddress()
{
}

void InetAddress::setInetAddr(sockaddr_in _addr)
{
    addr = _addr;
}

sockaddr_in InetAddress::getAddr()
{
    return addr;
}

char *InetAddress::getIp()
{
    return inet_ntoa(addr.sin_addr);
}

uint16_t InetAddress::getPort()
{
    return ntohs(addr.sin_port);
}

void Socket::closeEvent()
{
    epollDel();
    printf("close triggered\n");
    if (fd != -1)
    {
        close(fd);
        fd = -1;
    }
    fd = -1;
    delete this;
}

void Socket::readEvent()
{
    char mess[1024];
    while (true)
    {
        memset(mess, '\0', 1024);
        int n = read(fd, mess, 1024);
        if (n == 0)
        {
            closeEvent();
            break;
        }
        else if (n > 0)
        {
            _buf.append(std::string_view(mess));
        }
        else if (n == -1)
        {
            if (errno == EAGAIN)
            {
                break;
            }
            else if (errno == EINTR)
            {
                break;
            }
            else if (errno == EBADF)
            {
                closeEvent();
                break;
            }
        }
    }
    printf("Socket read finished!\n");
}