#pragma once

#include <gkr/api.hpp>

#include <string>

namespace gkr
{
namespace sys
{

GKR_INNER_API bool path_ensure_dir_exists(const char* path);

GKR_INNER_API bool path_ends_with_sep(const char* path);

GKR_INNER_API bool path_has_extension(const char* path, const char* ext);

GKR_INNER_API std::string path_insert_ext(const char* path, const char* ext, int order);

}
}
