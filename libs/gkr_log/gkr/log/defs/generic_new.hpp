#pragma once

#ifndef GENERIC_NEW_HPP
#define GENERIC_NEW_HPP

#ifndef GENERIC_HPP
#if !defined(GENERIC_LOG_USE_C_DEFS) && !defined(GENERIC_LOG_USE_CPP_LEGACY)
#ifndef __cpp_if_constexpr
#error  You must use legacy C++ implementation of log inlines - include "generic_old.hpp"
#endif
#endif
#include <gkr/log/defs/generic.hpp>
#endif

#if !defined(GENERIC_LOG_USE_C_DEFS) && !defined(GENERIC_LOG_USE_CPP_LEGACY)

template<int severity>
inline void _log_simple_msg_cts_uc(void* channel, int facility, const char* msg)
{
    if constexpr(_log_threshold(severity))
    {
        gkr_log_simple_message(channel, severity, facility, msg);
    }
}
template<int severity>
inline void _log_simple_msg_cts_if(bool condition, void* channel, int facility, const char* msg)
{
    if constexpr(_log_threshold(severity))
    {
        if(condition)
        {
            gkr_log_simple_message(channel, severity, facility, msg);
        }
    }
}

#ifndef GENERIC_LOG_SKIP_PRINTF

#include <utility>

template<typename... Args>
inline void _log_printf_msg_rts_uc(void* channel, int severity, int facility, const char* format, Args&&... args)
{
    if constexpr(sizeof...(args) > 0)
    {
        gkr_log_printf_message(channel, severity, facility, format, std::forward<Args>(args)...);
    }
    else
    {
        gkr_log_simple_message(channel, severity, facility, format);
    }
}
template<typename... Args>
inline void _log_printf_msg_rts_if(bool condition, void* channel, int severity, int facility, const char* format, Args&&... args)
{
    if constexpr(sizeof...(args) > 0)
    {
        if(condition)
        {
            gkr_log_printf_message(channel, severity, facility, format, std::forward<Args>(args)...);
        }
    }
    else
    {
        if(condition)
        {
            gkr_log_simple_message(channel, severity, facility, format);
        }
    }
}

template<int severity, typename... Args>
inline void _log_printf_msg_cts_uc(void* channel, int facility, const char* format, Args&&... args)
{
    if constexpr(_log_threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            gkr_log_printf_message(channel, severity, facility, format, std::forward<Args>(args)...);
        }
        else
        {
            gkr_log_simple_message(channel, severity, facility, format);
        }
    }
}
template<int severity, typename... Args>
inline void _log_printf_msg_cts_if(bool condition, void* channel, int facility, const char* format, Args&&... args)
{
    if constexpr(_log_threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                gkr_log_printf_message(channel, severity, facility, format, std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                gkr_log_simple_message(channel, severity, facility, format);
            }
        }
    }
}

#endif /*ndef GENERIC_LOG_SKIP_PRINTF*/

#ifndef GENERIC_LOG_SKIP_STREAM

template<int severity>
inline auto _log_stream_msg_cts_uc(void* intance, int facility)
{
    if constexpr(_log_threshold(severity))
    {
        return ostream(intance, severity, facility);
    }
    else
    {
        return impl::dummy_ostream();
    }
}
template<int severity>
inline auto _log_stream_msg_cts_if(bool condition, void* intance, int facility)
{
    if constexpr(_log_threshold(severity))
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
    else
    {
        return impl::dummy_ostream();
    }
}

#endif /*ndef GENERIC_LOG_SKIP_STREAM*/

#ifndef GENERIC_LOG_SKIP_FORMAT
#ifdef __cpp_lib_format

