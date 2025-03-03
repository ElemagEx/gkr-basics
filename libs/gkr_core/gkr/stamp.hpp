#pragma once

#include <gkr/capi/stamp.h>

namespace gkr
{

inline stamp_t stamp_now()
{
    return gkr_stamp_now();
}

inline int stamp_decompose(bool local, stamp_t stamp, struct tm& tm)
{
    return gkr_stamp_decompose(local, stamp, &tm);
}

}
