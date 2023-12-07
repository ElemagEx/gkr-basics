#pragma once

#include <gkr/api.h>

namespace gkr
{
namespace sys
{
//TODO:Make all return types unsigned
GKR_INNER_API int get_current_process_id();
GKR_INNER_API int get_current_process_name(char* name, unsigned cch);
GKR_INNER_API int get_current_process_path(char* path, unsigned cch);
}
}
