#pragma once

#include <gkr/api.hpp>

#include <string>

namespace gkr
{
namespace sys
{

//
// OS operation-free path operations
//

GKR_INNER_API bool path_is_valid(const char* path);
GKR_INNER_API bool path_is_os_valid(const char* path);
GKR_INNER_API bool path_is_relative(const char* path);

GKR_INNER_API bool path_ends_with_separator(const char* path);

GKR_INNER_API std::string path_get_filename(const char* path);

GKR_INNER_API bool path_set_filename(std::string& path, const char* name);
GKR_INNER_API bool path_add_filename(std::string& path, const char* name);
GKR_INNER_API bool path_del_filename(std::string& path);

GKR_INNER_API bool path_has_extension(const char* path, const char* ext);

GKR_INNER_API std::string path_get_extension(const char* path);

GKR_INNER_API bool path_set_extension(std::string& path, const char* ext);
GKR_INNER_API bool path_add_extension(std::string& path, const char* ext);
GKR_INNER_API bool path_del_extension(std::string& path);

//
// OS dependent path operations
//

GKR_INNER_API bool path_exists(const char* path);
GKR_INNER_API bool path_is_dir(const char* path);

GKR_INNER_API bool path_ensure_dir_exists(const char* path);

//
//
//

inline bool path_is_valid(const std::string& path)
{
    return path_is_valid(path.c_str());
}
inline bool path_is_os_valid(const std::string& path)
{
    return path_is_os_valid(path.c_str());
}
inline bool path_is_relative(const std::string& path)
{
    return path_is_relative(path.c_str());
}

inline bool path_ends_with_separator(const std::string& path)
{
    return path_ends_with_separator(path.c_str());
}

inline std::string path_get_filename(const std::string& path)
{
    return path_get_filename(path.c_str());
}
inline bool path_set_filename(std::string& path, const std::string& name)
{
    return path_set_filename(path, name.c_str());
}
inline bool path_add_filename(std::string& path, const std::string& name)
{
    return path_add_filename(path, name.c_str());
}

inline bool path_has_extension(const char* path, const std::string& ext)
{
    return path_has_extension(path, ext.c_str());
}
inline bool path_has_extension(const std::string& path, const char* ext)
{
    return path_has_extension(path.c_str(), ext);
}
inline bool path_has_extension(const std::string& path, const std::string& ext)
{
    return path_has_extension(path.c_str(), ext.c_str());
}

inline std::string path_get_extension(std::string& path)
{
    return path_get_extension(path.c_str());
}
inline bool path_set_extension(std::string& path, const std::string& ext)
{
    return path_set_extension(path, ext.c_str());
}
inline bool path_add_extension(std::string& path, const std::string& ext)
{
    return path_add_extension(path, ext.c_str());
}

//
//
//

inline bool path_exists(const std::string& path)
{
    return path_exists(path.c_str());
}
inline bool path_is_dir(const std::string& path)
{
    return path_is_dir(path.c_str());
}
inline bool path_ensure_dir_exists(const std::string& path)
{
    return path_ensure_dir_exists(path.c_str());
}

}
}
