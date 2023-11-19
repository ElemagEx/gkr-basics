#pragma once

#include <gkr/log/api.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef unsigned short gkr_log_id_t;

GKR_LOG_API int gkr_log_set_this_thread_name(const char* name);

GKR_LOG_API int gkr_log_simple_message(int wait, gkr_log_id_t severity, gkr_log_id_t facility, const char* text);
GKR_LOG_API int gkr_log_printf_message(int wait, gkr_log_id_t severity, gkr_log_id_t facility, const char* format, ...);
GKR_LOG_API int gkr_log_valist_message(int wait, gkr_log_id_t severity, gkr_log_id_t facility, const char* format, va_list args);

#ifdef __cplusplus
}
#endif
