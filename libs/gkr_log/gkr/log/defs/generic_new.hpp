#pragma once

#include <gkr/log/defs/generic.hpp>

#ifdef  LOG_GENERIC_DEF_OLD
#error  You cannot use both "generic_old.hpp" and "generic_new.hpp" - choose only one
#endif
#define LOG_GENERIC_DEF_NEW

#ifndef LOG_USE_C_DEFS

#ifndef __cpp_if_constexpr
#error  C++ constexpr if conditions are NOT enabled - use "generic_old.hpp" instead
#endif

namespace LOG_NS
{
template<int severity>
inline int simple(void* instance, int facility, const char* message)
{
    if constexpr(threshold(severity))
    {
        return gkr_log_simple_message(instance, severity, facility, message);
    }
    else
    {
        return 0;
    }
}
template<int severity>
inline int simple_if(bool condition, void* instance, int facility, const char* message)
{
    if constexpr(threshold(severity))
    {
        if(condition)
        {
            return gkr_log_simple_message(instance, severity, facility, message);
        }
    }
    else
    {
        return 0;
    }
}
}

template<int severity>
inline int LOG        (int facility, const char* message) { return LOG_NS::simple<severity            >(LOG_INSTANCE, facility, message); }

inline int LOGF       (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, message); }
inline int LOGE       (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, message); }
inline int LOGW       (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, message); }
inline int LOGI       (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, message); }
inline int LOGV       (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, message); }
inline int LOGD       (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, message); }
inline int LOGT       (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, message); }

inline int LOG_FATAL  (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, message); }
inline int LOG_ERROR  (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, message); }
inline int LOG_WARNING(int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, message); }
inline int LOG_INFO   (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, message); }
inline int LOG_VERBOSE(int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, message); }
inline int LOG_DEBUG  (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, message); }
inline int LOG_TRACE  (int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, message); }

template<int severity>
inline int LOG_IF         (bool condition, int facility, const char* message) { return LOG_NS::simple_if<severity            >(condition, LOG_INSTANCE, facility, message); }

inline int LOGF_IF        (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, message); }
inline int LOGE_IF        (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, message); }
inline int LOGW_IF        (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, message); }
inline int LOGI_IF        (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, message); }
inline int LOGV_IF        (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, message); }
inline int LOGD_IF        (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, message); }
inline int LOGT_IF        (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, message); }

inline int LOG_FATAL_IF   (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, message); }
inline int LOG_ERROR_IF   (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, message); }
inline int LOG_WARNING_IF (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, message); }
inline int LOG_INFO_IF    (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, message); }
inline int LOG_VERBOSE_IF (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, message); }
inline int LOG_DEBUG_IF   (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, message); }
inline int LOG_TRACE_IF   (bool condition, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, message); }

template<int severity>
inline int CLOG        (void* instance, int facility, const char* message) { return LOG_NS::simple<severity            >(instance, facility, message); }

inline int CLOGF       (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_FATAL  >(instance, facility, message); }
inline int CLOGE       (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_ERROR  >(instance, facility, message); }
inline int CLOGW       (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_WARNING>(instance, facility, message); }
inline int CLOGI       (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_INFO   >(instance, facility, message); }
inline int CLOGV       (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_VERBOSE>(instance, facility, message); }
inline int CLOGD       (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_DEBUG  >(instance, facility, message); }
inline int CLOGT       (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_TRACE  >(instance, facility, message); }

inline int CLOG_FATAL  (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_FATAL  >(instance, facility, message); }
inline int CLOG_ERROR  (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_ERROR  >(instance, facility, message); }
inline int CLOG_WARNING(void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_WARNING>(instance, facility, message); }
inline int CLOG_INFO   (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_INFO   >(instance, facility, message); }
inline int CLOG_VERBOSE(void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_VERBOSE>(instance, facility, message); }
inline int CLOG_DEBUG  (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_DEBUG  >(instance, facility, message); }
inline int CLOG_TRACE  (void* instance, int facility, const char* message) { return LOG_NS::simple<LOG_SEVERITY_TRACE  >(instance, facility, message); }

