#ifndef LFRINGBUFFER_H
#define LFRINGBUFFER_H
#include <atomic>
using namespace std;
template <typename T>
class lfRingBuffer
{

private:
    atomic<int> head_val = 0;
    atomic<int> head_olloc = 0;
    atomic<int> tail_val = 0;
    atomic<int> tail_alloc = 0;
    T *buffer;
    int capacity;

public:
    lfRingBuffer(int n = 8) : capacity(n)
    {
        buffer = new T[n];
    }
    ~lfRingBuffer()
    {
        delete[] buffer;
    }
    bool back(T data)
    {
        auto t_alloc = tail_alloc.load();
        auto h_alloc = head_olloc.load();
        auto t_val = tail_val.load();
        auto h_val = head_val.load();
        do
        {
            t_alloc = tail_alloc.load();
            h_alloc = head_olloc.load();
            t_val = tail_val.load();
            h_val = head_val.load();
            if ((t_alloc + 1) % capacity == h_alloc)
            {
                return false;
            }
            buffer[t_alloc] = data;
            tail_val.store(t_alloc);
        } while (!tail_alloc.compare_exchange_weak(t_alloc, (t_alloc + 1) % capacity));
        return true;
    }
    bool front(T & data)
    {

        auto t_alloc = tail_alloc.load();
        auto h_alloc = head_olloc.load();
        auto t_val = tail_val.load();
        auto h_val = head_val.load();
        do
        {
            t_alloc = tail_alloc.load();
            h_alloc = head_olloc.load();
            t_val = tail_val.load();
            h_val = head_val.load();
            if (h_alloc == t_alloc)
            {
                return false;
            } // empty
            data = buffer[h_val];
            head_val.store((h_alloc + 1) % capacity);

        } while (head_olloc.compare_exchange_weak(h_alloc, (h_alloc + 1) % capacity)!=true);
        return true;
    }
};
#endif