#pragma once
#include <cstring>
#include <ranges>
#include <cstddef>
#include <cstdlib>
#include <string_view>
#include <string>
#include <new>
#include <tools.h>
#include "tools.h"
#include <thread>
class ReadBuffer
{
    public:
    ReadBuffer();
    ReadBuffer(int _cap);
    ReadBuffer(std::string_view _sv);
    ~ReadBuffer();
    char* data=nullptr;
    std::size_t size;
    std::size_t capacity;
    void append(std::string_view _sv);
    void append(std::string _str);
    void append(const char* _str);
    std::string getLine();
    char * c_str();
    std::string substr(ssize_t _b,ssize_t _e);
    void removeFront(ssize_t n);
    int find(std::string _tok,int _pos);
};