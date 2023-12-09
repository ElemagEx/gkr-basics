#pragma once

#include <gkr/log/defs/generic.hpp>

#ifdef  LOG_GENERIC_DEF_OLD
#error  You cannot use both "generic_old.hpp" and "generic_new.hpp" - choose only one
#endif
#define LOG_GENERIC_DEF_NEW

#ifndef GKR_LOG_USE_C_DEFS

#ifndef __cpp_if_constexpr
#error  C++ constexpr if conditions are NOT enabled - use "generic_old.hpp" instead
#endif

namespace GKR_LOG_NS
{
template<int severity>
inline int log_simple(int facility, const char* message)
{
    if constexpr(log_threshold(severity)) {
        return gkr_log_simple_message(severity, facility, message);
    } else {
        return 0;
    }
}
template<int severity>
inline int log_simple_if(bool condition, int facility, const char* message)
{
    if constexpr(log_threshold(severity)) {
        return gkr_log_simple_message(severity, facility, message);
    } else {
        return 0;
    }
}
}

template<int severity>
inline int LOG        (int facility, const char* message) { return GKR_LOG_NS::log_simple<severity            >(facility, message); }

inline int LOGF       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_FATAL  >(facility, message); }
inline int LOGE       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_ERROR  >(facility, message); }
inline int LOGW       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_WARN   >(facility, message); }
inline int LOGI       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_INFO   >(facility, message); }
inline int LOGV       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_VERBOSE>(facility, message); }
inline int LOGD       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_DEBUG  >(facility, message); }
inline int LOGT       (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_TRACE  >(facility, message); }

inline int LOG_FATAL  (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_FATAL  >(facility, message); }
inline int LOG_ERROR  (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_ERROR  >(facility, message); }
inline int LOG_WARN   (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_WARN   >(facility, message); }
inline int LOG_INFO   (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_INFO   >(facility, message); }
inline int LOG_VERBOSE(int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_VERBOSE>(facility, message); }
inline int LOG_DEBUG  (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_DEBUG  >(facility, message); }
inline int LOG_TRACE  (int facility, const char* message) { return GKR_LOG_NS::log_simple<LOG_SEVERITY_TRACE  >(facility, message); }

template<int severity>
inline int LOG_IF         (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<severity            >(condition, facility, message); }

inline int LOGF_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_FATAL  >(condition, facility, message); }
inline int LOGE_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_ERROR  >(condition, facility, message); }
inline int LOGW_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_WARN   >(condition, facility, message); }
inline int LOGI_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_INFO   >(condition, facility, message); }
inline int LOGV_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_VERBOSE>(condition, facility, message); }
inline int LOGD_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_DEBUG  >(condition, facility, message); }
inline int LOGT_IF        (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_TRACE  >(condition, facility, message); }

inline int LOG_FATAL_IF   (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_FATAL  >(condition, facility, message); }
inline int LOG_ERROR_IF   (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_ERROR  >(condition, facility, message); }
inline int LOG_WARN_IF    (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_WARN   >(condition, facility, message); }
inline int LOG_INFO_IF    (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_INFO   >(condition, facility, message); }
inline int LOG_VERBOSE_IF (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_VERBOSE>(condition, facility, message); }
inline int LOG_DEBUG_IF   (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_DEBUG  >(condition, facility, message); }
inline int LOG_TRACE_IF   (bool condition, int facility, const char* message) { return GKR_LOG_NS::log_simple_if<LOG_SEVERITY_TRACE  >(condition, facility, message); }

#ifndef GKR_NO_PRINTF_LOGGING

#include <utility>

namespace GKR_LOG_NS
{
template<int severity, typename... Args>
inline int log_printf(int facility, const char* format, Args&&... args)
{
    if constexpr(log_threshold(severity)) {
        if(sizeof...(args) > 0) {
            return gkr_log_printf_message(severity, facility, format, std::forward<Args>(args)...);
        }
        else {
            return gkr_log_simple_message(severity, facility, format);
        }
    } else {
        return 0;
    }
}
template<int severity, typename... Args>
inline int log_printf_if(bool condition, int facility, const char* format, Args&&... args)
{
    if constexpr(log_threshold(severity)) {
        if(sizeof...(args) > 0) {
            if(condition) {
                return gkr_log_printf_message(severity, facility, format, std::forward<Args>(args)...);
            }
        }
        else {
            if(condition) {
                return gkr_log_simple_message(severity, facility, format);
            }
        }
    }
    return 0;
}
}

