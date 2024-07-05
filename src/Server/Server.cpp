#include <iostream>
#include "Socket.h"
#include "tools.h"
#include <unistd.h>
#include "Epoll.h"
#include <iostream>
using namespace std;


int main(int argc, char** argv)
{
    auto sock=new Socket();
    auto addr=new InetAddress("127.0.0.1",8000);
    sock->bind(addr);
    sock->listen();
    sock->setnonblocking();
    Epoll ep;
    epoll_event ev;
    ev.data.fd=sock->getFd();
    ev.events=EPOLLIN|EPOLLET;
    epoll_ctl(ep.epfd,EPOLL_CTL_ADD,sock->getFd(),&ev);
    while(true){
        auto revents=ep.getRevents();
        cout<<"get "<<revents.size()<<" events"<<endl;
        for(auto revent:revents)
        {
            if(revent.first==sock->getFd())
            {
                auto clnt_sockfd=sock->accept(addr);
                cout<<"accept a connection"<<endl;
                ep.addFd(clnt_sockfd);
            }
            else
            {
                char buf[1024];
                int n=read(revent.first,buf,1024);
                if(n==0)
                {
                    cout<<"connection closed"<<endl;
                    ep.removeFd(revent.first);
                    close(revent.first);
                }
                else
                {
                    cout<<"read "<<n<<" bytes form fd: "<<revent.first<<endl;
                    cout<<buf<<endl;
                    //write(revent.first,buf,n);
                }
            }
        }
    }
    cout << "Finish" << endl;
    return 0;
}
