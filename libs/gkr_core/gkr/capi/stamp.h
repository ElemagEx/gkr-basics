#pragma once

#include <gkr/capi/api.h>

#include <time.h>

typedef long long stamp_t;

#ifdef __cplusplus
extern "C" {
#endif

GKR_CORE_API stamp_t gkr_stamp_now();

GKR_CORE_API int gkr_stamp_decompose(int local, stamp_t stamp, struct tm* tm);

#ifdef __cplusplus
}
#endif
