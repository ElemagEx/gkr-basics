#pragma once

#include <gkr/capi/api.h>

#include <stdarg.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API int gkr_log_simple_message(void* channel, int severity, int facility, const char* text);
GKR_LOG_API int gkr_log_printf_message(void* channel, int severity, int facility, const char* format, ...);
GKR_LOG_API int gkr_log_valist_message(void* channel, int severity, int facility, const char* format, va_list args);

GKR_LOG_API int gkr_log_simple_message_ex(void* channel, const char* func, const char* file, unsigned line, int severity, int facility, const char* text);
GKR_LOG_API int gkr_log_printf_message_ex(void* channel, const char* func, const char* file, unsigned line, int severity, int facility, const char* format, ...);
GKR_LOG_API int gkr_log_valist_message_ex(void* channel, const char* func, const char* file, unsigned line, int severity, int facility, const char* format, va_list args);

GKR_LOG_API void* gkr_log_custom_message_start(void* channel, int severity, char** buf, unsigned* cch);

GKR_LOG_API int gkr_log_custom_message_cancel(void* context, void* channel);
GKR_LOG_API int gkr_log_custom_message_finish(void* context, void* channel, int severity, int facility);
GKR_LOG_API int gkr_log_custom_message_finish_ex(void* context, void* channel, const char* func, const char* file, unsigned line, int severity, int facility);

GKR_LOG_API int gkr_log_set_current_thread_name(const char* name);
GKR_LOG_API int gkr_log_get_current_thread_llm_id(int id);

#ifdef __cplusplus
}
#endif
