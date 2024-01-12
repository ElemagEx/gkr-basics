#pragma once

#define DIAG_SRC_INFO   2 /*DIAG_SRC_INFO_PREPROCESSOR_EX*/

#ifndef DIAG_MODE
#ifndef NDEBUG
#define DIAG_MODE       4 /*DIAG_MODE_INTRUSIVE*/
#else
#define DIAG_MODE       2 /*DIAG_MODE_STEADY*/
#endif
#endif

#define DIAG_EXTERNAL_API
#define DIAG_HALT       gkr_diag_halt
#define DIAG_WARN       gkr_diag_warn

#include <gkr/api.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef void (*GKR_DIAG_REPORT_FUNC)(int, const char*, const char*, const char*, int);

GKR_CORE_API GKR_DIAG_REPORT_FUNC gkr_diag_hook_report_func(GKR_DIAG_REPORT_FUNC fn);

GKR_NO_RETURN_API
GKR_CORE_API void gkr_diag_halt(int id, const char* text, const char* func, const char* file, int line);
GKR_CORE_API int  gkr_diag_warn(int id, const char* text, const char* func, const char* file, int line);

#ifdef __cplusplus
}
#endif
