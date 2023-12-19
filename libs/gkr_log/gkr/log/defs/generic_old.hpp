#pragma once

#include <gkr/log/defs/generic.hpp>

#ifdef  LOG_GENERIC_DEF_NEW
#error  You cannot use both "generic_old.hpp" and "generic_new.hpp" - choose only one
#endif
#define LOG_GENERIC_DEF_OLD

#ifndef LOG_USE_C_DEFS

namespace gkr
{
namespace log
{
template<int severity, bool>
inline void simple_message(void* instance, int facility, const char* message)
{
    gkr_log_simple_message(instance, severity, facility, message);
}
template<> inline void simple_message<LOG_SEVERITY_FATAL  ,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_ERROR  ,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_WARNING,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_INFO   ,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_VERBOSE,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_DEBUG  ,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_TRACE  ,false>(void*, int, const char*) {}

template<int severity, bool>
inline void simple_message_if(bool condition, void* instance, int facility, const char* message)
{
    if(condition)
    {
        gkr_log_simple_message(instance, severity, facility, message);
    }
}
template<> inline void simple_message_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_WARNING,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int, const char*) {}
}
}

template<int severity>
inline void LOG        (int facility, const char* message) { gkr::log::simple_message<severity            , gkr::log::threshold(severity            )>(LOG_INSTANCE, facility, message); }

inline void LOGF       (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, message); }
inline void LOGE       (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, message); }
inline void LOGW       (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, message); }
inline void LOGI       (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, message); }
inline void LOGV       (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, message); }
inline void LOGD       (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, message); }
inline void LOGT       (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, message); }

inline void LOG_FATAL  (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, message); }
inline void LOG_ERROR  (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, message); }
inline void LOG_WARNING(int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, message); }
inline void LOG_INFO   (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, message); }
inline void LOG_VERBOSE(int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, message); }
inline void LOG_DEBUG  (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, message); }
inline void LOG_TRACE  (int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, message); }

template<int severity>
inline void LOG_IF         (bool condition, int facility, const char* message) { gkr::log::simple_message_if<severity            , gkr::log::threshold(severity            )>(condition, LOG_INSTANCE, facility, message); }

inline void LOGF_IF        (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, message); }
inline void LOGE_IF        (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, message); }
inline void LOGW_IF        (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, message); }
inline void LOGI_IF        (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, message); }
inline void LOGV_IF        (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, message); }
inline void LOGD_IF        (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, message); }
inline void LOGT_IF        (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, message); }

inline void LOG_FATAL_IF   (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, message); }
inline void LOG_ERROR_IF   (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, message); }
inline void LOG_WARNING_IF (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, message); }
inline void LOG_INFO_IF    (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, message); }
inline void LOG_VERBOSE_IF (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, message); }
inline void LOG_DEBUG_IF   (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, message); }
inline void LOG_TRACE_IF   (bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, message); }

template<int severity>
inline void CLOG        (void* instance, int facility, const char* message) { gkr::log::simple_message<severity            , gkr::log::threshold(severity            )>(instance, facility, message); }

inline void CLOGF       (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, message); }
inline void CLOGE       (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, message); }
inline void CLOGW       (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, message); }
inline void CLOGI       (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, message); }
inline void CLOGV       (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, message); }
inline void CLOGD       (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, message); }
inline void CLOGT       (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, message); }

inline void CLOG_FATAL  (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, message); }
inline void CLOG_ERROR  (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, message); }
inline void CLOG_WARNING(void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, message); }
inline void CLOG_INFO   (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, message); }
inline void CLOG_VERBOSE(void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, message); }
inline void CLOG_DEBUG  (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, message); }
inline void CLOG_TRACE  (void* instance, int facility, const char* message) { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, message); }

template<int severity>
inline void CLOG_IF         (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<severity            , gkr::log::threshold(severity            )>(condition, instance, facility, message); }