template<typename... Args>
inline void _log_format_msg_rts_uc(void* channel, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(sizeof...(args) > 0)
    {
        gkr_log_format_message(channel, severity, facility, std::move(fmt), std::forward<Args>(args)...);
    }
    else
    {
        gkr_log_simple_message(channel, severity, facility, fmt.get());
    }
}
template<typename... Args>
inline void _log_format_msg_rts_uc(void* channel, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(sizeof...(args) > 0)
    {
        gkr_log_format_message(channel, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
    }
    else
    {
        gkr_log_simple_message(channel, severity, facility, fmt.get());
    }
}
template<typename... Args>
inline void _log_format_msg_rts_if(bool condition, void* channel, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(sizeof...(args) > 0)
    {
        if(condition)
        {
            gkr_log_format_message(channel, severity, facility, std::move(fmt), std::forward<Args>(args)...);
        }
    }
    else
    {
        if(condition)
        {
            gkr_log_simple_message(channel, severity, facility, fmt.get());
        }
    }
}
template<typename... Args>
inline void _log_format_msg_rts_if(bool condition, void* channel, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(sizeof...(args) > 0)
    {
        if(condition)
        {
            gkr_log_format_message(channel, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
        }
    }
    else
    {
        if(condition)
        {
            gkr_log_simple_message(channel, severity, facility, fmt.get());
        }
    }
}

template<int severity, typename... Args>
inline void _log_format_msg_cts_un(void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(_log_threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            gkr_log_format_message(channel, severity, facility, std::move(fmt), std::forward<Args>(args)...);
        }
        else
        {
            gkr_log_simple_message(channel, severity, facility, fmt.get());
        }
    }
}
template<int severity, typename... Args>
inline void _log_format_msg_cts_un(void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(_log_threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            gkr_log_format_message(channel, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
        }
        else
        {
            gkr_log_simple_message(channel, severity, facility, fmt.get());
        }
    }
}
template<int severity, typename... Args>
inline void _log_format_msg_cts_if(bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(_log_threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                gkr_log_format_message(channel, severity, facility, std::move(fmt), std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                gkr_log_simple_message(channel, severity, facility, fmt.get());
            }
        }
    }
}
template<int severity, typename... Args>
inline void _log_format_msg_cts_if(bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
{
    if constexpr(_log_threshold(severity))
    {
        if constexpr(sizeof...(args) > 0)
        {
            if(condition)
            {
                gkr_log_format_message(channel, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...);
            }
        }
        else
        {
            if(condition)
            {
                gkr_log_simple_message(channel, severity, facility, fmt.get());
            }
        }
    }
}

#endif /*def __cpp_lib_format*/
#endif /*ndef GENERIC_LOG_SKIP_FORMAT*/

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C++ Modern Simple basic LOG - bLOGxxx
//
inline void bLOG(int severity, const char* msg)                    { gkr::log::simple_message_rts(LOG_CHANNEL, severity, LOG_FACILITY, msg); }

inline void bLOGF       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGE       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGW       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGI       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGV       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGD       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGT       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY, msg); }

inline void bLOG_FATAL  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_ERROR  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_WARNING(const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_INFO   (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_VERBOSE(const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_DEBUG  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_TRACE  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY, msg); }

inline void bLOG_IF(bool condition, int severity, const char* msg) { gkr::log::simple_message_rts_if(condition, LOG_CHANNEL, severity, LOG_FACILITY, msg); }

inline void bLOGF_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGE_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGW_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGI_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGV_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGD_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGT_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }

inline void bLOG_FATAL_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_ERROR_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_WARNING_IF(bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_INFO_IF   (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_VERBOSE_IF(bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_DEBUG_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_TRACE_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C++ Modern Simple facility LOG - fLOGxxx
//
inline void fLOG(int severity, int facility, const char* msg)                    { gkr::log::simple_message_rts(LOG_CHANNEL, severity, facility, msg); }

inline void fLOGF       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility, msg); }
inline void fLOGE       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility, msg); }
inline void fLOGW       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility, msg); }
inline void fLOGI       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility, msg); }
inline void fLOGV       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility, msg); }
inline void fLOGD       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility, msg); }
inline void fLOGT       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility, msg); }

inline void fLOG_FATAL  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility, msg); }
inline void fLOG_ERROR  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility, msg); }
inline void fLOG_WARNING(int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility, msg); }
inline void fLOG_INFO   (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility, msg); }
inline void fLOG_VERBOSE(int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility, msg); }
inline void fLOG_DEBUG  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility, msg); }
inline void fLOG_TRACE  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility, msg); }

