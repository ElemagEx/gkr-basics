#pragma once

#include <gkr/diag/diagnostics.h>

namespace gkr
{
namespace sys
{
constexpr unsigned MAX_THREAD_NAME_CCH = 16;

bool set_current_thread_name(const char name[MAX_THREAD_NAME_CCH]) noexcept(DIAG_NOEXCEPT);
bool get_current_thread_name(      char name[MAX_THREAD_NAME_CCH]) noexcept(DIAG_NOEXCEPT);
}
}
