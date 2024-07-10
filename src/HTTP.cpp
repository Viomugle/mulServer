#include "HTTP.h"
#include <iostream>
std::optional<std::unordered_map<std::string, std::string>> HTTP::parseHTTP(ReadBuffer& _rb)
{
    std::string tok="\r\n";
    std::string toks="\r\n\r\n";
    std::unordered_map<std::string, std::string> res;
    std::string reqline;
    std::string headers;

    auto i=_rb.find(tok,0);
    if(i!=std::string::npos)
        reqline=_rb.substr(0,i);
    else    
        return std::nullopt;
    
    auto j=_rb.find(toks,i);
    if(j!=std::string::npos)
        headers=_rb.substr(i,j-i);
    else
        return std::nullopt;

    _rb.removeFront(j+toks.size());
    
    auto method=reqline.substr(0,reqline.find(' '));
    auto URL=reqline.substr(reqline.find(' ')+1,reqline.rfind(' ')-reqline.find(' ')-1);
    auto version=reqline.substr(reqline.rfind(' ')+1);

    res["method"]=method;
    res["URL"]=URL;
    res["version"]=version;
    
    std::vector<std::string> headlines;
    int begin=toks.size();
    while(true)
    {
        auto pos=headers.find(tok,begin);
        if(pos==std::string::npos)
        {
            break;
        }
        headlines.push_back(headers.substr(begin,pos-begin));
        begin=pos+tok.size();

    }
    for(auto &i:headlines)
    {
        auto pos=i.find(':');
        if(pos==std::string::npos)
        {
            return std::nullopt;
        }
        res[i.substr(0,pos)]=i.substr(pos+1);
    }

    return res;

}
