#include "LockFreeQueue.hpp"
#include <thread>
#include <functional>
#include "tools.h"
class ThreadPool{
    using Task=std::function<void()>;
    using TaskQueue=LockFreeQueue<Task>;
    public:
        ThreadPool(int _size);
        ~ThreadPool();
        TaskQueue* workQueue;
        void addTask(Task task);
        void start();
        void stop();
        bool willStop=false;
};