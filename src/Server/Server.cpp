#include <iostream>
#include "Socket.h"
#include "tools.h"
#include <unistd.h>
#include "Epoll.h"
#include <iostream>
#include <map>
#include "ReadBuffer.h"
#include <functional>
#include "threadpool.hpp"
#include <format>
#include <unordered_map>

std::unordered_map<int, Socket *> clients;

int main(int argc, char **argv)
{

    auto max = std::thread::hardware_concurrency();
    threadpool pool(max);

    auto sock = new Socket();
    auto addr = new InetAddress("127.0.0.1", 8000);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    Epoll ep;
    epoll_event ev;
    ev.data.fd = sock->getFd();
    ev.events = EPOLLET|EPOLLIN|EPOLLERR|EPOLLHUP|EPOLLRDHUP;
    epoll_ctl(ep.epfd, EPOLL_CTL_ADD, sock->getFd(), &ev);
    while (true)
    {
        auto revents = ep.getRevents();
        for (auto &revent : revents)
        {
            if (revent.first == sock->getFd())
            {
                cout<<"new connection"<<endl;
                auto clnt_fd = sock->accept(addr);
                auto clnt_sock = new Socket(clnt_fd);
                clnt_sock->setnonblocking();
                clients[clnt_fd] = clnt_sock;
                ep.addFd(clnt_fd);
                std::function<void()> closeCb = [&ep, clnt_fd]()
                {
                    ep.removeFd(clnt_fd);
                    auto ptr=clients[clnt_fd];
                    clients.erase(clnt_fd);
                };
                clnt_sock->epollDel = closeCb;
            }
            else
            {
                if(revent.second & EPOLLIN)
                {
                    auto clnt_sock = clients[revent.first];
                    std::function<void()> cb=std::bind(&Socket::readEvent,clnt_sock);
                    pool.addTask(cb);
                }
            }
        }
    }
    for(auto client :clients)
    {
        client.second->closeEvent();
    }

    pool.close();
    return 0;
}
