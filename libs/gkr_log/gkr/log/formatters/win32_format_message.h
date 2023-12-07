#pragma once

#include <gkr/log/log.h>

#ifdef _WIN32
#ifdef __cplusplus
extern "C" {
#endif
#ifndef _WINBASE_
__declspec(dllimport) unsigned __stdcall FormatMessageA(
    unsigned flags,
    void* source,
    unsigned messageId,
    unsigned landId,
    char* buffer,
    unsigned size,
    va_list* args
    );
__declspec(dllimport) int __stdcall lstrlenA(const char*);
__declspec(dllimport) char* __stdcall lstrcpynA(char*, const char*, int);
#endif

inline int gkr_log_win32_format_message(long messageId, int severity, int facility, const char* prefix)
{
    char* buf;
    unsigned cch;
    if(!gkr_log_custom_message_start(severity, &buf, &cch)) return 0;

    int len = 0;
    if(prefix != nullptr) {
        len = lstrlenA(prefix);
        if((unsigned)len >= cch) return gkr_log_custom_message_cancel();
        lstrcpynA(buf, prefix, len+1);
    }
    const unsigned flags = 0x000012FF; //(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK)
    const unsigned res   = FormatMessageA(flags, nullptr, (unsigned)messageId, 0, buf + len, cch - len, nullptr);

    if(res > 0) {
        return gkr_log_custom_message_finish(severity, facility);
    } else {
        return gkr_log_custom_message_cancel();
    }
}

#ifdef __cplusplus
}
#endif
#endif
