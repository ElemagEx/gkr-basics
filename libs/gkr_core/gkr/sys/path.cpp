#include "path.hpp"

#include <gkr/diagnostics.hpp>

#include <cstring>

namespace
{
const char* path_file_begin(const char* path)
{
    Assert_Check(path != nullptr);

    const char* begin;
#ifdef _WIN32
    begin = std::strrchr(path, '\\'); if(begin != nullptr) return (begin + 1);
#endif
    begin = std::strrchr(path, '/' ); if(begin != nullptr) return (begin + 1);
    return path;
}
}

namespace gkr
{
namespace sys
{

GKR_INNER_API bool path_has_extension(const char* path, const char* ext)
{
    Check_Arg_NotNull(path, false);
    Check_Arg_NotNull(ext , false);

    const char* name = path_file_begin(path);

    if(*ext == 0)
    {
        return (std::strchr(name, '.') == nullptr);
    }
    else
    {
        const char* pos = std::strrchr(name, '.');

        return (pos == nullptr)
            ? false
            : !std::strcmp(ext, pos + 1)
            ;
    }
}

GKR_INNER_API std::string path_insert_ext(const char* path, const char* ext, int order)
{
    Check_Arg_NotNull(path     , {});
    Check_Arg_NotNull( ext     , {});
    Check_Arg_IsValid(*ext != 0, {});

    const char* name = path_file_begin(path);

    std::string str(path, std::size_t(name - path));

    const char* pos = name;

    if(order > 0)
    {
        for( ; *pos != 0; ++pos)
        {
            if(*pos == '.') if(--order == 0) break;
        }
    }
    else if(order < 0)
    {
        for(const char* end = pos + std::strlen(pos) ; end > pos; --end)
        {
            if(*end == '.') if(++order == 0) { pos = end; break; }
        }
    }
    else
    {
        pos += std::strlen(pos);
    }
    str.append(name, std::size_t(pos - name));
    str.append(1, '.');
    str.append(ext);
    str.append(pos);
    return str;
}

}
}
