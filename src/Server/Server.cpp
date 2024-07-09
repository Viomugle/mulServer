#include <iostream>
#include "Socket.h"
#include "tools.h"
#include <unistd.h>
#include "Epoll.h"
#include <iostream>
#include <map>
#include "ReadBuffer.h"
#include <functional>
#include "ThreadPool.h"
#include <format>
#include <unordered_map>

std::unordered_map<int, Socket *> clients;

int main(int argc, char **argv)
{

    auto max = std::thread::hardware_concurrency();
    ThreadPool pool(max);
    std::thread poolThread(&ThreadPool::start, &pool);

    auto sock = new Socket();
    auto addr = new InetAddress("127.0.0.1", 8000);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    Epoll ep;
    epoll_event ev;
    ev.data.fd = sock->getFd();
    ev.events = EPOLLIN | EPOLLET;
    epoll_ctl(ep.epfd, EPOLL_CTL_ADD, sock->getFd(), &ev);
    printf("epfd is: %d\n",ep.epfd);
    while (true)
    {
        auto revents = ep.getRevents();
        for (auto &revent : revents)
        {
            if (revent.first == sock->getFd())
            {

                auto clnt_fd = sock->accept(addr);
                printf("new connection in fd: %d\n", clnt_fd);
                auto clnt_sock = new Socket(clnt_fd);
                clnt_sock->setnonblocking();
                clients[clnt_fd] = clnt_sock;
                ep.addFd(clnt_fd);
                std::function<void()> closeCb = [&ep, clnt_fd]()
                {
                    ep.removeFd(clnt_fd);
                    auto ptr=clients[clnt_fd];
                    //delete clients[clnt_fd];
                    clients.erase(clnt_fd);
                    //delete ptr;
                    printf("clnt %d close\n", clnt_fd);
                };
                clnt_sock->epollDel = closeCb;
            }
            else
            {
                printf("revents is:%d\n",revent.second);
                if (revent.second & EPOLLHUP)
                {
                    printf("HangUp triggered!");
                }
                else if (revent.second & EPOLLERR)
                {
                    printf("Err triggered!");
                }
                else
                {
                    auto clnt_sock = clients[revent.first];
                    std::function<void()> cb=std::bind(&Socket::readEvent,clnt_sock);
                    pool.addTask(cb);
                }
            }
        }
    }

    return 0;
}
