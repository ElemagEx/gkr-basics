#include <gkr/defs.hpp>

#include <gkr/url.hpp>

namespace
{
bool is_valid_scheme_name(const char* str, std::size_t len)
{
    if(!std::isalpha(*str++)) return false;

    for( ; --len > 0; ++str)
    {
        switch(*str)
        {
            case '+':
            case '.':
            case '-': break;
            default : if(!std::isalnum(*str)) return false; break;
        }
    }
    return true;
}
bool is_valid_ipv6_addr(const char* str, std::size_t len)
{
    // Veeeeery loose
    for( ; len-- > 0; ++str)
    {
        if((*str != ':') && (*str != '%') && !std::isxdigit(*str)) return false;
    }
    return true;
}
bool is_valid_ipv4_addr(const char* str, std::size_t len)
{
    // Veeeeeeeeery loose
    for( ; len-- > 0; ++str)
    {
        if((*str != '.') && !std::isdigit(*str)) return false;
    }
    return true;
}
bool is_valid_domain(const char* str, std::size_t len)
{
    // Veeeeeeeeeeeery loose
    return (len > 0);
}
bool is_valid_port(const char* str, std::size_t len, int& port)
{
    if((len < 1) || (len > 5)) return false;

    while(std::isdigit(*str))
    {
        port = (port * 10) + (*str++ - '0');
    }
    return ((port > 0) && (port < 65536));
}
bool is_valid_path(const char* str)
{
    if(*str != '/') return false;
    return true;
}
bool is_valid_query(const char* str, std::size_t& len, int& args)
{
    args = 0;
    return true;
}
bool is_valid_fragment(const char* str, std::size_t& len)
{
    return (len > 0);
}
void unescape_url(char* str)
{
}
}

extern "C"
{

int gkr_url_decompose(char* url, int unescape, gkr_url_parts* parts)
{
    Check_Arg_NotNull(url  , 0);
    Check_Arg_NotNull(parts, 0);

    parts->path = nullptr;

    for( ; std::isspace(*url); ++url);

    char* path = std::strchr(url, '/');
    if(path == nullptr) return 0; // path missing

    char* pos = std::strchr(url, ':');
    if((pos == nullptr) || (pos > path)) return 0; // scheme missing

    std::size_t len = std::size_t(pos - url);
    if(!is_valid_scheme_name(url, len)) return 0; // invalid scheme name

    url[len] = 0;
    parts->scheme = url;
    url += len + 1;

    if(url++ != path) return 0; // scheme colon followed by invalid character

    if(*url != '/')
    {
        parts->username = nullptr;
        parts->password = nullptr;
        parts->host     = nullptr;
        parts->port     = 0;
    }
    else
    {
        path = std::strchr(++url, '/');
        if(path == nullptr) return 0; // path missing

        pos = std::strchr(url, '@');
        if((pos == nullptr) || (pos > path))
        {
            parts->username = nullptr;
            parts->password = nullptr;
        }
        else
        {
            parts->username = url;
            char* sep = std::strchr(url, ':');
            if((sep == nullptr) || (pos > sep))
            {
                parts->password = nullptr;
            }
            else
            {
                url[sep - pos] = 0;
                parts->password = sep+1;
            }
            url[pos-url] = 0;
            url = pos + 1;
        }
        if(*url == '[')
        {
            //IPv6
            pos = std::strchr(++url, ']');
            if((pos == nullptr) || (pos > path)) return 0; // invalid IPv6 address
            len = std::size_t(pos - url);
            if(!is_valid_ipv6_addr(url, len)) return 0; // invalid IPv6 address
            parts->host = url;
            url[len] = 0;
            url += len + 1;
        }
        else if(std::isdigit(*url))
        {
            //IPv4
            pos = std::strchr(url, ':');
            if((pos == nullptr) || (pos > path))
            {
                len = std::size_t(path - url);
            }
            else
            {
                len = std::size_t(pos - url);
            }
            if(!is_valid_ipv4_addr(url, len)) return 0; // invalid IPv4 address
            parts->host = url;
            url += len;
        }
        else
        {
            //domain
            pos = std::strchr(url, ':');
            if((pos == nullptr) || (pos > path))
            {
                len = std::size_t(path - url);
            }
            else
            {
                len = std::size_t(pos - url);
            }
            if(!is_valid_domain(url, len)) return 0; // invalid domain
            parts->host = url;
            url += len;
        }
        if(*url == ':')
        {
            *url++ = 0;
            len = std::size_t(path - url);
            if(!is_valid_port(url, len, parts->port)) return 0; // invalid port
            url += len;
        }
        if((*url != '/') || (url != path)) return 0; // invalid chars before path
    }
    if(unescape) unescape_url(url);

    pos = std::strchr(url, '?');
    if(pos != nullptr)
    {
        *url++ = 0;
        if(!is_valid_query(url, len, parts->args)) return 0; //invalid query
        url[len] = 0;
        parts->query = url;
        url += len + 1;
    }
    pos = std::strchr(url, '#');
    if(pos != nullptr)
    {
        *url++ = 0;
        if(!is_valid_fragment(url, len)) return 0; //invalid fragment
        url[len] = 0;
        parts->fragment = url;
        url += len + 1;
    }
    if(!is_valid_path(path)) return 0; // invalid path

    parts->path = path;
    return 1;
}

}
