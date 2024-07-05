#include "LockFreeQueue.hpp"
#include <thread>
#include <iostream>
#include <functional>
#include <numeric>
#include "ThreadPool.h"
#include <mutex>
#include "spdlog/spdlog.h"
#include <hash_fun.h>
#include <pthread.h>    
#include <future>
#define SL std::cout << __LINE__ << std::endl;
std::mutex mtx;

void PrintThreadId()
{
        std::lock_guard<std::mutex> lock(mtx);
        printf("child thread id: %d\n", std::this_thread::get_id());
        fflush(stdout);
        return ;
}

int main()
{
    auto max = std::thread::hardware_concurrency();
    ThreadPool pool(max);
    std::thread poolThread(&ThreadPool::start, &pool);
    while (getchar() != 'q')
    {
        auto task = []()
        {
            PrintThreadId();
        };
        pool.addTask(task);
    }
    pool.willStop = true;
    poolThread.join();
    return 0;
}
