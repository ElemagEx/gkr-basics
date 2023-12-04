#pragma once

#include <gkr/log/api.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API int gkr_log_set_this_thread_name(const char* name);

GKR_LOG_API int gkr_log_simple_message(int wait, int severity, int facility, const char* text);
GKR_LOG_API int gkr_log_printf_message(int wait, int severity, int facility, const char* format, ...);
GKR_LOG_API int gkr_log_valist_message(int wait, int severity, int facility, const char* format, va_list args);

GKR_LOG_API int gkr_log_custom_message_start(char** buf, unsigned* cch);
GKR_LOG_API int gkr_log_custom_message_finish(int wait, int severity, int facility);

#ifdef __cplusplus
}
#endif
