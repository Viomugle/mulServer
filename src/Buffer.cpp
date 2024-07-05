#include "Buffer.h"

Buffer::Buffer(int _size)
{
    data=(char*)malloc(_size);
    memset(data,'\0',_size);
}
Buffer::Buffer(std::string_view _sv)
{
    data=(char*)malloc(_sv.size()*2);
    memset(data,'\0',_sv.size()*2);
    memcpy(data,_sv.data(),_sv.size());
    size=_sv.size();
    capacity=_sv.size()*2;
}

Buffer::~Buffer()
{
    free(data);
}

void Buffer::append(std::string_view _sv)
{
    if(size+_sv.size()>capacity)
    {
        auto newdata=(char*)malloc(2*(size+_sv.size()));
        memset(newdata,'\0',2*(size+_sv.size()));
        memcpy(newdata,data,size);
        free(data);
        data=newdata;
        capacity=2*(size+_sv.size());
        size=size+_sv.size();
    }
    else
    {
        memcpy(data+size,_sv.data(),_sv.size());
        size+=_sv.size();
    }
}

void Buffer::append(std::string _str)
{
    append(std::string_view(_str));
}

std::string Buffer::getLine()
{
    std::string_view sv(data,size);
    return std::string(sv.substr(0, sv.find('\n')));
}

char * Buffer::c_str()
{
    return data;
}