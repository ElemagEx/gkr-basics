#include <gkr/defs.hpp>
#include <gkr/sys/path.hpp>

#include <gkr/diagnostics.hpp>

#include <cstring>

namespace
{

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

std::size_t get_path_last_filename_pos(const char* path)
{
    const char* ptr;
#ifdef _WIN32
    ptr = std::strrchr(path, '\\'); if(ptr != nullptr) return std::size_t(ptr - path + 1);
#endif
    ptr = std::strrchr(path, '/' ); if(ptr != nullptr) return std::size_t(ptr - path + 1);

    return 0;
}

inline std::size_t get_path_last_filename_pos(const std::string& path)
{
    return get_path_last_filename_pos(path.c_str());
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
inline bool is_valid_path(const std::string& path)
{
    return is_valid_path(path.c_str());
}

bool is_valid_os_path(const char* path)
{
    Assert_NotNullPtr(path);

    return (is_valid_path(path) && (std::strchr(path, '/') == nullptr));
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

bool path_is_os_valid(const char* path)
{
    Check_Arg_NotNull(path, false);

    return is_valid_os_path(path);
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

std::string path_get_filename(const char* path)
{
    Check_Arg_NotNull(path, {});

    Check_Arg_IsValid(is_valid_path(path), {});

    const std::size_t pos = get_path_last_filename_pos(path);

    path += pos;

    return std::string(path);
}

bool path_set_filename(std::string& path, const char* name)
{
    Check_Arg_NotNull(name, false);

    Check_Arg_IsValid(is_valid_path(path), false);
    Check_Arg_IsValid(is_valid_name(name), false);

    const std::size_t pos = get_path_last_filename_pos(path);

    path.erase(pos);
    path.append(name);

    return true;
}

bool path_add_filename(std::string& path, const char* name)
{
    Check_Arg_NotNull(name, false);

    Check_Arg_IsValid(is_valid_path(path), false);
    Check_Arg_IsValid(is_valid_name(name), false);

    if(!is_path_separator(path.back()))
    {
#ifdef _WIN32
        if(path.find('/') == std::string::npos) path.append(1, '\\');
        else                                    path.append(1, '/' );
#else
        path.append(1, '/');
#endif
    }
    path.append(name);

    return true;
}

bool path_del_filename(std::string& path)
{
    Check_Arg_IsValid(is_valid_path(path), false);

    const std::size_t pos = get_path_last_filename_pos(path);

    path.erase(pos);

    if(is_path_separator(path.back())) path.pop_back();

    return true;
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

    Check_Arg_IsValid(is_valid_ext(ext), false);

    if(*ext != '.') ++pos;

    return (0 == std::strcmp(ext, pos));
}

std::string path_get_extension(const char* path)
{
    Check_Arg_NotNull(path, {});

    Check_Arg_IsValid(is_valid_path(path), {});

    const std::size_t pos = get_path_last_filename_pos(path);

    const char* ptr = std::strchr(path + pos, '.');

    std::string ext;

    if(ptr != nullptr)
    {
        ext.append(ptr + 1);
    }
    return ext;
}

bool path_set_extension(std::string& path, const char* ext)
{
    Check_Arg_NotNull(ext, false);

    Check_Arg_IsValid(is_valid_path(path), false);
    Check_Arg_IsValid(is_valid_ext (ext ), false);

    const std::size_t pos = get_path_last_filename_pos(path);

    const std::size_t dot = path.find('.', pos);

    if(dot != std::string::npos)
    {
        path.erase(dot);
    }
    if(*ext != '.') ++ext;

    if(*ext == 0)
    {
        if(path.back() == '.') path.pop_back();
    }
    else
    {
        if(path.back() != '.') path.append(1, '.');

        path.append(ext);
    }
    return true;
}

bool path_add_extension(std::string& path, const char* ext)
{
    Check_Arg_NotNull(ext, false);

    Check_Arg_IsValid(is_valid_path(path), false);
    Check_Arg_IsValid(is_valid_ext (ext ), false);

    if(*ext == '.') ++ext;

    if(*ext == 0)
    {
        if(path.back() == '.') path.pop_back();
    }
    else
    {
        if(path.back() != '.') path.append(1, '.');

        path.append(ext);
    }
    return true;
}

bool path_del_extension(std::string& path)
{
    Check_Arg_IsValid(is_valid_path(path), false);

    const std::size_t pos = get_path_last_filename_pos(path);

    const std::size_t dot = path.find('.', pos);

    if(dot != std::string::npos)
    {
        path.erase(dot);
    }
    return true;
}

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <Psapi.h>

bool path_exists(const char* path)
{
    Check_Arg_NotNull(path, false);

    Check_Arg_IsValid(is_valid_os_path(path), false);

    const DWORD attrs = GetFileAttributesA(path);

    return (attrs != INVALID_FILE_ATTRIBUTES);
}

bool path_is_dir(const char* path)
{
    Check_Arg_NotNull(path, false);

    Check_Arg_IsValid(is_valid_os_path(path), false);

    const DWORD attrs = GetFileAttributesA(path);

    return ((attrs != INVALID_FILE_ATTRIBUTES) && ((attrs & FILE_ATTRIBUTE_DIRECTORY) != 0));
}

bool path_ensure_dir_exists(const char* path)
{
    Check_Arg_NotNull(path, false);

    Check_Arg_IsValid(is_valid_os_path(path), false);

    Check_Arg_IsValid(get_path_absolute_prefix_len(path) == 3, false); // Must start with '<Drive-Letter>:\'

    std::string dir(path);

    for(std::size_t pos = dir.find('\\'); ; ++pos)
    {
        Assert_Check(pos != std::string::npos);

        pos = dir.find('\\', pos);

        if(pos == std::string::npos) break;

        Check_Arg_IsValid(path[pos - 1] != '.', false); // Encountered '.' or '..'

        dir[pos] = 0;
        if(pos > 2)
        {
            const DWORD attrs = GetFileAttributesA(dir.c_str());

            if(attrs == INVALID_FILE_ATTRIBUTES)
            {
                DIAG_VAR(BOOL, res)
                CreateDirectoryA(dir.c_str(), nullptr);
                Check_Sys_Result(res, false);
            }
            else if((attrs & FILE_ATTRIBUTE_DIRECTORY) == 0)
            {
                return false;
            }
        }
        dir[pos] = '\\';
    }
    return true;
}

#else

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

bool path_ensure_dir_exists(const char* path)
{
    Assert_FailureMsg("Not implemented");
    return false;
}

#endif

}
}
