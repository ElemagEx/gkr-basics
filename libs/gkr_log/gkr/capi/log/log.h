#pragma once

#include <gkr/capi/api.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API int gkr_log_simple_message(void* instance, int severity, int facility, const char* text);
GKR_LOG_API int gkr_log_printf_message(void* instance, int severity, int facility, const char* format, ...);
GKR_LOG_API int gkr_log_valist_message(void* instance, int severity, int facility, const char* format, va_list args);

GKR_LOG_API int gkr_log_simple_message_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility, const char* text);
GKR_LOG_API int gkr_log_printf_message_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility, const char* format, ...);
GKR_LOG_API int gkr_log_valist_message_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility, const char* format, va_list args);

GKR_LOG_API int gkr_log_custom_message_start(void* instance, int severity, char** buf, unsigned* cch);
GKR_LOG_API int gkr_log_custom_message_cancel(void* instance);
GKR_LOG_API int gkr_log_custom_message_finish(void* instance, int severity, int facility);
GKR_LOG_API int gkr_log_custom_message_finish_ex(void* instance, const char* func, const char* file, unsigned line, int severity, int facility);

GKR_LOG_API int gkr_log_set_current_thread_name(const char* name);
GKR_LOG_API int gkr_log_get_current_thread_llm_id(int id);

#ifdef __cplusplus
}
#endif
