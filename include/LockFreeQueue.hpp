#pragma once
#include <atomic>
template<typename T>
class LockFreeQueue
{
    public:
        LockFreeQueue(int _size);
        ~LockFreeQueue();
        bool push(T);
        T pop();
        T* data;
        int size;
        std::atomic<int> head;
        std::atomic<int> tail;
        std::atomic<int> count;
        bool isEmpty();
};

template<typename T>
LockFreeQueue<T>::~LockFreeQueue()
{
    delete[] data;
}


template<typename T>
bool LockFreeQueue<T>::push(T t)
{
    if(count.load()==size)
    {
        return false;
    }
    data[tail.load()]=t;
    tail.store((tail.load()+1)%size);
    count.store(count.load()+1);
    return true;
}

template<typename T>
T LockFreeQueue<T>::pop()
{
    if(count.load()==0)
    {
        return NULL;
    }
    T t=data[head.load()];
    head.store((head.load()+1)%size);
    count.store(count.load()-1);
    return t;
}

template <typename T>
LockFreeQueue<T>::LockFreeQueue(int _size)
{
    size=_size;
    data=new T[size];
    head.store(0);
    tail.store(0);
    count.store(0);
}


template<typename T>    
bool LockFreeQueue<T>::isEmpty()
{
    return count.load()==0;
}