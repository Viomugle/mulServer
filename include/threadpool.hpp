#ifndef _THREADPOOL_HPP_
#define _THREADPOOL_HPP_

#include "scsp.hpp"
#include "worker.hpp"
#include <vector>
#include <new>
#include <functional>
#include <span>
using namespace std;

class threadpool
{
    private:
        worker* workers=nullptr;
        int capacity;
        int index=0;
    public:
        threadpool(int _n=8)
        {
            capacity=_n;
            workers=new worker[capacity];

        }
        void addTask(function<void()> task)
        {
            workers[index].addTask(task);
            index=(index+1)%capacity;
        }

        void close()
        {
            auto s=span<worker>(workers,capacity);
            for(auto& w:s)
            {
                w.close();
            }

        }
        ~threadpool()
        {
            close();
        }


};

#endif