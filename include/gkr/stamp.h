#pragma once

#include <time.h>

#ifndef GKR_CORE_API
#define GKR_CORE_API
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef long long stamp_t;

GKR_CORE_API stamp_t gkr_stamp_now();

GKR_CORE_API void gkr_stamp_decompose(int local, stamp_t stamp, struct tm* tm, unsigned& ns);

#ifdef __cplusplus
}
#endif
