#pragma once

#include <gkr/api.hpp>

#include <string>

namespace gkr
{
namespace sys
{
GKR_INNER_API unsigned get_current_process_id();

GKR_INNER_API std::string get_current_process_name();
GKR_INNER_API std::string get_current_process_path();

GKR_INNER_API std::string expand_process_env_vars(const char* str, bool nativeFormat);
}
}
