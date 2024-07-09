#pragma once
#include <cstddef>
#include <arpa/inet.h>
#include <ReadBuffer.h>
#include <functional>
class InetAddress
{
private:
    struct sockaddr_in addr;

public:
    InetAddress();
    InetAddress(const char *_ip, uint16_t _port);
    ~InetAddress();

    void setInetAddr(sockaddr_in _addr);
    sockaddr_in getAddr();
    char *getIp();
    uint16_t getPort();
};

class Socket
{
private:
    int fd;
    ReadBuffer _buf;

public:
    Socket();
    Socket(int _fd);
    ~Socket();
    void bind(InetAddress *);
    void listen();
    int accept(InetAddress *);
    std::function<void()> epollDel;
    void connect(InetAddress *);

    void setnonblocking();
    int getFd();
    void readEvent();
    void closeEvent();
    bool operator<(const Socket _rhs);
};
