#pragma once

#include <gkr/api.hpp>

namespace gkr
{
namespace sys
{
GKR_INNER_API void file_report_error(int error, unsigned* errors = nullptr);

GKR_INNER_API bool file_is_atty(void* file);
}
}
