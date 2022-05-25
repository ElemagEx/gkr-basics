#pragma once

namespace gkr
{
namespace sys
{
constexpr unsigned max_thread_name_cch = 16;

bool set_current_thread_name(const char name[max_thread_name_cch]);
}
}
