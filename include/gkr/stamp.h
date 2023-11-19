#pragma once

#include <gkr/api.h>

#include <time.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef long long stamp_t;

GKR_CORE_API stamp_t gkr_stamp_now();

GKR_CORE_API int gkr_stamp_decompose(int local, stamp_t stamp, struct tm* tm);

#ifdef __cplusplus
}

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

#endif
