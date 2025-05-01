#include <gkr/defs.hpp>

#include <gkr/url.hpp>

#include <cctype>

namespace
{
bool is_valid_scheme(const char* str, std::size_t len)
{
    if(str != nullptr)
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
    }
    return true;
}
bool is_valid_username(const char* str, std::size_t len)
{
    if(str != nullptr)
    {
    }
    return true;
}
bool is_valid_password(const char* str, std::size_t len)
{
    if(str != nullptr)
    {
    }
    return true;
}
bool is_valid_ipv6_addr(const char* str, std::size_t len)
{
    if(str != nullptr)
    {
        for( ; len-- > 0; ++str)
        {
            if((*str != ':') && (*str != '%') && !std::isxdigit(*str)) return false;
        }
    }
    return true;
}
bool is_valid_ipv4_addr(const char* str, std::size_t len)
{
    if(str != nullptr)
    {
        for( ; len-- > 0; ++str)
        {
            if((*str != '.') && !std::isdigit(*str)) return false;
        }
    }
    return true;
}
bool is_valid_domain(const char* str, std::size_t len)
{
    if(str != nullptr)
    {
    }
    return true;
}
bool is_valid_host(const char* str, std::size_t len)
{
    return is_valid_ipv6_addr(str, len) || is_valid_ipv4_addr(str, len) || is_valid_domain(str, len);
}
bool is_valid_port(const char* str, std::size_t len, unsigned& port)
{
    port = 0;
    if(str != nullptr)
    {
        if((len < 1) || (len > 5)) return false;

        while(std::isdigit(*str))
        {
            port = (port * 10) + (*str++ - '0');
        }

        if((port < 1) || (port > 65535)) return false;
    }
    return true;
}
bool is_valid_path(const char* str, std::size_t len)
{
    if((str != nullptr) && (len > 0))
    {
        if(*str != '/') return false;
    }
    return true;
}
bool is_valid_query(const char* str, std::size_t len, unsigned& args)
{
    args = 0;
    if(str != nullptr)
    {
    }
    return true;
}
bool is_valid_fragment(const char* str, std::size_t len)
{
    if(str != nullptr)
    {
    }
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

int gkr_url_decompose(struct gkr_url_data* data, const char* url, unsigned len)
{
    Check_Arg_NotNull(url , gkr_false);
    Check_Arg_NotNull(data, gkr_false);

    *data = gkr_url_data {};

    if(len == unsigned(-1)) len = unsigned(std::strlen(url));

    data->str[gkr_url_part_whole] = url;
    data->len[gkr_url_part_whole] = len;

    std::size_t cch = len;

    const char* end = find_char(url, cch, '/');
    if(end == nullptr) return gkr_false; // must have at least one of host or path

    const char* pos = find_char(url, cch, ':');

    if((pos != nullptr) && (pos < end))
    {
        len = unsigned(pos - url);

        if(!is_valid_scheme(url, len)) return gkr_false;

        data->str[gkr_url_part_scheme] = url;
        data->len[gkr_url_part_scheme] = len;

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

                data->str[gkr_url_part_username] = url;
                data->len[gkr_url_part_username] = len;

                url += len + 1;
                cch -= len + 1;

                len = unsigned(pos - url);

                if(!is_valid_password(url, len)) return gkr_false;

                data->str[gkr_url_part_password] = url;
                data->len[gkr_url_part_password] = len;

                url += len + 1;
                cch -= len + 1;
            }
            else
            {
                len = unsigned(pos - url);

                if(!is_valid_username(url, len)) return gkr_false;

                data->str[gkr_url_part_username] = url;
                data->len[gkr_url_part_username] = len;

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

            data->str[gkr_url_part_host] = url;
            data->len[gkr_url_part_host] = len;

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

            data->str[gkr_url_part_host] = url;
            data->len[gkr_url_part_host] = len;

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

            data->str[gkr_url_part_host] = url;
            data->len[gkr_url_part_host] = len;

            url += len;
            cch -= len;
        }
        if((pos != nullptr) && (pos < end))
        {
            url += 1;
            cch -= 1;

            len = unsigned(end - url);

            if(!is_valid_port(url, len, data->port)) return gkr_false; // invalid port

            data->str[gkr_url_part_port] = url;
            data->len[gkr_url_part_port] = len;

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

            data->str[gkr_url_part_path] = url;
            data->len[gkr_url_part_path] = len;

            url += len + 1;
            cch -= len + 1;

            pos = find_char(url, cch, '#');

            if(pos != nullptr)
            {
                // path+query+fragment
                len = unsigned(pos - url);

                if(!is_valid_query(url, len, data->args)) return gkr_false; //invalid query

                data->str[gkr_url_part_query] = url;
                data->len[gkr_url_part_query] = len;

                url += len + 1;
                cch -= len + 1;

                len = unsigned(cch);

                if(!is_valid_fragment(url, len)) return gkr_false; // invalid fragment

                data->str[gkr_url_part_fragment] = url;
                data->len[gkr_url_part_fragment] = len;

                url += len;
                cch -= len;
            }
            else
            {
                // path+query-fragment
                len = unsigned(cch);

                if(!is_valid_query(url, len, data->args)) return gkr_false; //invalid query

                data->str[gkr_url_part_query] = url;
                data->len[gkr_url_part_query] = len;

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

                data->str[gkr_url_part_path] = url;
                data->len[gkr_url_part_path] = len;

                url += len + 1;
                cch -= len + 1;

                len = unsigned(cch);

                if(!is_valid_fragment(url, len)) return gkr_false; // invalid fragment

                data->str[gkr_url_part_fragment] = url;
                data->len[gkr_url_part_fragment] = len;

                url += len;
                cch -= len;
            }
            else
            {
                // path-query-fragment
                len = unsigned(cch);

                if(!is_valid_path(url, len)) return gkr_false; //invalid path

                data->str[gkr_url_part_path] = url;
                data->len[gkr_url_part_path] = len;

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

        data->str[gkr_url_part_path] = url;
        data->len[gkr_url_part_path] = len;
    }
    return (cch == 0);
}

GKR_CORE_API gkr_bool gkr_url_change_part(int part, struct gkr_url_data* data, char* url, unsigned cch, const char* str, unsigned len)
{
    Check_Arg_IsValid(part < gkr_url_parts_count, gkr_false);

    Check_Arg_NotNull(data, gkr_false);
    Check_Arg_NotNull(url , gkr_false);

    if(str == nullptr) len = 0; else if(len == unsigned(-1)) len = unsigned(std::strlen(url));

    if(part == gkr_url_part_whole)
    {
        Check_Arg_NotNull(str, gkr_false);

        if(cch <= len) return gkr_false;

        std::memcpy(url, str, len);
        url[len] = 0;

        return gkr_url_decompose(data, url, len);
    }

    if((str == nullptr) && (data->str[part] == nullptr)) return gkr_true;

    if((data->len[gkr_url_part_whole] + len - data->len[part] + 1) > cch) return gkr_false;

    const char* prefix = "";
    const char* suffix = "";

    std::ptrdiff_t offset = 0;

    switch(part)
    {
        case gkr_url_part_scheme:
            if(!is_valid_scheme(str, len)) return gkr_false;
            suffix = ":";
            if     (data->str[gkr_url_part_scheme  ] != nullptr) offset = data->str[gkr_url_part_scheme  ] - 0 - data->str[gkr_url_part_whole];
            else if(data->str[gkr_url_part_username] != nullptr) offset = data->str[gkr_url_part_username] - 2 - data->str[gkr_url_part_whole];
            else if(data->str[gkr_url_part_host    ] != nullptr) offset = data->str[gkr_url_part_host    ] - 2 - data->str[gkr_url_part_whole];
            else if(data->str[gkr_url_part_path    ] != nullptr) offset = data->str[gkr_url_part_path    ] - 1 - data->str[gkr_url_part_whole];
            else return gkr_false;
            break;

        case gkr_url_part_username:
            if((str == nullptr) && (data->str[gkr_url_part_password] != nullptr)) return false;

            if(!is_valid_username(str, len)) return gkr_false;
            suffix = (data->str[gkr_url_part_password] != nullptr) ? ":" : "@";
            if     (data->str[gkr_url_part_username] != nullptr) offset = data->str[gkr_url_part_username] - 0 - data->str[gkr_url_part_whole];
            else if(data->str[gkr_url_part_host    ] != nullptr) offset = data->str[gkr_url_part_host    ] - 0 - data->str[gkr_url_part_whole];
            else return gkr_false;
            break;

        case gkr_url_part_password:
            if(!is_valid_password(str, len)) return gkr_false;
            prefix = ":";
            if     (data->str[gkr_url_part_username] == nullptr) return gkr_false;
            if     (data->str[gkr_url_part_password] != nullptr) offset = data->str[gkr_url_part_password] - 1 - data->str[gkr_url_part_whole];
            else if(data->str[gkr_url_part_host    ] != nullptr) offset = data->str[gkr_url_part_host    ] - 1 - data->str[gkr_url_part_whole];
            else return gkr_false;
            break;

        case gkr_url_part_host:
            if((str == nullptr) && (data->len[gkr_url_part_path] == 0)) return false;

            if(!is_valid_host(str, len)) return gkr_false;

            if(data->str[gkr_url_part_host] == nullptr)
            {
                prefix = "//";
                offset = data->str[gkr_url_part_path] - data->str[gkr_url_part_whole];
            }
            else if(str == nullptr)
            {
                if(data->str[gkr_url_part_username] != nullptr) return gkr_false;
                if(data->str[gkr_url_part_password] != nullptr) return gkr_false;
                if(data->str[gkr_url_part_port    ] != nullptr) return gkr_false;
                prefix = "//";
                offset = data->str[gkr_url_part_host] - 2 - data->str[gkr_url_part_whole];
            }
            else
            {
                offset = data->str[gkr_url_part_host] - 0 - data->str[gkr_url_part_whole];
            }
            break;

        case gkr_url_part_port:
            if(!is_valid_port(str, len, data->port)) return gkr_false;
            prefix = ":";
            if     (data->str[gkr_url_part_host    ] == nullptr) return gkr_false;
            if     (data->str[gkr_url_part_port    ] != nullptr) offset = data->str[gkr_url_part_port    ] - 1 - data->str[gkr_url_part_whole];
            else if(data->str[gkr_url_part_path    ] != nullptr) offset = data->str[gkr_url_part_path    ] - 0 - data->str[gkr_url_part_whole];
            else return gkr_false;
            break;

        case gkr_url_part_path:
            if((str == nullptr) && (data->str[gkr_url_part_host] == nullptr)) return false;

            if(!is_valid_path(str, len)) return gkr_false;
            prefix = "";
            if     (data->str[gkr_url_part_path    ] != nullptr) offset = data->str[gkr_url_part_path    ] - 0 - data->str[gkr_url_part_whole];
            else return gkr_false;
            break;

        case gkr_url_part_query:
            if(!is_valid_query(str, len, data->args)) return gkr_false;
            prefix = "?";
            if     (data->str[gkr_url_part_query   ] != nullptr) offset = data->str[gkr_url_part_query   ] - 1 - data->str[gkr_url_part_whole];
            else if(data->str[gkr_url_part_fragment] != nullptr) offset = data->str[gkr_url_part_fragment] - 1 - data->str[gkr_url_part_whole];
            else if(data->len[gkr_url_part_path    ] != 0      ) offset = data->len[gkr_url_part_whole];
            else return gkr_false;
            break;

        case gkr_url_part_fragment:
            if(!is_valid_fragment(str, len)) return gkr_false;
            prefix = "#";
            if     (data->str[gkr_url_part_fragment] != nullptr) offset = data->str[gkr_url_part_fragment] - 1 - data->str[gkr_url_part_whole];
            else if(data->len[gkr_url_part_path    ] != 0      ) offset = data->len[gkr_url_part_whole];
            else return gkr_false;
            break;
    }

    char* dest = url + offset;

    if(data->str[part] != nullptr)
    {
        const std::size_t old_len = std::strlen(prefix) + data->len[part] + std::strlen(suffix);

        std::size_t size = data->len[gkr_url_part_whole] - offset - old_len;

        std::memmove(dest, dest + old_len, size);

        data->str[part] = (part == gkr_url_part_path) ? dest : nullptr;
        data->len[part] = 0;

        data->len[gkr_url_part_whole] -= unsigned(old_len);

        url[data->len[gkr_url_part_whole]] = 0;

        for(int index = part + 1; index < gkr_url_parts_count; ++index)
        {
            if(data->str[index] != nullptr) data->str[index] -= old_len;
        }
    }
    if(str != nullptr)
    {
        const std::size_t new_len = std::strlen(prefix) + len + std::strlen(suffix);

        std::size_t size = data->len[gkr_url_part_whole] - std::size_t(offset);

        std::memmove(dest + new_len, dest, size);

        size = std::strlen(prefix); if(size > 0) { std::memcpy(dest, prefix, size); dest += size; }
        data->str[part] = dest;
        data->len[part] = len;
        size = len;                 if(size > 0) { std::memcpy(dest, str   , size); dest += size; }
        size = std::strlen(suffix); if(size > 0) { std::memcpy(dest, suffix, size); dest += size; }

        data->len[gkr_url_part_whole] += unsigned(new_len);

        url[data->len[gkr_url_part_whole]] = 0;

        for(int index = part + 1; index < gkr_url_parts_count; ++index)
        {
            if(data->str[index] != nullptr) data->str[index] += new_len;
        }
    }
    return gkr_true;
}

}
