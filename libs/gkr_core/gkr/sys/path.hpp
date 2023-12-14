#pragma once

#include <gkr/api.h>

#include <string>

namespace gkr
{
namespace sys
{

GKR_INNER_API bool path_has_extension(const char* path, const char* ext);

GKR_INNER_API std::string path_insert_ext(const char* path, const char* ext, int order);

}
}
