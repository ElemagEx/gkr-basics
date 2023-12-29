#pragma once

#include <gkr/log/defs/generic.hpp>

#ifdef  LOG_GENERIC_DEF_OLD
#error  You cannot use both "generic_old.hpp" and "generic_new.hpp" - choose only one
#endif
#define LOG_GENERIC_DEF_NEW

#ifndef LOG_USE_C_DEFS

#ifndef __cpp_if_constexpr
#error  C++ constexpr if conditions are NOT available - use "generic_old.hpp" instead
#endif

namespace gkr
{
namespace log
{
template<int severity>
inline void simple_message(void* instance, int facility, const char* msg)
{
    if constexpr(threshold(severity))
    {
        gkr_log_simple_message(instance, severity, facility, msg);
    }
}
template<int severity>
inline void simple_message_if(bool condition, void* instance, int facility, const char* msg)
{
    if constexpr(threshold(severity))
    {
        if(condition)
        {
            gkr_log_simple_message(instance, severity, facility, msg);
        }
    }
}
}
}

template<int severity>
inline void LOG        (int facility, const char* msg) { gkr::log::simple_message<severity            >(LOG_INSTANCE, facility, msg); }

inline void LOGF       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, msg); }
inline void LOGE       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, msg); }
inline void LOGW       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, msg); }
inline void LOGI       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, msg); }
inline void LOGV       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, msg); }
inline void LOGD       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, msg); }
inline void LOGT       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, msg); }

inline void LOG_FATAL  (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, msg); }
inline void LOG_ERROR  (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, msg); }
inline void LOG_WARNING(int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, msg); }
inline void LOG_INFO   (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, msg); }
inline void LOG_VERBOSE(int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, msg); }
inline void LOG_DEBUG  (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, msg); }
inline void LOG_TRACE  (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, msg); }

template<int severity>
inline void LOG_IF         (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<severity            >(condition, LOG_INSTANCE, facility, msg); }

inline void LOGF_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, msg); }
inline void LOGE_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, msg); }
inline void LOGW_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, msg); }
inline void LOGI_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, msg); }
inline void LOGV_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, msg); }
inline void LOGD_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, msg); }
inline void LOGT_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, msg); }

inline void LOG_FATAL_IF   (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_ERROR_IF   (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_WARNING_IF (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_INFO_IF    (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_VERBOSE_IF (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_DEBUG_IF   (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_TRACE_IF   (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, msg); }

template<int severity>
inline void CLOG        (void* instance, int facility, const char* msg) { gkr::log::simple_message<severity            >(instance, facility, msg); }

inline void CLOGF       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(instance, facility, msg); }
inline void CLOGE       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(instance, facility, msg); }
inline void CLOGW       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING>(instance, facility, msg); }
inline void CLOGI       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   >(instance, facility, msg); }
inline void CLOGV       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(instance, facility, msg); }
inline void CLOGD       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(instance, facility, msg); }
inline void CLOGT       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(instance, facility, msg); }

inline void CLOG_FATAL  (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(instance, facility, msg); }
inline void CLOG_ERROR  (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(instance, facility, msg); }
inline void CLOG_WARNING(void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING>(instance, facility, msg); }
inline void CLOG_INFO   (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   >(instance, facility, msg); }
inline void CLOG_VERBOSE(void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(instance, facility, msg); }
inline void CLOG_DEBUG  (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(instance, facility, msg); }
inline void CLOG_TRACE  (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(instance, facility, msg); }

template<int severity>
inline void CLOG_IF         (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<severity            >(condition, instance, facility, msg); }

inline void CLOGF_IF        (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, msg); }
inline void CLOGE_IF        (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, msg); }
inline void CLOGW_IF        (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, msg); }
inline void CLOGI_IF        (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, msg); }
inline void CLOGV_IF        (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, msg); }
inline void CLOGD_IF        (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, msg); }
inline void CLOGT_IF        (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, msg); }

inline void CLOG_FATAL_IF   (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, msg); }
inline void CLOG_ERROR_IF   (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, msg); }
inline void CLOG_WARNING_IF (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, msg); }
inline void CLOG_INFO_IF    (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, msg); }
inline void CLOG_VERBOSE_IF (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, msg); }
inline void CLOG_DEBUG_IF   (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, msg); }
inline void CLOG_TRACE_IF   (bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, msg); }

#ifndef GKR_NO_PRINTF_LOGGING

#include <utility>

namespace gkr
{
namespace log
{
template<int severity, typename... Args>
inline void printf_message(void* instance, int facility, const char* format, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            gkr_log_printf_message(instance, severity, facility, format, std::forward<Args>(args)...);
        }
        else
        {
            gkr_log_simple_message(instance, severity, facility, format);
        }
    }
}
template<int severity, typename... Args>
inline void printf_message_if(bool condition, void* instance, int facility, const char* format, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                gkr_log_printf_message(instance, severity, facility, format, std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                gkr_log_simple_message(instance, severity, facility, format);
            }
        }
    }
}
}
}

