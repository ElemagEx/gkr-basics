#pragma once

#include <gkr/api.h>

namespace gkr
{
namespace sys
{
GKR_CORE_API int get_current_process_id();
GKR_CORE_API int get_current_process_name(char* name, unsigned cch);
}
}
