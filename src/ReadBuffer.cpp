#include "ReadBuffer.h"
#include <cstdlib>
#include <cstdio>
ReadBuffer::ReadBuffer()
{
    data = (char *)malloc(1024);
    tools::assert(data == nullptr, "Buffer create failed");
    memset(data, '\0', 1024);
    size = 0;
    capacity = 1024;
}

ReadBuffer::ReadBuffer(int _size)
{
    data = (char *)malloc(_size);
    memset(data, '\0', _size);
}
ReadBuffer::ReadBuffer(std::string_view _sv)
{
    data = (char *)malloc(_sv.size() * 2*sizeof(char));
    memset(data, '\0', _sv.size() * 2);
    memcpy(data, _sv.data(), _sv.size());
    size = _sv.size();
    capacity = _sv.size() * 2;
}

ReadBuffer::~ReadBuffer()
{
    if (data != nullptr)
        free(data);
    data = nullptr;
}

void ReadBuffer::append(const char *_str)
{
    append(std::string_view(_str));
}

void ReadBuffer::append(std::string_view _sv)
{
    if (size + _sv.size() > capacity)
    {
        auto newdata = (char *)malloc(sizeof(char)*2 * (size + _sv.size()));
        memset(newdata, '\0', sizeof(char)*2 * (size + _sv.size()));
        memcpy(newdata, data, size);
        free(data);
        data = newdata;
        capacity = 2 * (size + _sv.size());
        size = size + _sv.size();
    }
    else
    {
        memcpy(data + size, _sv.data(), _sv.size());
        size += _sv.size();
    }
}

void ReadBuffer::append(std::string _str)
{
    append(std::string_view(_str));
}

std::string ReadBuffer::getLine()
{
    std::string_view sv(data, size);
    return std::string(sv.substr(0, sv.find('\n')));
}

char *ReadBuffer::c_str()
{
    if (data != nullptr)
        return data;
    else
        tools::assert(true, "trying to get a nullptr");
}



int ReadBuffer::find(std::string _tok,int _pos)
{
    return std::string_view(data).find(_tok,_pos);
}

std::string ReadBuffer::substr(ssize_t _b, ssize_t _len)
{
    std::string_view sv(data);
    return std::string(sv.substr(_b, _len));
}

void ReadBuffer::removeFront(ssize_t n)
{
    tools::assert(n>size,"trying to remove more than size");
    memmove(data,data+n,size-n);
    size-=n;
    memset(data+size,'\0',n);

}