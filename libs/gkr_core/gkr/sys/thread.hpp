#pragma once

#include <gkr/api.h>

namespace gkr
{
namespace sys
{
constexpr unsigned MAX_THREAD_NAME_CCH = 16;

GKR_INNER_API bool set_current_thread_name(const char name[MAX_THREAD_NAME_CCH]);
GKR_INNER_API bool get_current_thread_name(      char name[MAX_THREAD_NAME_CCH]);
}
}
