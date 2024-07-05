#include "ThreadPool.h"
#include <stdlib.h>
#include <stdio.h>
#include <future>

ThreadPool::ThreadPool(int _size)
{
    workQueue=new TaskQueue(_size);
    tools::assert(workQueue==nullptr,"workQueue create error");
}

ThreadPool::~ThreadPool()
{
    delete workQueue;
}

void ThreadPool::addTask(Task task)
{
    while(workQueue->push(task)!=true);
}

void ThreadPool::stop()
{
    while(!workQueue->isEmpty())
    {
        auto task=workQueue->pop();
        std::thread taskThread(task);
//        taskThread.join();
    }
}

void ThreadPool::start()
{
    printf("thread pool main loop at thread: %d\n",std::this_thread::get_id());
    while(willStop==false)
    {
        if(workQueue->isEmpty())
        {
            continue;
        }
        auto task=workQueue->pop();
        std::thread taskThread(task);
        taskThread.detach();
    }
    stop();
    printf("kill thread pool\n");
    fflush(stdout);
}