template<int severity>
inline int CLOG_IF         (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<severity            >(condition, instance, facility, message); }

inline int CLOGF_IF        (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, message); }
inline int CLOGE_IF        (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, message); }
inline int CLOGW_IF        (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_WARNING>(condition, instance, facility, message); }
inline int CLOGI_IF        (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_INFO   >(condition, instance, facility, message); }
inline int CLOGV_IF        (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, message); }
inline int CLOGD_IF        (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, message); }
inline int CLOGT_IF        (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, message); }

inline int CLOG_FATAL_IF   (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, message); }
inline int CLOG_ERROR_IF   (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, message); }
inline int CLOG_WARNING_IF (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_WARNING>(condition, instance, facility, message); }
inline int CLOG_INFO_IF    (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_INFO   >(condition, instance, facility, message); }
inline int CLOG_VERBOSE_IF (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, message); }
inline int CLOG_DEBUG_IF   (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, message); }
inline int CLOG_TRACE_IF   (bool condition, void* instance, int facility, const char* message) { return LOG_NS::simple_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, message); }

#ifndef GKR_NO_PRINTF_LOGGING

#include <utility>

namespace LOG_NS
{
template<int severity, typename... Args>
inline int printf(void* instance, int facility, const char* format, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            return gkr_log_printf_message(instance, severity, facility, format, std::forward<Args>(args)...);
        }
        else
        {
            return gkr_log_simple_message(instance, severity, facility, format);
        }
    }
    else
    {
        return 0;
    }
}
template<int severity, typename... Args>
inline int printf_if(bool condition, void* instance, int facility, const char* format, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                return gkr_log_printf_message(instance, severity, facility, format, std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                return gkr_log_simple_message(instance, severity, facility, format);
            }
        }
    }
    else
    {
        return 0;
    }
}
}

