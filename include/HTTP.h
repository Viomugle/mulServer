#include "Buffer.h"
#include <string>
#include <map>
class HTTP
{
    enum Name{
        GET,
        POST,
        HEAD,
        PUT,
        DELETE,
        CONNECT,
        OPTIONS,
        TRACE,
        PATCH
    };
    enum Version{
        HTTP_1_0,
        HTTP_1_1,
        HTTP_2_0
    };
    enum StatusCode{
        OK=200,
        BAD_REQUEST=400,
        NOT_FOUND=404,
        METHOD_NOT_ALLOWED=405,
        REQUEST_TIMEOUT=408,
        INTERNAL_SERVER_ERROR=500,
        NOT_IMPLEMENTED=501,
        HTTP_VERSION_NOT_SUPPORTED=505
    };
    enum Header{
        Accept,
        Accept_Charset,
        Accept_Encoding,
        Accept_Language,
        Accept_Datetime,
        Authorization,
        Cache_Control,
        Connection,
        Cookie,
        Content_Length,
        Content_MD5,
        Content_Type,
        Date,
        Expect,
        Forwarded,
        From,
        Host,
        If_Match,
        If_Modified_Since,
        If_None_Match,
        If_Range,
        If_Unmodified_Since,
        Max_Forwards,
        Origin,
        Pragma,
        Proxy_Authorization,
        Range,
        Referer,
        TE,
        User_Agent,
        Upgrade,
        Via,
        Warning
    };

    struct Request
    {
        Name name;
        std::string url;
        Version version;
        std::map<Header,std::string> headers;
        std::string body;
    };
    struct Response
    {
        Version version;
        StatusCode status;
        std::map<Header,std::string> headers;
        std::string body;
    };
    public:
        HTTP();
        ~HTTP();
        Request parseRequest(Buffer* buffer);
        Response parseResponse(Buffer* buffer);
        Buffer* encodeRequest(Request request);
        Buffer* encodeResponse(Response response);
        std::string getHeader(Header header);
        void setHeader(Header header,std::string value);
        void setBody(std::string body);
        void setVersion(Version version);
        void setStatus(StatusCode status);
        void setName(Name name);
        void setUrl(std::string url);
        void setHeader(std::string header,std::string value);
        std::string getBody();
        Version getVersion();
        StatusCode getStatus();
        Name getName();
        std::string getUrl();
        std::string getHeader(std::string header);
        std::string getHeader(Header header);
        std::map<Header,std::string> getHeaders();
        std::map<std::string,std::string> getHeaders();


}