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
GKR_INNER_API bool path_is_relative(const char* path);

GKR_INNER_API bool path_ends_with_separator(const char* path);

GKR_INNER_API std::string path_append_filename(const char* path, const char* name);
GKR_INNER_API std::string path_remove_filename(const char* path, const char* name);

GKR_INNER_API std::string path_change_filename(const char* path, const char* name);
GKR_INNER_API std::string path_obtain_filename(const char* path);

GKR_INNER_API bool path_has_extension(const char* path, const char* ext);

GKR_INNER_API std::string path_add_extension(const char* path, const char* ext);
GKR_INNER_API std::string path_del_extension(const char* path);

GKR_INNER_API std::string path_set_extension(const char* path, const char* ext);
GKR_INNER_API std::string path_get_extension(const char* path);

//
// OS dependent path operations
//

GKR_INNER_API bool path_exists(const char* path);
GKR_INNER_API bool path_is_dir(const char* path);

GKR_INNER_API bool path_ensure_dir_exists(const char* path);

}
}
