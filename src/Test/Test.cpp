#include "LockFreeQueue.hpp"
#include <thread>
#include <iostream>
#include <functional>
#include <numeric>
#include "ThreadPool.h"
#include <mutex>
#include <hash_fun.h>
#include <pthread.h>    
#include <future>
#include "ReadBuffer.h"
#include "HTTP.h"
#include <ranges>

#include <unordered_map>


using namespace std;
string mess="GET /index.html HTTP/1.1\r\nHost: www.example.com\r\nUser-Agent: Mozilla/5.0 (Windows NT 10.0; Win64; x64; rv:91.0) Gecko/20100101 Firefox/91.0\r\nAccept: text/html,application/xhtml+xml,application/xml;q=0.9,image/webp,*/*;q=0.8\r\nAccept-Encoding: gzip, deflate\r\nConnection: keep-alive\r\n\r\n";
int main()
{
    ReadBuffer rb(mess);
    rb.append("/*/*append at end/*/*");
    HTTP http;
    auto res=http.parseHTTP(rb);
    if(res.has_value())
    {
        for(auto &i:*res)
        {
            cout<<i.first<<":    :"<<i.second<<"/-/"<<endl;
        }
    }
    else
    {
        cout<<"parse failed"<<endl;
    }
    return 0;
}