template<int severity,
         typename... Args> void LOG_        (int facility, const char* format, Args&&... args) { gkr::log::printf_message<severity            >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void LOGF_       (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGE_       (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGW_       (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGI_       (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGV_       (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGD_       (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGT_       (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_  (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_  (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARNING_(int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_   (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_(int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_  (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_  (int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void LOG_IF_        (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<severity            >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF_       (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF_       (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF_       (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF_       (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF_       (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF_       (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF_       (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF_  (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF_  (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARNING_IF_(bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF_   (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF_(bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF_  (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF_  (bool condition, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void CLOG_        (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<severity            >(instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_       (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_       (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_       (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_WARNING>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_       (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_INFO   >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_       (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_       (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_       (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_  (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_  (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARNING_(void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_WARNING>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_   (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_INFO   >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_(void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_  (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_  (void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(instance, facility, format, std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void CLOG_IF_        (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<severity            >(condition, instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARNING_IF_(bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_IF_   (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_IF_(bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, format, std::forward<Args>(args)...); }

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

namespace gkr
{
namespace log
{
template<int severity>
inline auto stream_message(void* intance, int facility)
{
    if constexpr(threshold(severity))
    {
        return ostream(intance, severity, facility);
    }
    else
    {
        return impl::dummy_ostream();
    }
}
template<int severity>
inline auto stream_message_if(bool condition, void* intance, int facility)
{
    if constexpr(threshold(severity))
    {
        if(condition)
        {
            return ostream(intance, severity, facility);
        }
        else
        {
            return ostream(intance);
        }
    }
}
}
}

template<int severity>
inline auto LOG        (int facility) { return gkr::log::stream_message<severity            >(LOG_INSTANCE, facility); }

inline auto LOGF       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto LOGE       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto LOGW       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto LOGI       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto LOGV       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto LOGD       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto LOGT       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

inline auto LOG_FATAL  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto LOG_ERROR  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto LOG_WARNING(int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto LOG_INFO   (int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto LOG_VERBOSE(int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto LOG_DEBUG  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto LOG_TRACE  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

template<int severity>
inline auto LOG_IF         (bool condition, int facility) { return gkr::log::stream_message_if<severity            >(condition, LOG_INSTANCE, facility); }

inline auto LOGF_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto LOGE_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto LOGW_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto LOGI_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto LOGV_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto LOGD_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto LOGT_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

inline auto LOG_FATAL_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto LOG_ERROR_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto LOG_WARNING_IF (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto LOG_INFO_IF    (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto LOG_VERBOSE_IF (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto LOG_DEBUG_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto LOG_TRACE_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

template<int severity>
inline auto CLOG        (void* instance, int facility) { return gkr::log::stream_message<severity            >(LOG_INSTANCE, facility); }

inline auto CLOGF       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto CLOGE       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto CLOGW       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto CLOGI       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto CLOGV       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto CLOGD       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto CLOGT       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

inline auto CLOG_FATAL  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto CLOG_ERROR  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto CLOG_WARNING(void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto CLOG_INFO   (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto CLOG_VERBOSE(void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto CLOG_DEBUG  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto CLOG_TRACE  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

template<int severity>
inline auto CLOG_IF         (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<severity            >(condition, LOG_INSTANCE, facility); }

inline auto CLOGF_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto CLOGE_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto CLOGW_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto CLOGI_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto CLOGV_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto CLOGD_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto CLOGT_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

inline auto CLOG_FATAL_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto CLOG_ERROR_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto CLOG_WARNING_IF (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto CLOG_INFO_IF    (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto CLOG_VERBOSE_IF (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto CLOG_DEBUG_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto CLOG_TRACE_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

#endif /*ndef GKR_NO_STREAM_LOGGING*/

#ifndef GKR_NO_FORMAT_LOGGING
#ifdef __cpp_lib_format

namespace gkr::log
{
template<int severity, typename... Args>
inline void format_message(void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            gkr_log_format_message(instance, severity, facility, std::move(fmt), std::forward<Args>(args)...);
        }
        else
        {
            gkr_log_simple_message(instance, severity, facility, fmt.get());
        }
    }
}
template<int severity, typename... Args>
inline void format_message(void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            gkr_log_format_message(instance, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
        }
        else
        {
            gkr_log_simple_message(instance, severity, facility, fmt.get());
        }
    }
}
template<int severity, typename... Args>
inline void format_message_if(bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                gkr_log_format_message(instance, severity, facility, std::move(fmt), std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                gkr_log_simple_message(instance, severity, facility, fmt.get());
            }
        }
    }
}
template<int severity, typename... Args>
inline void format_message_if(bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                gkr_log_format_message(instance, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                gkr_log_simple_message(instance, severity, facility, fmt.get());
            }
        }
    }
}
}

template<int severity,
         typename... Args> void LOG        (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<severity            >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF       (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE       (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW       (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI       (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV       (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD       (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT       (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL  (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR  (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARNING(int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO   (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE(int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG  (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE  (int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void LOG        (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<severity            >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARNING(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO   (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void LOG_IF        (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<severity            >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARN_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF(bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void LOG_IF        (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<severity            >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARN_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF(bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void CLOG        (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<severity            >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARNING(void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO   (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE(void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void CLOG        (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<severity            >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARNING(void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO   (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE(void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void CLOG_IF        (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<severity            >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARN_IF   (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_IF   (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_IF(bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void CLOG_IF        (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<severity            >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARN_IF   (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_IF   (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_IF(bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

#endif /*def __cpp_lib_format*/
#endif /*ndef GKR_NO_FORMAT_LOGGING*/

#endif /*ndef LOG_USE_C_DEFS*/