template<int severity,
         typename... Args> int LOG_        (int facility, const char* format, Args&&... args) { return LOG_NS::printf<severity            >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int LOGF_       (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGE_       (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGW_       (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGI_       (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGV_       (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGD_       (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGT_       (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int LOG_FATAL_  (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_ERROR_  (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_WARNING_(int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_INFO_   (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_VERBOSE_(int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_DEBUG_  (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_TRACE_  (int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<int severity,
         typename... Args> int LOG_IF_        (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<severity            >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int LOGF_IF_       (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGE_IF_       (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGW_IF_       (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGI_IF_       (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGV_IF_       (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGD_IF_       (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGT_IF_       (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int LOG_FATAL_IF_  (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_ERROR_IF_  (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_WARNING_IF_(bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_INFO_IF_   (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_VERBOSE_IF_(bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_DEBUG_IF_  (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_TRACE_IF_  (bool condition, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<int severity,
         typename... Args> int CLOG_        (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<severity            >(instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int CLOGF_       (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_FATAL  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGE_       (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_ERROR  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGW_       (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_WARNING>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGI_       (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_INFO   >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGV_       (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_VERBOSE>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGD_       (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_DEBUG  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGT_       (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_TRACE  >(instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int CLOG_FATAL_  (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_FATAL  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_ERROR_  (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_ERROR  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_WARNING_(void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_WARNING>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_INFO_   (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_INFO   >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_VERBOSE_(void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_VERBOSE>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_DEBUG_  (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_DEBUG  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_TRACE_  (void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf<LOG_SEVERITY_TRACE  >(instance, facility, format, std::forward<Args>(args)...); }

template<int severity,
         typename... Args> int CLOG_IF_        (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<severity            >(condition, instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int CLOGF_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGE_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGW_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_WARNING>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGI_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_INFO   >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGV_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGD_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOGT_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int CLOG_FATAL_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_ERROR_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_WARNING_IF_(bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_WARNING>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_INFO_IF_   (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_INFO   >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_VERBOSE_IF_(bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_DEBUG_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int CLOG_TRACE_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args) { return LOG_NS::printf_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, format, std::forward<Args>(args)...); }

#endif /*!GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

namespace LOG_NS
{
template<int severity>
inline auto stream(void* intance, int facility)
{
    if constexpr(threshold(severity))
    {
        return gkr::log::ostream(intance, severity, facility);
    }
    else
    {
        return gkr::log::impl::dummy_ostream();
    }
}
template<int severity>
inline auto stream_if(bool condition, void* intance, int facility)
{
    if constexpr(threshold(severity))
    {
        if(condition)
        {
            return gkr::log::ostream(intance, severity, facility);
        }
        else
        {
            return gkr::log::ostream(intance);
        }
    }
    else
    {
        return gkr::log::impl::dummy_ostream();
    }
}
}

template<int severity>
inline auto LOG        (int facility) { return LOG_NS::stream<severity            >(LOG_INSTANCE, facility); }

inline auto LOGF       (int facility) { return LOG_NS::stream<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto LOGE       (int facility) { return LOG_NS::stream<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto LOGW       (int facility) { return LOG_NS::stream<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto LOGI       (int facility) { return LOG_NS::stream<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto LOGV       (int facility) { return LOG_NS::stream<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto LOGD       (int facility) { return LOG_NS::stream<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto LOGT       (int facility) { return LOG_NS::stream<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

inline auto LOG_FATAL  (int facility) { return LOG_NS::stream<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto LOG_ERROR  (int facility) { return LOG_NS::stream<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto LOG_WARNING(int facility) { return LOG_NS::stream<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto LOG_INFO   (int facility) { return LOG_NS::stream<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto LOG_VERBOSE(int facility) { return LOG_NS::stream<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto LOG_DEBUG  (int facility) { return LOG_NS::stream<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto LOG_TRACE  (int facility) { return LOG_NS::stream<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

template<int severity>
inline auto LOG_IF         (bool condition, int facility) { return LOG_NS::stream_if<severity            >(condition, LOG_INSTANCE, facility); }

inline auto LOGF_IF        (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto LOGE_IF        (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto LOGW_IF        (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto LOGI_IF        (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto LOGV_IF        (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto LOGD_IF        (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto LOGT_IF        (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

inline auto LOG_FATAL_IF   (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto LOG_ERROR_IF   (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto LOG_WARNING_IF (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto LOG_INFO_IF    (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto LOG_VERBOSE_IF (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto LOG_DEBUG_IF   (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto LOG_TRACE_IF   (bool condition, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

template<int severity>
inline auto CLOG        (void* instance, int facility) { return LOG_NS::stream<severity            >(LOG_INSTANCE, facility); }

inline auto CLOGF       (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto CLOGE       (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto CLOGW       (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto CLOGI       (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto CLOGV       (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto CLOGD       (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto CLOGT       (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

inline auto CLOG_FATAL  (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto CLOG_ERROR  (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto CLOG_WARNING(void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto CLOG_INFO   (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto CLOG_VERBOSE(void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto CLOG_DEBUG  (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto CLOG_TRACE  (void* instance, int facility) { return LOG_NS::stream<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

template<int severity>
inline auto CLOG_IF         (bool condition, void* instance, int facility) { return LOG_NS::stream_if<severity            >(condition, LOG_INSTANCE, facility); }

inline auto CLOGF_IF        (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto CLOGE_IF        (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto CLOGW_IF        (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto CLOGI_IF        (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto CLOGV_IF        (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto CLOGD_IF        (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto CLOGT_IF        (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

inline auto CLOG_FATAL_IF   (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto CLOG_ERROR_IF   (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto CLOG_WARNING_IF (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto CLOG_INFO_IF    (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto CLOG_VERBOSE_IF (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto CLOG_DEBUG_IF   (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto CLOG_TRACE_IF   (bool condition, void* instance, int facility) { return LOG_NS::stream_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

#endif /*!GKR_NO_STREAM_LOGGING*/

#ifndef GKR_NO_FORMAT_LOGGING
#ifdef __cpp_lib_format

namespace LOG_NS
{
template<int severity, typename... Args>
inline int format(void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            return gkr_log_format_message(instance, severity, facility, std::move(fmt), std::forward<Args>(args)...);
        }
        else
        {
            return gkr_log_simple_message(instance, severity, facility, fmt.get());
        }
    }
    else
    {
        return 0;
    }
}
template<int severity, typename... Args>
inline int format(void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            return gkr_log_format_message(instance, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
        }
        else
        {
            return gkr_log_simple_message(instance, severity, facility, fmt.get());
        }
    }
    else
    {
        return 0;
    }
}
template<int severity, typename... Args>
inline int format_if(bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                return gkr_log_format_message(instance, severity, facility, std::move(fmt), std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                return gkr_log_simple_message(instance, severity, facility, fmt.get());
            }
        }
    }
    else
    {
        return 0;
    }
}
template<int severity, typename... Args>
inline int format_if(bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                return gkr_log_format_message(instance, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                return gkr_log_simple_message(instance, severity, facility, fmt.get());
            }
        }
    }
    else
    {
        return 0;
    }
}
}

template<int severity,
         typename... Args> int LOG        (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<severity            >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LOGF       (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGE       (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGW       (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGI       (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGV       (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGD       (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGT       (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LOG_FATAL  (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_ERROR  (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_WARNING(int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_INFO   (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_VERBOSE(int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_DEBUG  (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_TRACE  (int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> int LOG        (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<severity            >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LOGF       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGE       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGW       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGI       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGV       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGD       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGT       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LOG_FATAL  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_ERROR  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_WARNING(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_INFO   (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_VERBOSE(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_DEBUG  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_TRACE  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void LOG_IF        (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<severity            >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARN_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF(bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void LOG_IF        (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<severity            >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARN_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF(bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> int CLOG        (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<severity            >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int CLOGF       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_FATAL  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGE       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_ERROR  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGW       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_WARNING>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGI       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_INFO   >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGV       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_VERBOSE>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGD       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_DEBUG  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGT       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_TRACE  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int CLOG_FATAL  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_FATAL  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOG_ERROR  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_ERROR  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOG_WARNING(void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_WARNING>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOG_INFO   (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_INFO   >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOG_VERBOSE(void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_VERBOSE>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOG_DEBUG  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_DEBUG  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOG_TRACE  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_TRACE  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> int CLOG        (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<severity            >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int CLOGF       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_FATAL  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGE       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_ERROR  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGW       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_WARNING>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGI       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_INFO   >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGV       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_VERBOSE>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGD       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_DEBUG  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int CLOGT       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_TRACE  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LCOG_FATAL  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_FATAL  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LCOG_ERROR  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_ERROR  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LCOG_WARNING(void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_WARNING>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LCOG_INFO   (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_INFO   >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LCOG_VERBOSE(void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_VERBOSE>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LCOG_DEBUG  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_DEBUG  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LCOG_TRACE  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format<LOG_SEVERITY_TRACE  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void CLOG_IF        (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<severity            >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_WARNING>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_INFO   >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARN_IF   (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_WARNING>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_IF   (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_INFO   >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_IF(bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void CLOG_IF        (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<severity            >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_WARNING>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_INFO   >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARN_IF   (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_WARNING>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_IF   (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_INFO   >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_IF(bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return LOG_NS::format_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

#endif /*__cpp_lib_format*/
#endif /*!GKR_NO_FORMAT_LOGGING*/

#endif /*!LOG_USE_C_DEFS*/
