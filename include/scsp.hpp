#ifndef _SCSP_HPP_
#define _SCSP_HPP_
#include <atomic>
using namespace std;
template<typename T>
class scsp
{
    private:
        T* buffer;
        int capacity;
        atomic<int> head;
        atomic<int> tail;
    public:
    scsp(int _n=8)
    {
        capacity=_n;
        buffer=(T*)malloc(capacity*sizeof(T));
        head=0;
        tail=0;
    }
    ~scsp()
    {
        free(buffer);
    }
    bool back(const T _val)
    {
        auto h=head.load();
        auto t=tail.load();
        auto target=(t+1)%capacity;
        if(target==h)//full
        return false;
        buffer[t]=_val;
        tail.store(target);
        return true;
    }
    bool front(T& _val)
    {
        auto h=head.load();
        auto t=tail.load();
        auto target=(h+1)%capacity;
        if(h==t)//empty
        return false;
        _val=buffer[h];
        head.store((h+1)%capacity);
        return true;
    }
    bool empty()
    {
        return head.load()==tail.load();
    }
};


#endif