inline void CLOGF_IF        (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, message); }
inline void CLOGE_IF        (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, message); }
inline void CLOGW_IF        (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, message); }
inline void CLOGI_IF        (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, message); }
inline void CLOGV_IF        (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, message); }
inline void CLOGD_IF        (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, message); }
inline void CLOGT_IF        (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, message); }

inline void CLOG_FATAL_IF   (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, message); }
inline void CLOG_ERROR_IF   (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, message); }
inline void CLOG_WARNING_IF (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, message); }
inline void CLOG_INFO_IF    (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, message); }
inline void CLOG_VERBOSE_IF (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, message); }
inline void CLOG_DEBUG_IF   (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, message); }
inline void CLOG_TRACE_IF   (void* instance, bool condition, int facility, const char* message) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, message); }

#ifndef GKR_NO_PRINTF_LOGGING

namespace gkr
{
namespace log
{
template<int severity, bool>
inline void printf_message(void* instance, int facility, const char* format, va_list args)
{
    gkr_log_valist_message(instance, severity, facility, format, args);
}
template<> void printf_message<LOG_SEVERITY_FATAL  ,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_ERROR  ,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_WARNING,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_INFO   ,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_VERBOSE,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_DEBUG  ,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_TRACE  ,false>(void*, int, const char*, va_list) {}

template<int severity, bool>
inline void printf_message_if(bool condition, void* instance, int facility, const char* format, va_list args)
{
    if(condition)
    {
        gkr_log_valist_message(instance, severity, facility, format, args);
    }
}
template<> inline void printf_message_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_WARNING,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int, const char*, va_list) {}
}
}

template<int severity>
inline void LOG_        (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<severity            , gkr::log::threshold(severity            )>(LOG_INSTANCE, facility, format, args); va_end(args); }

inline void LOGF_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGE_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGW_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGI_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGV_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGD_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGT_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, format, args); va_end(args); }

inline void LOG_FATAL_  (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_ERROR_  (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_WARNING_(int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_INFO_   (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_VERBOSE_(int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_DEBUG_  (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_TRACE_  (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, format, args); va_end(args); }

template<int severity>
inline void LOG_IF_        (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<severity            , gkr::log::threshold(severity            )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }

inline void LOGF_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGE_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGW_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGI_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGV_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGD_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGT_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }

inline void LOG_FATAL_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_ERROR_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_WARNING_IF_(bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_INFO_IF_   (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_VERBOSE_IF_(bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_DEBUG_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_TRACE_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }

template<int severity>
inline void CLOG_        (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<severity            , gkr::log::threshold(severity            )>(instance, facility, format, args); va_end(args); }

inline void CLOGF_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, format, args); va_end(args); }
inline void CLOGE_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, format, args); va_end(args); }
inline void CLOGW_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, format, args); va_end(args); }
inline void CLOGI_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, format, args); va_end(args); }
inline void CLOGV_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, format, args); va_end(args); }
inline void CLOGD_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, format, args); va_end(args); }
inline void CLOGT_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, format, args); va_end(args); }

inline void CLOG_FATAL_  (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, format, args); va_end(args); }
inline void CLOG_ERROR_  (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, format, args); va_end(args); }
inline void CLOG_WARNING_(void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, format, args); va_end(args); }
inline void CLOG_INFO_   (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, format, args); va_end(args); }
inline void CLOG_VERBOSE_(void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, format, args); va_end(args); }
inline void CLOG_DEBUG_  (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, format, args); va_end(args); }
inline void CLOG_TRACE_  (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, format, args); va_end(args); }

template<int severity>
inline void CLOG_IF_        (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<severity            , gkr::log::threshold(severity            )>(condition, instance, facility, format, args); va_end(args); }

inline void CLOGF_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGE_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGW_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGI_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGV_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGD_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGT_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, format, args); va_end(args); }

