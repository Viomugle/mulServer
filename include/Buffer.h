#include <cstring>
#include <ranges>
#include <cstddef>
#include <cstdlib>
#include <string_view>
#include <string>
class Buffer
{
    public:
    Buffer(int _cap);
    Buffer(std::string_view _sv);
    ~Buffer();
    char* data;
    size_t size;
    size_t capacity;
    void append(std::string_view _sv);
    void append(std::string _str);
    std::string getLine();
    std::string last();
    char * c_str();
};