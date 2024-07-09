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
    data = (char *)malloc(_sv.size() * 2);
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

void ReadBuffer::append(std::string_view _sv)
{
    printf("size is: %d sv.size is: %d cap is: %d\n", size, _sv.size(), capacity);
    if (size + _sv.size() > capacity)
    {
        auto newdata = (char *)malloc(2 * (size + _sv.size()));
        memset(newdata, '\0', 2 * (size + _sv.size()));
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
    printf("size is: %d sv.size is: %d cap is: %d\n", size, _sv.size(), capacity);
    printf("buf is:\n%s\n", data);
    printf("thread id is: %d\n", std::this_thread::get_id());
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
    if(data!=nullptr)
    return data;
    else
    tools::assert(true,"trying to get a nullptr");
}

int ReadBuffer::findFirst(std::string_view _sv)
{
    std::string_view sv(data, 0);
    return sv.find(_sv);
}