inline void fLOG_IF(bool condition, int severity, int facility, const char* msg) { gkr::log::simple_message_rts_if(condition, LOG_CHANNEL, severity, facility, msg); }

inline void fLOGF_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGE_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGW_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGI_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGV_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGD_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGT_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility, msg); }

inline void fLOG_FATAL_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_ERROR_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_WARNING_IF(bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_INFO_IF   (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_VERBOSE_IF(bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_DEBUG_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_TRACE_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility, msg); }
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C++ Modern Simple channel LOG - cLOGxxx
//
inline void cLOG(void* channel, int severity, const char* msg)                    { gkr::log::simple_message_rts(channel, severity, LOG_FACILITY, msg); }

inline void cLOGF       (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY, msg); }
inline void cLOGE       (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY, msg); }
inline void cLOGW       (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY, msg); }
inline void cLOGI       (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY, msg); }
inline void cLOGV       (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY, msg); }
inline void cLOGD       (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY, msg); }
inline void cLOGT       (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY, msg); }

inline void cLOG_FATAL  (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY, msg); }
inline void cLOG_ERROR  (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY, msg); }
inline void cLOG_WARNING(void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY, msg); }
inline void cLOG_INFO   (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY, msg); }
inline void cLOG_VERBOSE(void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY, msg); }
inline void cLOG_DEBUG  (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY, msg); }
inline void cLOG_TRACE  (void* channel, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY, msg); }

inline void cLOG_IF(bool condition, void* channel, int severity, const char* msg) { gkr::log::simple_message_rts_if(condition, channel, severity, LOG_FACILITY, msg); }

inline void cLOGF_IF       (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY, msg); }
inline void cLOGE_IF       (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY, msg); }
inline void cLOGW_IF       (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY, msg); }
inline void cLOGI_IF       (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY, msg); }
inline void cLOGV_IF       (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY, msg); }
inline void cLOGD_IF       (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY, msg); }
inline void cLOGT_IF       (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY, msg); }

inline void cLOG_FATAL_IF  (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_ERROR_IF  (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_WARNING_IF(bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_INFO_IF   (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_VERBOSE_IF(bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_DEBUG_IF  (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_TRACE_IF  (bool condition, void* channel, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY, msg); }
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C++ Modern Simple general LOG - gLOGxxx
//
inline void gLOG(void* channel, int severity, int facility, const char* msg)                    { gkr::log::simple_message_rts(channel, severity, facility, msg); }

inline void gLOGF       (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(channel, facility, msg); }
inline void gLOGE       (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(channel, facility, msg); }
inline void gLOGW       (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(channel, facility, msg); }
inline void gLOGI       (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(channel, facility, msg); }
inline void gLOGV       (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(channel, facility, msg); }
inline void gLOGD       (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(channel, facility, msg); }
inline void gLOGT       (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(channel, facility, msg); }

inline void gLOG_FATAL  (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(channel, facility, msg); }
inline void gLOG_ERROR  (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(channel, facility, msg); }
inline void gLOG_WARNING(void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(channel, facility, msg); }
inline void gLOG_INFO   (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(channel, facility, msg); }
inline void gLOG_VERBOSE(void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(channel, facility, msg); }
inline void gLOG_DEBUG  (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(channel, facility, msg); }
inline void gLOG_TRACE  (void* channel, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(channel, facility, msg); }

inline void gLOG_IF(bool condition, void* channel, int severity, int facility, const char* msg) { gkr::log::simple_message_rts_if(condition, channel, severity, facility, msg); }

inline void gLOGF_IF       (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility, msg); }
inline void gLOGE_IF       (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility, msg); }
inline void gLOGW_IF       (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility, msg); }
inline void gLOGI_IF       (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility, msg); }
inline void gLOGV_IF       (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility, msg); }
inline void gLOGD_IF       (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility, msg); }
inline void gLOGT_IF       (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility, msg); }

