#ifndef _WORKER_HPP_
#define _WORKER_HPP_
#include "scsp.hpp"
#include <thread>
#include <cstdlib>
#include <functional>
#include <iostream>
using namespace std;



class worker
{
public:
    scsp<function<void(void)>> q;
    thread t;
    atomic<bool> stop = false;
    void loop()
    {
        while(stop.load()==false)
        {
            function<void()> task;
            if(q.front(ref(task)))
            {
                task();
            }
            else
            {
                this_thread::yield();
            }
        }
        while(!q.empty())
        {
            function<void()> task;
            if(q.front(ref(task)))
            {
                task();
            }
            else
            {
                this_thread::yield();
            }
        }
    }
    worker()
    {
        t=thread(&worker::loop,this);
    }

    void close()
    {
        stop.store(true);
        if(t.joinable())
        t.join();
    }

    bool addTask(function<void()> task)
    {
        if (stop.load() == true)
        {
            cout << "worker is closed" << endl;
            return false;
        }
        while (!q.back(task))
        {
            this_thread::yield();
        }
        return true;
    }

    ~worker()
    {
        close();
    }
};

#endif