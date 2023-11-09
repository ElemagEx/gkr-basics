#pragma once

#ifndef GKR_SYS_API
#define GKR_SYS_API
#endif

namespace gkr
{
namespace sys
{
GKR_SYS_API int get_current_process_id();
GKR_SYS_API int get_current_process_name(char* name, unsigned cch);
}
}