inline void CLOG_FATAL_IF_  (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_ERROR_IF_  (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_WARNING_IF_(void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_INFO_IF_   (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_VERBOSE_IF_(void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_DEBUG_IF_  (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_TRACE_IF_  (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, format, args); va_end(args); }

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

namespace gkr
{
namespace log
{
template<int severity, bool>
inline auto stream_message(void* instance, int facility)
{
    return ostream(instance, severity, facility);
}
template<> auto stream_message<LOG_SEVERITY_FATAL  ,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_ERROR  ,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_WARNING,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_INFO   ,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_VERBOSE,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_DEBUG  ,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_TRACE  ,false>(void*, int) { return impl::dummy_ostream(); }
template<int severity, bool>
inline auto stream_message_if(bool condition, void* instance, int facility)
{
    if(condition)
    {
        return gkr::log::ostream(instance, severity, facility);
    }
    else
    {
        return gkr::log::ostream(instance);
    }
}
template<> auto stream_message_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_WARNING,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
}
}

template<int severity>
inline auto LOG        (int facility) { return gkr::log::stream_message<severity            , gkr::log::threshold(severity            )>(LOG_INSTANCE, facility); }

inline auto LOGF       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility); }
inline auto LOGE       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility); }
inline auto LOGW       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility); }
inline auto LOGI       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility); }
inline auto LOGV       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility); }
inline auto LOGD       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility); }
inline auto LOGT       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility); }

inline auto LOG_FATAL  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility); }
inline auto LOG_ERROR  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility); }
inline auto LOG_WARNING(int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility); }
inline auto LOG_INFO   (int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility); }
inline auto LOG_VERBOSE(int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility); }
inline auto LOG_DEBUG  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility); }
inline auto LOG_TRACE  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility); }

template<int severity>
inline auto LOG_IF         (bool condition, int facility) { return gkr::log::stream_message_if<severity            , gkr::log::threshold(severity            )>(condition, LOG_INSTANCE, facility); }

inline auto LOGF_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility); }
inline auto LOGE_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility); }
inline auto LOGW_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility); }
inline auto LOGI_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility); }
inline auto LOGV_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility); }
inline auto LOGD_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility); }
inline auto LOGT_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility); }

inline auto LOG_FATAL_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility); }
inline auto LOG_ERROR_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility); }
inline auto LOG_WARNING_IF (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility); }
inline auto LOG_INFO_IF    (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility); }
inline auto LOG_VERBOSE_IF (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility); }
inline auto LOG_DEBUG_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility); }
inline auto LOG_TRACE_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility); }

template<int severity>
inline auto CLOG        (void* instance, int facility) { return gkr::log::stream_message<severity            , gkr::log::threshold(severity            )>(instance, facility); }

inline auto CLOGF       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility); }
inline auto CLOGE       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility); }
inline auto CLOGW       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility); }
inline auto CLOGI       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility); }
inline auto CLOGV       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility); }
inline auto CLOGD       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility); }
inline auto CLOGT       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility); }

inline auto CLOG_FATAL  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility); }
inline auto CLOG_ERROR  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility); }
inline auto CLOG_WARNING(void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility); }
inline auto CLOG_INFO   (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility); }
inline auto CLOG_VERBOSE(void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility); }
inline auto CLOG_DEBUG  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility); }
inline auto CLOG_TRACE  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility); }

template<int severity>
inline auto CLOG_IF         (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<severity            , gkr::log::threshold(severity            )>(condition, instance, facility); }

inline auto CLOGF_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility); }
inline auto CLOGE_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility); }
inline auto CLOGW_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility); }
inline auto CLOGI_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility); }
inline auto CLOGV_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility); }
inline auto CLOGD_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility); }
inline auto CLOGT_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility); }

inline auto CLOG_FATAL_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility); }
inline auto CLOG_ERROR_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility); }
inline auto CLOG_WARNING_IF (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility); }
inline auto CLOG_INFO_IF    (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility); }
inline auto CLOG_VERBOSE_IF (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility); }
inline auto CLOG_DEBUG_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility); }
inline auto CLOG_TRACE_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility); }

#endif /*ndef GKR_NO_STREAM_LOGGING*/

#endif /*ndef LOG_USE_C_DEFS*/
