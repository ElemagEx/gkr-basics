#pragma once

#include <gkr/capi/log/log.h>

#ifdef _WIN32

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#ifdef __clang__
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wold-style-cast"
#pragma clang diagnostic ignored "-Wzero-as-null-pointer-constant"
#endif
#endif

#ifndef _WINBASE_
__declspec(dllimport) unsigned __stdcall FormatMessageA(
    unsigned flags,
    const void* source,
    unsigned messageId,
    unsigned landId,
    char* buffer,
    unsigned size,
    va_list* args
    );
#endif

inline int gkr_log_win32_format_message(void* channel, long msg_id, int severity, int facility, const char* prefix)
{
    char* buf;
    unsigned cch;
    unsigned len = 0;
    void* ctx = gkr_log_custom_message_start(channel, severity, &buf, &cch);
    if(ctx == NULL) return 0;

    if(prefix != NULL)
    {
        void* arg = NULL;
        *(size_t*)&arg = (size_t)(unsigned long)msg_id;
        //flags:0x00002400 = (FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY)
        len = FormatMessageA(0x00002400, prefix, (unsigned)msg_id, 0, buf, cch, (va_list*)&arg);
        if(len == 0) return gkr_log_custom_message_cancel(ctx, channel);
    }
    //flags:0x000012FF = (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK)
    len = FormatMessageA(0x000012FF, NULL, (unsigned)msg_id, 0, buf + len, (unsigned)(cch - len), NULL);

    if(len == 0) return gkr_log_custom_message_cancel(ctx, channel);

    return gkr_log_custom_message_finish(ctx, channel, severity, facility);
}
inline int gkr_log_win32_format_message_ex(void* channel, long msg_id, const char* func, const char* file, unsigned line, int severity, int facility, const char* prefix)
{
    char* buf;
    unsigned cch;
    unsigned len = 0;
    void* ctx = gkr_log_custom_message_start(channel, severity, &buf, &cch);
    if(ctx == NULL) return 0;

    if(prefix != NULL)
    {
        void* arg = NULL;
        *(size_t*)&arg = (size_t)(unsigned long)msg_id;
        //flags:0x00002400 = (FORMAT_MESSAGE_FROM_STRING | FORMAT_MESSAGE_ARGUMENT_ARRAY)
        len = FormatMessageA(0x00002400, prefix, (unsigned)msg_id, 0, buf, cch, (va_list*)&arg);
        if(len == 0) return gkr_log_custom_message_cancel(ctx, channel);
    }
    //flags:0x000012FF = (FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK)
    len = FormatMessageA(0x000012FF, NULL, (unsigned)msg_id, 0, buf + len, (unsigned)(cch - len), NULL);

    if(len == 0) return gkr_log_custom_message_cancel(ctx, channel);

    return gkr_log_custom_message_finish_ex(ctx, channel, func, file, line, severity, facility);
}
#ifdef __cplusplus
#ifdef __clang__
#pragma clang diagnostic pop
#endif
}
#endif

#endif /*def _WIN32*/
