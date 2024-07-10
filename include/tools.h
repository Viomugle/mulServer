#include <cstdlib>
#include <cstdarg>
#include <cstdio>

#define DEF_CON(cname) cname()=default;
#define DEF_DES(cname) ~cname()=default;
#define DEFAULT(cname)  DEF_CON(cname)\ 
                        DEF_DES(cname)
namespace tools{
void assert(bool cond,const char* mes);
}