inline void gLOG_FATAL_IF  (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility, msg); }
inline void gLOG_ERROR_IF  (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility, msg); }
inline void gLOG_WARNING_IF(bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility, msg); }
inline void gLOG_INFO_IF   (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility, msg); }
inline void gLOG_VERBOSE_IF(bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility, msg); }
inline void gLOG_DEBUG_IF  (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility, msg); }
inline void gLOG_TRACE_IF  (bool condition, void* channel, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility, msg); }
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#ifndef GENERIC_LOG_SKIP_PRINTF

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C++ Modern PrintF basic LOG - bLOGxxx
//
template<typename... Args> void bLOG_(int severity, const char* format, Args&&... args)                    { gkr::log::printf_message_rts(LOG_CHANNEL, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOGF_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGE_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGW_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGI_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGV_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGD_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGT_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL_  (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR_  (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARNING_(const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO_   (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE_(const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG_  (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE_  (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOG_IF_(bool condition, int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, LOG_CHANNEL, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOGF_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGE_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGW_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGI_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGV_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGD_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGT_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL_IF_  (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR_IF_  (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARNING_IF_(bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO_IF_   (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE_IF_(bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG_IF_  (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE_IF_  (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY, format, std::forward<Args>(args)...); }
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C++ Modern PrintF facility LOG - fLOGxxx
//
template<typename... Args> void fLOG_(int severity, int facility, const char* format, Args&&... args)                    { gkr::log::printf_message_rts(LOG_CHANNEL, severity, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOGF_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGE_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGW_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGI_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGV_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGD_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGT_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL_  (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR_  (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARNING_(int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO_   (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE_(int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG_  (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE_  (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOG_IF_(bool condition, int severity, int facility, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, LOG_CHANNEL, severity, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOGF_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGE_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGW_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGI_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGV_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGD_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGT_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL_IF_  (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR_IF_  (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARNING_IF_(bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO_IF_   (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE_IF_(bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG_IF_  (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE_IF_  (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility, format, std::forward<Args>(args)...); }
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C++ Modern PrintF channel LOG - cLOGxxx
//
template<typename... Args> void cLOG_(void* channel, int severity, const char* format, Args&&... args)                    { gkr::log::printf_message_rts(channel, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOGF_       (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGE_       (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGW_       (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGI_       (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGV_       (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGD_       (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGT_       (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOG_FATAL_  (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR_  (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARNING_(void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO_   (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE_(void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG_  (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE_  (void* channel, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOG_IF_(bool condition, void* channel, int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, channel, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOGF_IF_       (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGE_IF_       (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGW_IF_       (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGI_IF_       (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGV_IF_       (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGD_IF_       (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGT_IF_       (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOG_FATAL_IF_  (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR_IF_  (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARNING_IF_(bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO_IF_   (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE_IF_(bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG_IF_  (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE_IF_  (bool condition, void* channel, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY, format, std::forward<Args>(args)...); }
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C++ Modern PrintF general LOG - gLOGxxx
//
template<typename... Args> void gLOG_(void* channel, int severity, int facility, const char* format, Args&&... args)                    { gkr::log::printf_message_rts(channel, severity, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOGF_       (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGE_       (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGW_       (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGI_       (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGV_       (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGD_       (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGT_       (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(channel, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOG_FATAL_  (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR_  (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARNING_(void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO_   (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE_(void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG_  (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE_  (void* channel, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(channel, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOG_IF_(bool condition, void* channel, int severity, int facility, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, channel, severity, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOGF_IF_       (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGE_IF_       (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGW_IF_       (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGI_IF_       (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGV_IF_       (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGD_IF_       (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGT_IF_       (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOG_FATAL_IF_  (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR_IF_  (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARNING_IF_(bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO_IF_   (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE_IF_(bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG_IF_  (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE_IF_  (bool condition, void* channel, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility, format, std::forward<Args>(args)...); }
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#endif /*ndef GENERIC_LOG_SKIP_PRINTF*/

#ifndef GENERIC_LOG_SKIP_STREAM

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C++ Modern Stream basic LOG - bLOGxxx
//
inline auto bLOG(int severity)                    { return gkr::log::stream_message_rts(LOG_CHANNEL, severity, LOG_FACILITY); }

inline auto bLOGF       ()                        { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGE       ()                        { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGW       ()                        { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGI       ()                        { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGV       ()                        { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGD       ()                        { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGT       ()                        { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY); }

inline auto bLOG_FATAL  ()                        { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_ERROR  ()                        { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_WARNING()                        { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_INFO   ()                        { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_VERBOSE()                        { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_DEBUG  ()                        { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_TRACE  ()                        { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY); }

inline auto bLOG_IF(bool condition, int severity) { return gkr::log::stream_message_rts_if(condition, LOG_CHANNEL, severity, LOG_FACILITY); }

inline auto bLOGF_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGE_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGW_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGI_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGV_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGD_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGT_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY); }

inline auto bLOG_FATAL_IF  (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_ERROR_IF  (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_WARNING_IF(bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_INFO_IF   (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_VERBOSE_IF(bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_DEBUG_IF  (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_TRACE_IF  (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY); }
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C++ Modern Stream facility LOG - fLOGxxx
//
inline auto fLOG(int severity, int facility)                    { return gkr::log::stream_message_rts(LOG_CHANNEL, severity, facility); }

inline auto fLOGF       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility); }
inline auto fLOGE       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility); }
inline auto fLOGW       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility); }
inline auto fLOGI       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility); }
inline auto fLOGV       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility); }
inline auto fLOGD       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility); }
inline auto fLOGT       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility); }

inline auto fLOG_FATAL  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility); }
inline auto fLOG_ERROR  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility); }
inline auto fLOG_WARNING(int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility); }
inline auto fLOG_INFO   (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility); }
inline auto fLOG_VERBOSE(int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility); }
inline auto fLOG_DEBUG  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility); }
inline auto fLOG_TRACE  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility); }

inline auto fLOG_IF(bool condition, int severity, int facility) { return gkr::log::stream_message_rts_if(condition, LOG_CHANNEL, severity, facility); }

inline auto fLOGF_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility); }
inline auto fLOGE_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility); }
inline auto fLOGW_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility); }
inline auto fLOGI_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility); }
inline auto fLOGV_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility); }
inline auto fLOGD_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility); }
inline auto fLOGT_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility); }

inline auto fLOG_FATAL_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility); }
inline auto fLOG_ERROR_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility); }
inline auto fLOG_WARNING_IF(bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility); }
inline auto fLOG_INFO_IF   (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility); }
inline auto fLOG_VERBOSE_IF(bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility); }
inline auto fLOG_DEBUG_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility); }
inline auto fLOG_TRACE_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility); }
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C++ Modern Stream channel LOG - cLOGxxx
//
inline auto cLOG(void* channel, int severity)                    { return gkr::log::stream_message_rts(channel, severity, LOG_FACILITY); }

inline auto cLOGF       (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY); }
inline auto cLOGE       (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY); }
inline auto cLOGW       (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY); }
inline auto cLOGI       (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY); }
inline auto cLOGV       (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY); }
inline auto cLOGD       (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY); }
inline auto cLOGT       (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY); }

inline auto cLOG_FATAL  (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY); }
inline auto cLOG_ERROR  (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY); }
inline auto cLOG_WARNING(void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY); }
inline auto cLOG_INFO   (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY); }
inline auto cLOG_VERBOSE(void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY); }
inline auto cLOG_DEBUG  (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY); }
inline auto cLOG_TRACE  (void* channel)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY); }

inline auto cLOG_IF(bool condition, void* channel, int severity) { return gkr::log::stream_message_rts_if(condition, channel, severity, LOG_FACILITY); }

inline auto cLOGF_IF       (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY); }
inline auto cLOGE_IF       (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY); }
inline auto cLOGW_IF       (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY); }
inline auto cLOGI_IF       (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY); }
inline auto cLOGV_IF       (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY); }
inline auto cLOGD_IF       (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY); }
inline auto cLOGT_IF       (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY); }

inline auto cLOG_FATAL_IF  (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY); }
inline auto cLOG_ERROR_IF  (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY); }
inline auto cLOG_WARNING_IF(bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY); }
inline auto cLOG_INFO_IF   (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY); }
inline auto cLOG_VERBOSE_IF(bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY); }
inline auto cLOG_DEBUG_IF  (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY); }
inline auto cLOG_TRACE_IF  (bool condition, void* channel)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY); }
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C++ Modern Stream general LOG - gLOGxxx
//
inline auto gLOG(void* channel, int severity, int facility)                    { return gkr::log::stream_message_rts(channel, severity, facility); }

inline auto gLOGF       (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(channel, facility); }
inline auto gLOGE       (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(channel, facility); }
inline auto gLOGW       (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(channel, facility); }
inline auto gLOGI       (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(channel, facility); }
inline auto gLOGV       (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(channel, facility); }
inline auto gLOGD       (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(channel, facility); }
inline auto gLOGT       (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(channel, facility); }

inline auto gLOG_FATAL  (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(channel, facility); }
inline auto gLOG_ERROR  (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(channel, facility); }
inline auto gLOG_WARNING(void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(channel, facility); }
inline auto gLOG_INFO   (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(channel, facility); }
inline auto gLOG_VERBOSE(void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(channel, facility); }
inline auto gLOG_DEBUG  (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(channel, facility); }
inline auto gLOG_TRACE  (void* channel, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(channel, facility); }

inline auto gLOG_IF(bool condition, void* channel, int severity, int facility) { return gkr::log::stream_message_rts_if(condition, channel, severity, facility); }

inline auto gLOGF_IF       (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility); }
inline auto gLOGE_IF       (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility); }
inline auto gLOGW_IF       (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility); }
inline auto gLOGI_IF       (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility); }
inline auto gLOGV_IF       (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility); }
inline auto gLOGD_IF       (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility); }
inline auto gLOGT_IF       (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility); }

inline auto gLOG_FATAL_IF  (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility); }
inline auto gLOG_ERROR_IF  (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility); }
inline auto gLOG_WARNING_IF(bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility); }
inline auto gLOG_INFO_IF   (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility); }
inline auto gLOG_VERBOSE_IF(bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility); }
inline auto gLOG_DEBUG_IF  (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility); }
inline auto gLOG_TRACE_IF  (bool condition, void* channel, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility); }
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#endif /*ndef GENERIC_LOG_SKIP_STREAM*/

#ifndef GENERIC_LOG_SKIP_FORMAT
#ifdef __cpp_lib_format

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C++ Modern Format basic LOG - bLOGxxx
//
template<typename... Args> void bLOG(int severity, std::format_string<Args...> fmt, Args&&... args)                    { gkr::log::format_message_rts(LOG_CHANNEL, severity, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOGF       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGE       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGW       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGI       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGV       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGD       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGT       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL  (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR  (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARNING(std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO   (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE(std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG  (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE  (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_IF(bool condition, int severity, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_CHANNEL, severity, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOGF_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGE_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGW_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGI_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGV_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGD_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGT_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARN_IF   (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO_IF   (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE_IF(bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
//
// plus locale
//
template<typename... Args> void bLOG(int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                    { gkr::log::format_message_rts(LOG_CHANNEL, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOGF       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGE       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGW       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGI       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGV       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGD       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGT       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARNING(const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO   (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE(const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_IF(bool condition, int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_CHANNEL, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOGF_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGE_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGW_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGI_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGV_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGD_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGT_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARN_IF   (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO_IF   (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE_IF(bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C++ Modern Format facility LOG - fLOGxxx
//
template<typename... Args> void fLOG(int severity, int facility, std::format_string<Args...> fmt, Args&&... args)                    { gkr::log::format_message_rts(LOG_CHANNEL, severity, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOGF       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGE       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGW       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGI       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGV       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGD       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGT       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL  (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR  (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARNING(int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO   (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE(int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG  (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE  (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_IF(bool condition, int severity, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_CHANNEL, severity, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOGF_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGE_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGW_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGI_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGV_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGD_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGT_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARN_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE_IF(bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
//
// plus locale
//
template<typename... Args> void fLOG(int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                     { gkr::log::format_message_rts(LOG_CHANNEL, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOGF       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGE       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGW       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGI       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGV       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGD       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGT       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARNING(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO   (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_CHANNEL, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_IF (bool condition, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_CHANNEL, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOGF_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGE_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGW_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGI_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGV_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGD_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGT_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARN_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE_IF(bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_CHANNEL, facility, std::move(fmt), std::forward<Args>(args)...); }
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C++ Modern Format channel LOG - mfLOGxxx
//
template<typename... Args> void cLOG(void* channel, int severity, std::format_string<Args...> fmt, Args&&... args)                    { gkr::log::format_message_rts(channel, severity, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOGF       (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGE       (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGW       (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGI       (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGV       (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGD       (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGT       (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOG_FATAL  (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR  (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARNING(void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO   (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE(void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG  (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE  (void* channel, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOG_IF(bool condition, void* channel, int severity, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, channel, severity, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOGF_IF       (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGE_IF       (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGW_IF       (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGI_IF       (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGV_IF       (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGD_IF       (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGT_IF       (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...)    ; }

template<typename... Args> void cLOG_FATAL_IF  (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR_IF  (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARN_IF   (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO_IF   (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE_IF(bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG_IF  (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE_IF  (bool condition, void* channel, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
//
// plus locale
//
template<typename... Args> void cLOG(void* channel, int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                    { gkr::log::format_message_rts   (           channel, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOGF       (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGE       (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGW       (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGI       (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGV       (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGD       (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGT       (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...);                            }

template<typename... Args> void cLOG_FATAL  (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR  (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARNING(void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO   (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE(void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG  (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE  (void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOG_IF(bool condition, void* channel, int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, channel, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOGF_IF       (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGE_IF       (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGW_IF       (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGI_IF       (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGV_IF       (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGD_IF       (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGT_IF       (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...    ); }

template<typename... Args> void cLOG_FATAL_IF  (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR_IF  (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARN_IF   (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO_IF   (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE_IF(bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG_IF  (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE_IF  (bool condition, void* channel, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, channel, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C++ Modern Format general LOG - gLOGxxx
//
template<typename... Args> void gLOG(void* channel, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)                    { gkr::log::format_message_rts(channel, severity, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOGF       (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGE       (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGW       (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGI       (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGV       (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGD       (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGT       (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOG_FATAL  (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR  (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARNING(void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO   (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE(void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG  (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE  (void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(channel, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOG_IF(bool condition, void* channel, int severity, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, channel, severity, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOGF_IF       (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGE_IF       (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGW_IF       (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGI_IF       (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGV_IF       (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGD_IF       (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGT_IF       (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...)      ; }

template<typename... Args> void gLOG_FATAL_IF  (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR_IF  (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARN_IF   (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO_IF   (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE_IF(bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG_IF  (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE_IF  (bool condition, void* channel, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility, std::move(fmt), std::forward<Args>(args)...); }
//
// plus locale
//
template<typename... Args> void gLOG(void* channel, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                    { gkr::log::format_message_rts   (           channel, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOGF       (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGE       (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGW       (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGI       (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGV       (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGD       (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGT       (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }                                    

template<typename... Args> void gLOG_FATAL  (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR  (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARNING(void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO   (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE(void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG  (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE  (void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOG_IF(bool condition, void* channel, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, channel, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOGF_IF       (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGE_IF       (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGW_IF       (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGI_IF       (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGV_IF       (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGD_IF       (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGT_IF       (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...      ); }

template<typename... Args> void gLOG_FATAL_IF  (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR_IF  (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARN_IF   (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO_IF   (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE_IF(bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG_IF  (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE_IF  (bool condition, void* channel, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, channel, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#endif /*def __cpp_lib_format*/
#endif /*ndef GENERIC_LOG_SKIP_FORMAT*/

#endif /*!defined(GENERIC_LOG_USE_C_DEFS) && !defined(GENERIC_LOG_USE_CPP_LEGACY)*/
#endif /*ndef GENERIC_NEW_HPP*/
