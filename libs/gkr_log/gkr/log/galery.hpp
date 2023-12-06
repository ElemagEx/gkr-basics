#pragma once

#include <gkr/log/galery.h>

namespace gkr
{
namespace log
{

using message = gkr_log_message;

inline unsigned format_output(const message& msg, char* buf, unsigned cch, int type, int color_scheme)
{
    return gkr_log_format_output(&msg, buf, cch, type, color_scheme);
}

}
}
