#include <gkr/defs.hpp>

#include <gkr/url.hpp>

#include <cctype>

namespace
{
bool is_valid_scheme(const char* str, std::size_t len)
{
    if((len == 0) || !std::isalpha(*str++)) return false;

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
bool is_valid_username(const char* str, std::size_t len)
{
    return true;
}
bool is_valid_password(const char* str, std::size_t len)
{
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
//bool is_valid_host(const char* str, std::size_t len)
//{
//    return is_valid_ipv6_addr(str, len) || is_valid_ipv4_addr(str, len) || is_valid_domain(str, len);
//}
bool is_valid_port(const char* str, std::size_t len, int& port)
{
    if((len < 1) || (len > 5)) return false;

    while(std::isdigit(*str))
    {
        port = (port * 10) + (*str++ - '0');
    }
    return ((port > 0) && (port < 65536));
}
bool is_valid_path(const char* str, std::size_t len)
{
    if(*str != '/') return false;
    return true;
}
bool is_valid_query(const char* url, std::size_t len, int& args)
{
    args = 0;
    return true;
}
bool is_valid_fragment(const char* url, std::size_t len)
{
    return true;
}
//void unescape_url(char* str)
//{
//}

const char* find_char(const char* str, std::size_t cch, int ch)
{
    for( ; cch > 0; --cch, ++str) if(*str == ch) return str;
    return nullptr;
}
}

extern "C"
{

int gkr_url_decompose(struct gkr_url_parts* parts, const char* url, unsigned len)
{
    Check_Arg_NotNull(url  , gkr_false);
    Check_Arg_NotNull(parts, gkr_false);

    *parts = gkr_url_parts {};

    if(len == unsigned(-1)) len = unsigned(std::strlen(url));

    parts->str[gkr_url_part_whole] = url;
    parts->len[gkr_url_part_whole] = len;

    std::size_t cch = len;

    for( ; (cch > 0) && std::isspace(*url); ++url, --cch);

    const char* end = find_char(url, cch, '/');
    if(end == nullptr) end = url + cch;

    const char* pos = find_char(url, cch, ':');

    if((pos != nullptr) && (pos < end))
    {
        len = unsigned(pos - url);

        if(!is_valid_scheme(url, len)) return gkr_false;

        parts->str[gkr_url_part_scheme] = url;
        parts->len[gkr_url_part_scheme] = len;

        url += len + 1;
        cch -= len + 1;
    }
    if((cch >= 2) && (url[0] == '/') && (url[1] == '/'))
    {
        url += 2;
        cch -= 2;

        end = find_char(url, cch, '/');
        if(end == nullptr) end = url + cch;

        pos = find_char(url, cch, '@');
        if((pos != nullptr) && (pos < end))
        {
            const char* sep = find_char(url, cch, ':');

            if((sep != nullptr) && (sep < pos))
            {
                len = unsigned(sep - url);

                if(!is_valid_username(url, len)) return gkr_false;

                parts->str[gkr_url_part_username] = url;
                parts->len[gkr_url_part_username] = len;

                url += len + 1;
                cch -= len + 1;

                len = unsigned(pos - url);

                if(!is_valid_password(url, len)) return gkr_false;

                parts->str[gkr_url_part_password] = url;
                parts->len[gkr_url_part_password] = len;

                url += len + 1;
                cch -= len + 1;
            }
            else
            {
                len = unsigned(pos - url);

                if(!is_valid_username(url, len)) return gkr_false;

                parts->str[gkr_url_part_username] = url;
                parts->len[gkr_url_part_username] = len;

                url += len + 1;
                cch -= len + 1;
            }
        }
        if(cch == 0) return gkr_false;

        if(*url == '[')
        {
            //IPv6
            url += 1;
            cch -= 1;

            pos = find_char(url, cch, ']');
            if((pos == nullptr) || (pos > end)) return gkr_false; // invalid IPv6 address

            len = unsigned(pos - url);

            if(!is_valid_ipv6_addr(url, len)) return gkr_false; // invalid IPv6 address

            parts->str[gkr_url_part_host] = url;
            parts->len[gkr_url_part_host] = len;

            url += len + 1;
            cch -= len + 1;

            pos = find_char(url, cch, ':');
        }
        else if(std::isdigit(*url))
        {
            //IPv4
            pos = find_char(url, cch, ':');

            if((pos != nullptr) && (pos < end))
            {
                len = unsigned(pos - url);
            }
            else
            {
                len = unsigned(end - url);
            }

            if(!is_valid_ipv4_addr(url, len)) return gkr_false; // invalid IPv4 address

            parts->str[gkr_url_part_host] = url;
            parts->len[gkr_url_part_host] = len;

            url += len;
            cch -= len;
        }
        else
        {
            //domain
            pos = find_char(url, cch, ':');

            if((pos != nullptr) && (pos < end))
            {
                len = unsigned(pos - url);
            }
            else
            {
                len = unsigned(end - url);
            }

            if(!is_valid_domain(url, len)) return gkr_false; // invalid domain

            parts->str[gkr_url_part_host] = url;
            parts->len[gkr_url_part_host] = len;

            url += len;
            cch -= len;
        }
        if((pos != nullptr) && (pos < end))
        {
            url += 1;
            cch -= 1;

            len = unsigned(end - url);

            if(!is_valid_port(url, len, parts->port)) return gkr_false; // invalid port

            parts->str[gkr_url_part_port] = url;
            parts->len[gkr_url_part_port] = len;

            url += len;
            cch -= len;
        }
    }
    if((cch >= 1) && (*url == '/'))
    {
        // path
        pos = find_char(url, cch, '?');

        if(pos != nullptr)
        {
            // path+query
            len = unsigned(pos - url);

            if(!is_valid_path(url, len)) return gkr_false; //invalid path

            parts->str[gkr_url_part_path] = url;
            parts->len[gkr_url_part_path] = len;

            url += len + 1;
            cch -= len + 1;

            pos = find_char(url, cch, '#');

            if(pos != nullptr)
            {
                // path+query+fragment
                len = unsigned(pos - url);

                if(!is_valid_query(url, len, parts->args)) return gkr_false; //invalid query

                parts->str[gkr_url_part_query] = url;
                parts->len[gkr_url_part_query] = len;

                url += len + 1;
                cch -= len + 1;

                len = unsigned(cch);

                if(!is_valid_fragment(url, len)) return gkr_false; // invalid fragment

                parts->str[gkr_url_part_fragment] = url;
                parts->len[gkr_url_part_fragment] = len;

                url += len;
                cch -= len;
            }
            else
            {
                // path+query-fragment
                len = unsigned(cch);

                if(!is_valid_query(url, len, parts->args)) return gkr_false; //invalid query

                parts->str[gkr_url_part_query] = url;
                parts->len[gkr_url_part_query] = len;

                url += len;
                cch -= len;
            }
        }
        else
        {
            // path-query
            pos = find_char(url, cch, '?');

            if(pos != nullptr)
            {
                // path-query+fragment
                len = unsigned(pos - url);

                if(!is_valid_path(url, len)) return gkr_false; //invalid path

                parts->str[gkr_url_part_path] = url;
                parts->len[gkr_url_part_path] = len;

                url += len + 1;
                cch -= len + 1;

                len = unsigned(cch);

                if(!is_valid_fragment(url, len)) return gkr_false; // invalid fragment

                parts->str[gkr_url_part_fragment] = url;
                parts->len[gkr_url_part_fragment] = len;

                url += len;
                cch -= len;
            }
            else
            {
                // path-query-fragment
                len = unsigned(cch);

                if(!is_valid_path(url, len)) return gkr_false; //invalid path

                parts->str[gkr_url_part_path] = url;
                parts->len[gkr_url_part_path] = len;

                url += len;
                cch -= len;
            }
        }
    }
    else
    {
        // empty path
        if(cch > 0) return gkr_false;

        len = 0;

        parts->str[gkr_url_part_path] = url;
        parts->len[gkr_url_part_path] = len;
    }

    for( ; (cch > 0) && std::isspace(*url); ++url, --cch);

    return (cch == 0);
}

GKR_CORE_API gkr_bool gkr_url_change_part(int part, struct gkr_url_parts* parts, char* url, unsigned cch, const char* str, unsigned len)
{
    Check_Arg_IsValid(part < gkr_url_parts_count, gkr_false);

    Check_Arg_NotNull(parts, gkr_false);
    Check_Arg_NotNull(url  , gkr_false);

    if(len == unsigned(-1)) len = unsigned(std::strlen(url));

    if(part == gkr_url_part_whole)
    {
        Check_Arg_NotNull(str, gkr_false);

        if(cch <= len) return gkr_false;

        std::memcpy(url, str, len);
        url[len] = 0;

        return gkr_url_decompose(parts, url, len);
    }

    if((str == nullptr) && (parts->str[part] == nullptr)) return gkr_true;

    const char* prefix = "";
    const char* suffix = "";

    std::size_t offset = 0;

    switch(part)
    {
        case gkr_url_part_scheme:
            if((str != nullptr) && !is_valid_scheme(str, len)) return gkr_false;
            suffix = ":";
            if     (parts->str[gkr_url_part_scheme  ] != nullptr) offset = 0;
            else if(parts->str[gkr_url_part_username] != nullptr) offset = std::size_t(parts->str[gkr_url_part_username] - 2 - parts->str[gkr_url_part_whole]);
            else if(parts->str[gkr_url_part_host    ] != nullptr) offset = std::size_t(parts->str[gkr_url_part_host    ] - 2 - parts->str[gkr_url_part_whole]);
            else if(parts->str[gkr_url_part_path    ] != nullptr) offset = std::size_t(parts->str[gkr_url_part_path    ] - 1 - parts->str[gkr_url_part_whole]);
            else return gkr_false;
            break;
    }

    std::size_t old_len = parts->len[part];
    std::size_t new_len = (str == nullptr) ? 0 : len;

    if(str == nullptr)
    {
        old_len += std::strlen(prefix) + std::strlen(suffix);

        char* dest = url + (parts->str[part] - parts->str[gkr_url_part_whole]) - std::strlen(prefix);

        std::size_t size = parts->len[gkr_url_part_whole] - old_len - (dest - url);

        std::memmove(dest + new_len, dest + old_len, size);

        parts->str[part] = nullptr;
        parts->len[part] = 0;

        len = unsigned(parts->len[gkr_url_part_whole] - old_len);

        url[len] = 0;

        parts->len[gkr_url_part_whole] = len;

        for(int index = part + 1; index < gkr_url_parts_count; ++index)
        {
            if(parts->str[index] != nullptr) parts->str[index] -= old_len;
        }
    }
    else if(parts->str[part] == nullptr)
    {
        new_len += std::strlen(prefix) + std::strlen(suffix);

        char* dest = url + offset;

        std::size_t size = std::size_t(parts->len[gkr_url_part_whole] - (dest - url));

        std::memmove(dest + new_len, dest, size);

        size = std::strlen(prefix); if(size > 0) { std::memcpy(dest, prefix, size); dest += size; }
        parts->str[part] = dest;
        parts->len[part] = len;
        size = len;                 if(size > 0) { std::memcpy(dest, str   , size); dest += size; }
        size = std::strlen(suffix); if(size > 0) { std::memcpy(dest, suffix, size); dest += size; }

        len = unsigned(parts->len[gkr_url_part_whole] + new_len);

        url[len] = 0;

        parts->len[gkr_url_part_whole] = len;

        for(int index = part + 1; index < gkr_url_parts_count; ++index)
        {
            if(parts->str[index] != nullptr) parts->str[index] += new_len;
        }
    }
    return gkr_true;
}

}
