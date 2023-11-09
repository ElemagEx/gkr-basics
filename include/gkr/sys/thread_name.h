#pragma once

#ifndef GKR_SYS_API
#define GKR_SYS_API
#endif

namespace gkr
{
namespace sys
{
constexpr unsigned MAX_THREAD_NAME_CCH = 16;

GKR_SYS_API bool set_current_thread_name(const char name[MAX_THREAD_NAME_CCH]);
GKR_SYS_API bool get_current_thread_name(      char name[MAX_THREAD_NAME_CCH]);
}
}
