#pragma once

#include <gkr/log/log.h>

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
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
__declspec(dllimport) int __stdcall lstrlenA(const char*);
__declspec(dllimport) char* __stdcall lstrcpynA(char*, const char*, int);
__declspec(dllimport) void* __stdcall GetModuleHandleA(const char*);
#endif

inline int gkr_log_win32_format_message(void* instance, long msg_id, int severity, int facility, const char* prefix)
{
    char* buf;
    unsigned cch;
    if(!gkr_log_custom_message_start(instance, severity, &buf, &cch)) return 0;

    int len = 0;
    if(prefix != nullptr)
    {
        len = lstrlenA(prefix);
        if((unsigned)len >= cch) return gkr_log_custom_message_cancel(instance);
        lstrcpynA(buf, prefix, len+1);
    }
    const unsigned flags = 0x000012FF; //(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK)
    const unsigned res   = FormatMessageA(flags, nullptr, (unsigned)msg_id, 0, buf + len, cch - len, nullptr);

    if(res == 0) return gkr_log_custom_message_cancel(instance);

    return gkr_log_custom_message_finish(instance, severity, facility);
}
inline int gkr_log_win32_format_message_ex(void* instance, long msg_id, const char* func, const char* file, unsigned line, int severity, int facility, const char* prefix)
{
    char* buf;
    unsigned cch;
    if(!gkr_log_custom_message_start(instance, severity, &buf, &cch)) return 0;

    int len = 0;
    if(prefix != nullptr)
    {
        len = lstrlenA(prefix);
        if((unsigned)len >= cch) return gkr_log_custom_message_cancel(instance);
        lstrcpynA(buf, prefix, len+1);
    }
    const unsigned flags = 0x000012FF; //(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK)
    const unsigned res   = FormatMessageA(flags, nullptr, (unsigned)msg_id, 0, buf + len, cch - len, nullptr);

    if(res == 0) return gkr_log_custom_message_cancel(instance);

    return gkr_log_custom_message_finish_ex(instance, func, file, line, severity, facility);
}

#ifdef __cplusplus
}
#endif
#endif
