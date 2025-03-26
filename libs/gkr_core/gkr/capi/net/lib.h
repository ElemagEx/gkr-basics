#pragma once

#include <gkr/capi/api.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_INNER_API int gkr_net_lib_startup();
GKR_INNER_API int gkr_net_lib_shutdown();

GKR_INNER_API int gkr_net_lib_get_hostname(char* buf, int cb);

#ifdef __cplusplus
}
#endif