template<int severity,
         typename... Args> int LOG_        (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<severity            >(facility, format, std::forward<Args>(args)...); }

template<typename... Args> int LOGF_       (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_FATAL  >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGE_       (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_ERROR  >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGW_       (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_WARN   >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGI_       (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_INFO   >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGV_       (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_VERBOSE>(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGD_       (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_DEBUG  >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGT_       (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_TRACE  >(facility, format, std::forward<Args>(args)...); }

template<typename... Args> int LOG_FATAL_  (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_FATAL  >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_ERROR_  (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_ERROR  >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_WARN_   (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_WARN   >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_INFO_   (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_INFO   >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_VERBOSE_(int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_VERBOSE>(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_DEBUG_  (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_DEBUG  >(facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_TRACE_  (int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf<LOG_SEVERITY_TRACE  >(facility, format, std::forward<Args>(args)...); }

template<int severity,
         typename... Args> int LOG_IF_        (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<severity            >(condition, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int LOGF_IF_       (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_FATAL  >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGE_IF_       (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_ERROR  >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGW_IF_       (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_WARN   >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGI_IF_       (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_INFO   >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGV_IF_       (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_VERBOSE>(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGD_IF_       (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_DEBUG  >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOGT_IF_       (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_TRACE  >(condition, facility, format, std::forward<Args>(args)...); }

template<typename... Args> int LOG_FATAL_IF_  (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_FATAL  >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_ERROR_IF_  (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_ERROR  >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_WARN_IF_   (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_WARN   >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_INFO_IF_   (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_INFO   >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_VERBOSE_IF_(bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_VERBOSE>(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_DEBUG_IF_  (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_DEBUG  >(condition, facility, format, std::forward<Args>(args)...); }
template<typename... Args> int LOG_TRACE_IF_  (bool condition, int facility, const char* format, Args&&... args) { return GKR_LOG_NS::log_printf_if<LOG_SEVERITY_TRACE  >(condition, facility, format, std::forward<Args>(args)...); }

#endif /*GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

namespace GKR_LOG_NS
{
template<int severity>
inline auto log_stream(int facility)
{
    if constexpr(log_threshold(severity)) {
        return gkr::log::ostream(severity, facility);
    } else {
        return gkr::log::impl::dummy_ostream();
    }
}
template<int severity>
inline auto log_stream_if(bool condition, int facility)
{
    if constexpr(log_threshold(severity)) {
        if(condition) {
            return gkr::log::ostream(severity, facility);
        } else {
            return gkr::log::ostream(nullptr);
        }
    } else {
        return gkr::log::impl::dummy_ostream();
    }
}
}

template<int severity>
inline auto LOG        (int facility) { return GKR_LOG_NS::log_stream<severity            >(facility); }

inline auto LOGF       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_FATAL  >(facility); }
inline auto LOGE       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_ERROR  >(facility); }
inline auto LOGW       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_WARN   >(facility); }
inline auto LOGI       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_INFO   >(facility); }
inline auto LOGV       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_VERBOSE>(facility); }
inline auto LOGD       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_DEBUG  >(facility); }
inline auto LOGT       (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_TRACE  >(facility); }

inline auto LOG_FATAL  (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_FATAL  >(facility); }
inline auto LOG_ERROR  (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_ERROR  >(facility); }
inline auto LOG_WARN   (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_WARN   >(facility); }
inline auto LOG_INFO   (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_INFO   >(facility); }
inline auto LOG_VERBOSE(int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_VERBOSE>(facility); }
inline auto LOG_DEBUG  (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_DEBUG  >(facility); }
inline auto LOG_TRACE  (int facility) { return GKR_LOG_NS::log_stream<LOG_SEVERITY_TRACE  >(facility); }

template<int severity>
inline auto LOG_IF         (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<severity            >(condition, facility); }

inline auto LOGF_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_FATAL  >(condition, facility); }
inline auto LOGE_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_ERROR  >(condition, facility); }
inline auto LOGW_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_WARN   >(condition, facility); }
inline auto LOGI_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_INFO   >(condition, facility); }
inline auto LOGV_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_VERBOSE>(condition, facility); }
inline auto LOGD_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_DEBUG  >(condition, facility); }
inline auto LOGT_IF        (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_TRACE  >(condition, facility); }

inline auto LOG_FATAL_IF   (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_FATAL  >(condition, facility); }
inline auto LOG_ERROR_IF   (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_ERROR  >(condition, facility); }
inline auto LOG_WARN_IF    (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_WARN   >(condition, facility); }
inline auto LOG_INFO_IF    (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_INFO   >(condition, facility); }
inline auto LOG_VERBOSE_IF (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_VERBOSE>(condition, facility); }
inline auto LOG_DEBUG_IF   (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_DEBUG  >(condition, facility); }
inline auto LOG_TRACE_IF   (bool condition, int facility) { return GKR_LOG_NS::log_stream_if<LOG_SEVERITY_TRACE  >(condition, facility); }

#endif /*GKR_NO_STREAM_LOGGING*/

#ifndef GKR_NO_FORMAT_LOGGING
#ifdef __cpp_lib_format

namespace GKR_LOG_NS
{
template<int severity, typename... Args>
inline int log_format(int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(log_threshold(severity)) {
        if constexpr(sizeof...(args) > 0) {
            return gkr_log_format_message(severity, facility, std::move(fmt), std::forward<Args>(args)...);
        }
        else {
            return gkr_log_simple_message(severity, facility, fmt.get());
        }
    } else {
        return 0;
    }
}
template<int severity, typename... Args>
inline int log_format(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(log_threshold(severity)) {
        if constexpr(sizeof...(args) > 0) {
            return gkr_log_format_message(severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
        }
        else {
            return gkr_log_simple_message(severity, facility, fmt.get());
        }
    } else {
        return 0;
    }
}
template<int severity, typename... Args>
inline int log_format_if(bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(log_threshold(severity)) {
        if constexpr(sizeof...(args) > 0) {
            if(condition) {
                return gkr_log_format_message(severity, facility, std::move(fmt), std::forward<Args>(args)...);
            }
        }
        else {
            if(condition) {
                return gkr_log_simple_message(severity, facility, fmt.get());
            }
        }
    }
    return 0;
}
template<int severity, typename... Args>
inline int log_format_if(bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(log_threshold(severity)) {
        if constexpr(sizeof...(args) > 0) {
            if(condition) {
                return gkr_log_format_message(severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
            }
        }
        else {
            if(condition) {
                return gkr_log_simple_message(severity, facility, fmt.get());
            }
        }
    }
    return 0;
}
}

template<int severity,
         typename... Args> int LOG        (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<severity            >(facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LOGF       (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_FATAL  >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGE       (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_ERROR  >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGW       (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_WARN   >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGI       (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_INFO   >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGV       (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_VERBOSE>(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGD       (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_DEBUG  >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGT       (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_TRACE  >(facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LOG_FATAL  (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_FATAL  >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_ERROR  (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_ERROR  >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_WARN   (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_WARN   >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_INFO   (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_INFO   >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_VERBOSE(int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_VERBOSE>(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_DEBUG  (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_DEBUG  >(facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_TRACE  (int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_TRACE  >(facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> int LOG        (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<severity            >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LOGF       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_FATAL  >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGE       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_ERROR  >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGW       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_WARN   >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGI       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_INFO   >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGV       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_VERBOSE>(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGD       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_DEBUG  >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOGT       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_TRACE  >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> int LOG_FATAL  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_FATAL  >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_ERROR  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_ERROR  >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_WARN   (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_WARN   >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_INFO   (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_INFO   >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_VERBOSE(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_VERBOSE>(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_DEBUG  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_DEBUG  >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> int LOG_TRACE  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format<LOG_SEVERITY_TRACE  >(facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void LOG_IF        (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<severity            >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_FATAL  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_ERROR  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_WARN   >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_INFO   >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_VERBOSE>(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_DEBUG  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_TRACE  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_FATAL  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_ERROR  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARN_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_WARN   >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_INFO   >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF(bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_VERBOSE>(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_DEBUG  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_TRACE  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }

template<int severity,
         typename... Args> void LOG_IF        (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<severity            >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_FATAL  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_ERROR  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_WARN   >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_INFO   >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_VERBOSE>(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_DEBUG  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_TRACE  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_FATAL  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_ERROR  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARN_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_WARN   >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_INFO   >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF(bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_VERBOSE>(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_DEBUG  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { return GKR_LOG_NS::log_format_if<LOG_SEVERITY_TRACE  >(condition, facility, std::move(fmt), std::forward<Args>(args)...); }

#endif /*__cpp_lib_format*/
#endif /*GKR_NO_FORMAT_LOGGING*/

#endif /*GKR_LOG_USE_C_DEFS*/
