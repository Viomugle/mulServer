#ifndef THREADPOOL_H
#define THREADPOOL_H
#include <functional>
#include "LockFreeQueue.hpp"
#include <vector>
#include <thread>
#include <cstdlib>
#include <atomic>
#include <iostream>


class ThreadPool
{
    private:
        lfRingBuffer<std::function<void()>>* pool;
        std::atomic<bool> flag = false;
        std::vector<std::thread> threads;
        int size;
    public:
        ThreadPool(int n = 8){
            pool = new lfRingBuffer<std::function<void()>>(n);
            size = n;
            threads.reserve(n);
            for(int i=0;i<n;i++){
                threads.emplace_back([this](){
                    std::function<void()> t;
                    while (flag.load() == false)
                    {
                        if (pool->front(t) == true)
                        {
                            t();
                        }
                        else
                        {
                            //std::this_thread::yield();
                        }
                    }
                    cout<<"thread: "<<this_thread::get_id()<<"  exit"<<endl;
                });
            }
        } 

        void close()
        {
            flag.store(true);
            for(auto &t:threads){
                if(t.joinable())
                t.join();
            }
        }

        ~ThreadPool(){
            close();
            delete pool;
            #ifdef NDEBUG
            printf("threadpool deleted\n");
            #endif
        }

        void addTask(std::function<void()> t){
            while (pool->back(t) != true)
            {
                std::this_thread::yield();
            }
        }

};

#endif