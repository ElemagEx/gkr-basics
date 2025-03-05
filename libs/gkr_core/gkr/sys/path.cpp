#include <gkr/defs.hpp>
#include <gkr/sys/path.hpp>

#include <gkr/diagnostics.hpp>

#include <cstring>

namespace
{

#ifdef _WIN32
constexpr char OS_SEPARATOR = '\\';
#else
constexpr char OS_SEPARATOR = '\\';
#endif

const char* get_path_last_filename(const char* path)
{
    Assert_Check(path != nullptr);

    const char* ptr;
#ifdef _WIN32
    ptr = std::strrchr(path, '\\'); if(ptr != nullptr) return (ptr + 1);
#endif
    ptr = std::strrchr(path, '/' ); if(ptr != nullptr) return (ptr + 1);

    return path;
}

bool is_path_separator(char ch)
{
#ifdef _WIN32
    if(ch == '\\') return true;
#endif
    return (ch == '/');
}

std::size_t get_path_absolute_prefix_len(const char* path)
{
    Assert_NotNullPtr(path);

#ifdef _WIN32
    if(std::strlen(path) >= 2)
    {
        if((path[0] == '\\') && (path[1] == '\\'))
        {
            Assert_FailureMsg("UNC path is not supported yet");
        }
        if(std::isalpha(path[0]) && (path[1] == ':'))
        {
            if(is_path_separator(path[2]))
            {
                return 3;
            }
            else
            {
                return 2;
            }
        }
    }
#endif
    if(is_path_separator(*path))
    {
        return 1;
    }
    return 0;
}

bool is_valid_name(const char* path, std::size_t* len = nullptr)
{
    Assert_NotNullPtr(path);

    for(const char* start = path; ; ++path)
    {
        switch(*path)
        {
            case 0:
                if(len != nullptr)
                {
                    *len = std::size_t(path - start);
                }
                return ((path - start) > 0);

            case '/':
#ifdef _WIN32
            case '\\':
#endif
                if(len != nullptr)
                {
                    *len = std::size_t(path - start + 1);
                    return (*len > 1);
                }
                break;

            case '.':
                if(path[1] != '.') continue;

                if(start == path)
                {
                    ++path;
                    if(path[1] == 0) continue;
                    if(is_path_separator(path[1]) && (len != nullptr)) continue;
                }
                break;

#ifdef _WIN32
            default:
                if(std::strchr("<>:|?*", *path) == nullptr) continue;
                break;
#else
            case '-':
            case '_':
                continue;
            default:
                if(std::isalnum(*path)) continue;
                break;
#endif
        }
        break;
    }
    return false;
}

bool is_valid_path(const char* path)
{
    Assert_NotNullPtr(path);

    path += get_path_absolute_prefix_len(path);

    for(std::size_t len; *path != 0; )
    {
        if(!is_valid_name(path, &len)) return false;

        path += len;
    }
    return true;
}

bool is_valid_ext(const char* ext)
{
    Assert_NotNullPtr(ext);

    return (is_valid_name(ext) && std::strcmp(ext, ".."));
}
}

