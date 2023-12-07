#pragma once

#include <gkr/api.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API int gkr_log_set_this_thread_name(const char* name);

GKR_LOG_API int gkr_log_simple_message(int severity, int facility, const char* text);
GKR_LOG_API int gkr_log_printf_message(int severity, int facility, const char* format, ...);
GKR_LOG_API int gkr_log_valist_message(int severity, int facility, const char* format, va_list args);

GKR_LOG_API int gkr_log_simple_message_ex(const char* func, const char* file, unsigned line, int severity, int facility, const char* text);
GKR_LOG_API int gkr_log_printf_message_ex(const char* func, const char* file, unsigned line, int severity, int facility, const char* format, ...);
GKR_LOG_API int gkr_log_valist_message_ex(const char* func, const char* file, unsigned line, int severity, int facility, const char* format, va_list args);

GKR_LOG_API int gkr_log_custom_message_start(char** buf, unsigned* cch);
GKR_LOG_API int gkr_log_custom_message_finish(int severity, int facility);
GKR_LOG_API int gkr_log_custom_message_finish_ex(const char* func, const char* file, unsigned line, int severity, int facility);

#ifdef __cplusplus
}
#endif
