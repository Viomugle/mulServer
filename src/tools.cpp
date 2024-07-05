#include "tools.h"

namespace tools
{
    void assert(bool cond,const char* mes)
{
    if(cond==true)
    {
        printf("%s\n",mes);
        exit(1);
    }
}
} // namespace name