namespace gkr
{
namespace sys
{

bool path_is_valid(const char* path)
{
    Check_Arg_NotNull(path, false);

    return is_valid_path(path);
}

bool path_is_relative(const char* path)
{
    Check_Arg_NotNull(path, false);

    Check_Arg_IsValid(is_valid_path(path), {});

    const std::size_t prefix_len = get_path_absolute_prefix_len(path);

#ifdef _WIN32
    return (prefix_len > 2);
#else
    return (prefix_len > 0);
#endif
}

bool path_ends_with_separator(const char* path)
{
    Check_Arg_NotNull(path, false);

    Check_Arg_IsValid(is_valid_path(path), {});

    const std::size_t len = std::strlen(path);

    return ((len > 0) && is_path_separator(path[len - 1]));
}

std::string path_append_filename(const char* path, const char* name)
{
    Check_Arg_NotNull(path, {});
    Check_Arg_NotNull(name, {});

    Check_Arg_IsValid(is_valid_path(path), {});
    Check_Arg_IsValid(is_valid_name(name), {});

    std::string str(path);

    if(is_path_separator(str.back())) str.append(1, OS_SEPARATOR);

    str.append(name);

    return str;
}

std::string path_remove_filename(const char* path, const char* name)
{
    Check_Arg_NotNull(path, {});
    Check_Arg_NotNull(name, {});

    Check_Arg_IsValid(is_valid_path(path), {});
    Check_Arg_IsValid(is_valid_name(name), {});

    const char* last = get_path_last_filename(path);

    std::string str(path, std::size_t(last - path));

    if(str.back() == OS_SEPARATOR) str.pop_back();

    return str;
}

std::string path_change_filename(const char* path, const char* name)
{
    Check_Arg_NotNull(path, {});
    Check_Arg_NotNull(name, {});

    Check_Arg_IsValid(is_valid_path(path), {});
    Check_Arg_IsValid(is_valid_name(name), {});

    const char* last = get_path_last_filename(path);

    std::string str(path, std::size_t(last - path));

    str += name;

    return str;
}

std::string path_obtain_filename(const char* path)
{
    Check_Arg_NotNull(path, {});

    Check_Arg_IsValid(is_valid_path(path), {});

    const char* last = get_path_last_filename(path);

    std::string str(last);

    return str;
}

bool path_has_extension(const char* path, const char* ext)
{
    Check_Arg_NotNull(path, false);

    Check_Arg_IsValid(is_valid_path(path), {});

    const char* last = get_path_last_filename(path);

    const char* pos = std::strchr(last, '.');

    if(pos == nullptr)
    {
        return false;
    }
    if(ext == nullptr)
    {
        return (pos[1] != 0);
    }

    Check_Arg_IsValid(is_valid_ext(ext));

    if(*ext == '.') ++ext; else ++pos;

    return (0 == std::strcmp(ext, pos));
}

std::string path_add_extension(const char* path, const char* ext)
{
    Check_Arg_NotNull(path, {});
    Check_Arg_NotNull(ext , {});

    Check_Arg_IsValid(is_valid_path(path), {});
    Check_Arg_IsValid(is_valid_ext (ext ), {});

    std::string str(path);

    if(str.back() != '.') str.append(1, '.');

    if(*ext == '.') ++ext;

    str.append(ext);

    return str;
}

std::string path_del_extension(const char* path)
{
    Check_Arg_NotNull(path, {});

    Check_Arg_IsValid(is_valid_path(path), {});

    const char* last = get_path_last_filename(path);

    std::string str(path, std::size_t(last - path));

    const char* pos = std::strchr(last, '.');

    if(pos == nullptr)
    {
        str.append(last);
    }
    else
    {
        str.append(last, std::size_t(pos - last));
    }
    return str;
}

std::string path_set_extension(const char* path, const char* ext)
{
    Check_Arg_NotNull(path, {});
    Check_Arg_NotNull(ext , {});

    Check_Arg_IsValid(is_valid_path(path), {});
    Check_Arg_IsValid(is_valid_ext (ext ), {});

    const char* last = get_path_last_filename(path);

    std::string str(path, std::size_t(last - path));

    const char* pos = std::strchr(last, '.');

    if(pos == nullptr)
    {
        str.append(last);
    }
    else
    {
        str.append(last, std::size_t(pos - last));
    }

    if(*ext != '.') str.append(1, '.');

    str.append(ext);

    return str;
}

std::string path_get_extension(const char* path)
{
    Check_Arg_NotNull(path, {});

    Check_Arg_IsValid(is_valid_path(path), {});

    const char* last = get_path_last_filename(path);

    const char* pos = std::strchr(last, '.');

    std::string ext;

    if(pos != nullptr)
    {
        ext.append(pos + 1);
    }
    return ext;
}

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

bool path_exists(const char* path)
{
    Check_Arg_NotNull(path, false);

    const DWORD attrs = GetFileAttributesA(path);

    return (attrs != INVALID_FILE_ATTRIBUTES);
}

bool path_is_dir(const char* path)
{
    Check_Arg_NotNull(path, false);

    const DWORD attrs = GetFileAttributesA(path);

    return ((attrs != INVALID_FILE_ATTRIBUTES) && ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0));
}

bool path_ensure_dir_exists(const char* path)
{
    Assert_Check(path != nullptr);

    if(std::strlen(path) >= 4)
    {
        if(!std::strncmp(path + 0, "\\\\?\\", 4)) return false;
        if( std::strncmp(path + 1, ":\\"    , 2)) return false;

        if(!std::isalpha(*path)) return false;
    }
    for(const char* pos = std::strchr(path, '\\'); ; )
    {
        pos = std::strchr(pos + 1, '\\');

        if(pos == nullptr) break;

        std::string dir(path, std::size_t(pos - path));

        CreateDirectoryA(dir.c_str(), nullptr);
    }
    return true;
}

#else

bool path_ensure_dir_exists(const char* path)
{
    Assert_FailureMsg("Not implemented");
    return false;
}

bool path_exists(const char* path)
{
    Assert_FailureMsg("Not implemented");
    return false;
}

bool path_is_dir(const char* path)
{
    Assert_FailureMsg("Not implemented");
    return false;
}

#endif

}
}
