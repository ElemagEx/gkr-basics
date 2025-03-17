#pragma once

#ifndef GKR_LOG_GENERIC_NEW
#define GKR_LOG_GENERIC_NEW

#ifndef GKR_LOG_GENERIC_X
#if !defined(LOG_USE_C_DEFS) && !defined(LOG_USE_CPP_LEGACY)
#ifndef __cpp_if_constexpr
#error  You must use legacy C++ implementation of log inlines - include "generic_old.hpp"
#endif
#endif
#include <gkr/log/defs/generic.hpp>
#endif

#if !defined(LOG_USE_C_DEFS) && !defined(LOG_USE_CPP_LEGACY)

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

#ifndef GKR_NO_PRINTF_LOGGING

#include <utility>

namespace gkr
{
namespace log
{
template<typename... Args>
inline void printf_message_rts(void* instance, int severity, int facility, const char* format, Args&&... args)
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
template<typename... Args>
inline void printf_message_rts_if(bool condition, void* instance, int severity, int facility, const char* format, Args&&... args)
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
    else
    {
        return impl::dummy_ostream();
    }
}
}
}

#endif /*ndef GKR_NO_STREAM_LOGGING*/

#ifndef GKR_NO_FORMAT_LOGGING
#ifdef __cpp_lib_format

namespace gkr::log
{
template<typename... Args>
inline void format_message_rts(void* instance, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)
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
template<typename... Args>
inline void format_message_rts(void* instance, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
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
template<typename... Args>
inline void format_message_rts_if(bool condition, void* instance, int severity, int facility, std::format_string<Args...> fmt, Args&&... args)
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
template<typename... Args>
inline void format_message_rts_if(bool condition, void* instance, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)
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

#endif /*def __cpp_lib_format*/
#endif /*ndef GKR_NO_FORMAT_LOGGING*/

#ifdef GKR_LOG_BASIC_KEEP
//
// C++ Modern Simple basic LOG - bLOGxxx
//
inline void bLOG   (                int severity, const char* msg) { gkr::log::simple_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY, msg); }
inline void bLOG_IF(bool condition, int severity, const char* msg) { gkr::log::simple_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, msg); }

inline void bLOGF       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGE       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGW       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGI       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGV       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGD       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGT       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, msg); }

inline void bLOG_FATAL  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_ERROR  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_WARNING(const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_INFO   (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_VERBOSE(const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_DEBUG  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_TRACE  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, msg); }

inline void bLOGF_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGE_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGW_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGI_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGV_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGD_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGT_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }

inline void bLOG_FATAL_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_ERROR_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_WARNING_IF(bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_INFO_IF   (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_VERBOSE_IF(bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_DEBUG_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_TRACE_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C++ Modern Simple facility LOG - fLOGxxx
//
inline void fLOG   (                int severity, int facility, const char* msg) { gkr::log::simple_message_rts   (           LOG_INSTANCE, severity, facility, msg); }
inline void fLOG_IF(bool condition, int severity, int facility, const char* msg) { gkr::log::simple_message_rts_if(condition, LOG_INSTANCE, severity, facility, msg); }

inline void fLOGF       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, msg); }
inline void fLOGE       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, msg); }
inline void fLOGW       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, msg); }
inline void fLOGI       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, msg); }
inline void fLOGV       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, msg); }
inline void fLOGD       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, msg); }
inline void fLOGT       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, msg); }

inline void fLOG_FATAL  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, msg); }
inline void fLOG_ERROR  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, msg); }
inline void fLOG_WARNING(int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, msg); }
inline void fLOG_INFO   (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, msg); }
inline void fLOG_VERBOSE(int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, msg); }
inline void fLOG_DEBUG  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, msg); }
inline void fLOG_TRACE  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, msg); }

inline void fLOGF_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGE_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGW_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGI_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGV_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGD_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGT_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, msg); }

inline void fLOG_FATAL_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_ERROR_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_WARNING_IF(bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_INFO_IF   (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_VERBOSE_IF(bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_DEBUG_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_TRACE_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, msg); }
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_CHANNEL_KEEP
//
// C++ Modern Simple channel LOG - cLOGxxx
//
inline void cLOG   (                void* instance, int severity, const char* msg) { gkr::log::simple_message_rts   (           instance, severity, LOG_FACILITY, msg); }
inline void cLOG_IF(bool condition, void* instance, int severity, const char* msg) { gkr::log::simple_message_rts_if(condition, instance, severity, LOG_FACILITY, msg); }

inline void cLOGF       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, msg); }
inline void cLOGE       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, msg); }
inline void cLOGW       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, msg); }
inline void cLOGI       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, msg); }
inline void cLOGV       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, msg); }
inline void cLOGD       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, msg); }
inline void cLOGT       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, msg); }

inline void cLOG_FATAL  (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, msg); }
inline void cLOG_ERROR  (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, msg); }
inline void cLOG_WARNING(void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, msg); }
inline void cLOG_INFO   (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, msg); }
inline void cLOG_VERBOSE(void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, msg); }
inline void cLOG_DEBUG  (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, msg); }
inline void cLOG_TRACE  (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, msg); }

inline void cLOGF_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, msg); }
inline void cLOGE_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, msg); }
inline void cLOGW_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, msg); }
inline void cLOGI_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, msg); }
inline void cLOGV_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, msg); }
inline void cLOGD_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, msg); }
inline void cLOGT_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, msg); }

inline void cLOG_FATAL_IF  (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, msg); }
inline void cLOG_ERROR_IF  (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, msg); }
inline void cLOG_WARNING_IF(bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, msg); }
inline void cLOG_INFO_IF   (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, msg); }
inline void cLOG_VERBOSE_IF(bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, msg); }
inline void cLOG_DEBUG_IF  (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, msg); }
inline void cLOG_TRACE_IF  (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, msg); }
#endif /*def GKR_LOG_CHANNEL_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C++ Modern Simple general LOG - gLOGxxx
//
inline void gLOG   (                void* instance, int severity, int facility, const char* msg) { gkr::log::simple_message_rts   (           instance, severity, facility, msg); }
inline void gLOG_IF(bool condition, void* instance, int severity, int facility, const char* msg) { gkr::log::simple_message_rts_if(condition, instance, severity, facility, msg); }

inline void gLOGF       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(instance, facility, msg); }
inline void gLOGE       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(instance, facility, msg); }
inline void gLOGW       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(instance, facility, msg); }
inline void gLOGI       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(instance, facility, msg); }
inline void gLOGV       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(instance, facility, msg); }
inline void gLOGD       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(instance, facility, msg); }
inline void gLOGT       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(instance, facility, msg); }

inline void gLOG_FATAL  (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(instance, facility, msg); }
inline void gLOG_ERROR  (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(instance, facility, msg); }
inline void gLOG_WARNING(void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING>(instance, facility, msg); }
inline void gLOG_INFO   (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   >(instance, facility, msg); }
inline void gLOG_VERBOSE(void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(instance, facility, msg); }
inline void gLOG_DEBUG  (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(instance, facility, msg); }
inline void gLOG_TRACE  (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(instance, facility, msg); }

inline void gLOGF_IF       (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, msg); }
inline void gLOGE_IF       (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, msg); }
inline void gLOGW_IF       (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, msg); }
inline void gLOGI_IF       (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, msg); }
inline void gLOGV_IF       (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, msg); }
inline void gLOGD_IF       (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, msg); }
inline void gLOGT_IF       (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, msg); }

inline void gLOG_FATAL_IF  (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, msg); }
inline void gLOG_ERROR_IF  (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, msg); }
inline void gLOG_WARNING_IF(bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, msg); }
inline void gLOG_INFO_IF   (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, msg); }
inline void gLOG_VERBOSE_IF(bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, msg); }
inline void gLOG_DEBUG_IF  (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, msg); }
inline void gLOG_TRACE_IF  (bool condition, void* instance, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, msg); }
#endif /*def GKR_LOG_GENERAL_KEEP*/

#ifndef GKR_NO_PRINTF_LOGGING

#ifdef GKR_LOG_BASIC_KEEP
//
// C++ Modern PrintF basic LOG - bLOGxxx
//
template<typename... Args> void bLOG_   (                int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_IF_(bool condition, int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOGF_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGE_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGW_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGI_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGV_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGD_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGT_       (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL_  (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR_  (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARNING_(const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO_   (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE_(const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG_  (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE_  (const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOGF_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGE_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGW_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGI_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGV_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGD_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOGT_IF_       (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL_IF_  (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR_IF_  (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARNING_IF_(bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO_IF_   (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE_IF_(bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG_IF_  (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE_IF_  (bool condition, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C++ Modern PrintF facility LOG - fLOGxxx
//
template<typename... Args> void fLOG_   (                int severity, int facility, const char* format, Args&&... args) { gkr::log::printf_message_rts   (           LOG_INSTANCE, severity, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_IF_(bool condition, int severity, int facility, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, LOG_INSTANCE, severity, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOGF_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGE_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGW_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGI_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGV_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGD_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGT_       (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL_  (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR_  (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARNING_(int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO_   (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE_(int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG_  (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE_  (int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOGF_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGE_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGW_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGI_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGV_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGD_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOGT_IF_       (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL_IF_  (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR_IF_  (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARNING_IF_(bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO_IF_   (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE_IF_(bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG_IF_  (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE_IF_  (bool condition, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, format, std::forward<Args>(args)...); }
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_CHANNEL_KEEP
//
// C++ Modern PrintF channel LOG - cLOGxxx
//
template<typename... Args> void cLOG_   (                void* instance, int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts   (           instance, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_IF_(bool condition, void* instance, int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, instance, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOGF_       (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGE_       (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGW_       (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGI_       (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGV_       (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGD_       (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGT_       (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOG_FATAL_  (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR_  (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARNING_(void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO_   (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE_(void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG_  (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE_  (void* instance, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOGF_IF_       (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGE_IF_       (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGW_IF_       (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGI_IF_       (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGV_IF_       (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGD_IF_       (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOGT_IF_       (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void cLOG_FATAL_IF_  (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR_IF_  (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARNING_IF_(bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO_IF_   (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE_IF_(bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG_IF_  (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE_IF_  (bool condition, void* instance, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
#endif /*def GKR_LOG_CHANNEL_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C++ Modern PrintF general LOG - gLOGxxx
//
template<typename... Args> void gLOG_   (                void* instance, int severity, int facility, const char* format, Args&&... args) { gkr::log::printf_message_rts   (           instance, severity, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_IF_(bool condition, void* instance, int severity, int facility, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, instance, severity, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOGF_       (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGE_       (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGW_       (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGI_       (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGV_       (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGD_       (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGT_       (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOG_FATAL_  (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR_  (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARNING_(void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_WARNING>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO_   (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_INFO   >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE_(void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG_  (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE_  (void* instance, int facility, const char* format, Args&&... args)                          { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOGF_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGE_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGW_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGI_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGV_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGD_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOGT_IF_       (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, format, std::forward<Args>(args)...); }

template<typename... Args> void gLOG_FATAL_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARNING_IF_(bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO_IF_   (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE_IF_(bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, format, std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE_IF_  (bool condition, void* instance, int facility, const char* format, Args&&... args)       { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, format, std::forward<Args>(args)...); }
#endif /*def GKR_LOG_GENERAL_KEEP*/

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

#ifdef GKR_LOG_BASIC_KEEP
//
// C++ Modern Stream basic LOG - bLOGxxx
//
inline auto bLOG   (                int severity) { return gkr::log::stream_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY); }
inline auto bLOG_IF(bool condition, int severity) { return gkr::log::stream_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY); }

inline auto bLOGF       ()                        { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGE       ()                        { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGW       ()                        { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGI       ()                        { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGV       ()                        { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGD       ()                        { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGT       ()                        { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY); }

inline auto bLOG_FATAL  ()                        { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_ERROR  ()                        { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_WARNING()                        { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_INFO   ()                        { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_VERBOSE()                        { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_DEBUG  ()                        { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_TRACE  ()                        { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY); }

inline auto bLOGF_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGE_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGW_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGI_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGV_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGD_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGT_IF       (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY); }

inline auto bLOG_FATAL_IF  (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_ERROR_IF  (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_WARNING_IF(bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_INFO_IF   (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_VERBOSE_IF(bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_DEBUG_IF  (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_TRACE_IF  (bool condition)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY); }
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C++ Modern Stream facility LOG - fLOGxxx
//
inline auto fLOG   (                int severity, int facility) { return gkr::log::stream_message_rts   (           LOG_INSTANCE, severity, facility); }
inline auto fLOG_IF(bool condition, int severity, int facility) { return gkr::log::stream_message_rts_if(condition, LOG_INSTANCE, severity, facility); }

inline auto fLOGF       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto fLOGE       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto fLOGW       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto fLOGI       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto fLOGV       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto fLOGD       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto fLOGT       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

inline auto fLOG_FATAL  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility); }
inline auto fLOG_ERROR  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility); }
inline auto fLOG_WARNING(int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility); }
inline auto fLOG_INFO   (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility); }
inline auto fLOG_VERBOSE(int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility); }
inline auto fLOG_DEBUG  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility); }
inline auto fLOG_TRACE  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility); }

inline auto fLOGF_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto fLOGE_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto fLOGW_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto fLOGI_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto fLOGV_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto fLOGD_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto fLOGT_IF       (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }

inline auto fLOG_FATAL_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility); }
inline auto fLOG_ERROR_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility); }
inline auto fLOG_WARNING_IF(bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility); }
inline auto fLOG_INFO_IF   (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility); }
inline auto fLOG_VERBOSE_IF(bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility); }
inline auto fLOG_DEBUG_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility); }
inline auto fLOG_TRACE_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility); }
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_CHANNEL_KEEP
//
// C++ Modern Stream channel LOG - cLOGxxx
//
inline auto cLOG   (                void* instance, int severity) { return gkr::log::stream_message_rts   (           instance, severity, LOG_FACILITY); }
inline auto cLOG_IF(bool condition, void* instance, int severity) { return gkr::log::stream_message_rts_if(condition, instance, severity, LOG_FACILITY); }

inline auto cLOGF       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY); }
inline auto cLOGE       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY); }
inline auto cLOGW       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY); }
inline auto cLOGI       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY); }
inline auto cLOGV       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY); }
inline auto cLOGD       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY); }
inline auto cLOGT       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY); }

inline auto cLOG_FATAL  (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY); }
inline auto cLOG_ERROR  (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY); }
inline auto cLOG_WARNING(void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY); }
inline auto cLOG_INFO   (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY); }
inline auto cLOG_VERBOSE(void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY); }
inline auto cLOG_DEBUG  (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY); }
inline auto cLOG_TRACE  (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY); }

inline auto cLOGF_IF       (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY); }
inline auto cLOGE_IF       (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY); }
inline auto cLOGW_IF       (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY); }
inline auto cLOGI_IF       (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY); }
inline auto cLOGV_IF       (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY); }
inline auto cLOGD_IF       (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY); }
inline auto cLOGT_IF       (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY); }

inline auto cLOG_FATAL_IF  (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY); }
inline auto cLOG_ERROR_IF  (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY); }
inline auto cLOG_WARNING_IF(bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY); }
inline auto cLOG_INFO_IF   (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY); }
inline auto cLOG_VERBOSE_IF(bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY); }
inline auto cLOG_DEBUG_IF  (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY); }
inline auto cLOG_TRACE_IF  (bool condition, void* instance)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY); }
#endif /*def GKR_LOG_CHANNEL_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C++ Modern Stream general LOG - gLOGxxx
//
inline auto gLOG   (                void* instance, int severity, int facility) { return gkr::log::stream_message_rts   (           instance, severity, facility); }
inline auto gLOG_IF(bool condition, void* instance, int severity, int facility) { return gkr::log::stream_message_rts_if(condition, instance, severity, facility); }

inline auto gLOGF       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(instance, facility); }
inline auto gLOGE       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(instance, facility); }
inline auto gLOGW       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(instance, facility); }
inline auto gLOGI       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(instance, facility); }
inline auto gLOGV       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(instance, facility); }
inline auto gLOGD       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(instance, facility); }
inline auto gLOGT       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(instance, facility); }

inline auto gLOG_FATAL  (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(instance, facility); }
inline auto gLOG_ERROR  (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(instance, facility); }
inline auto gLOG_WARNING(void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(instance, facility); }
inline auto gLOG_INFO   (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(instance, facility); }
inline auto gLOG_VERBOSE(void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(instance, facility); }
inline auto gLOG_DEBUG  (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(instance, facility); }
inline auto gLOG_TRACE  (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(instance, facility); }

inline auto gLOGF_IF       (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility); }
inline auto gLOGE_IF       (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility); }
inline auto gLOGW_IF       (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility); }
inline auto gLOGI_IF       (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility); }
inline auto gLOGV_IF       (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility); }
inline auto gLOGD_IF       (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility); }
inline auto gLOGT_IF       (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility); }

inline auto gLOG_FATAL_IF  (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility); }
inline auto gLOG_ERROR_IF  (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility); }
inline auto gLOG_WARNING_IF(bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility); }
inline auto gLOG_INFO_IF   (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility); }
inline auto gLOG_VERBOSE_IF(bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility); }
inline auto gLOG_DEBUG_IF  (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility); }
inline auto gLOG_TRACE_IF  (bool condition, void* instance, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility); }
#endif /*def GKR_LOG_GENERAL_KEEP*/

#endif /*ndef GKR_NO_STREAM_LOGGING*/

#ifndef GKR_NO_FORMAT_LOGGING
#ifdef __cpp_lib_format

#ifdef GKR_LOG_BASIC_KEEP
//
// C++ Modern Format basic LOG - bLOGxxx
//
template<typename... Args> void bLOG   (                int severity, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts  (            LOG_INSTANCE, severity, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_IF(bool condition, int severity, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOGF       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGE       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGW       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGI       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGV       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGD       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGT       (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL  (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR  (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARNING(std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO   (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE(std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG  (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE  (std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOGF_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGE_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGW_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGI_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGV_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGD_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGT_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARN_IF   (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO_IF   (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE_IF(bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
//
// plus locale
//
template<typename... Args> void bLOG   (                int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts  (            LOG_INSTANCE, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_IF(bool condition, int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOGF       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGE       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGW       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGI       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGV       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGD       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGT       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARNING(const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO   (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE(const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOGF_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGE_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGW_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGI_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGV_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGD_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOGT_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void bLOG_FATAL_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_ERROR_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_WARN_IF   (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_INFO_IF   (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_VERBOSE_IF(bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_DEBUG_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void bLOG_TRACE_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C++ Modern Format facility LOG - fLOGxxx
//
template<typename... Args> void fLOG   (                int severity, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts  (            LOG_INSTANCE, severity, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_IF(bool condition, int severity, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_INSTANCE, severity, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOGF       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGE       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGW       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGI       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGV       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGD       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGT       (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL  (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR  (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARNING(int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO   (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE(int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG  (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE  (int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOGF_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGE_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGW_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGI_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGV_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGD_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGT_IF       (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARN_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO_IF   (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE_IF(bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE_IF  (bool condition, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
//
// plus locale
//
template<typename... Args> void fLOG    (                int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts  (            LOG_INSTANCE, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_IF (bool condition, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_INSTANCE, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOGF       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGE       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGW       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGI       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGV       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGD       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGT       (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARNING(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO   (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE(int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE  (int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                           { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOGF_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGE_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGW_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGI_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGV_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGD_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOGT_IF       (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void fLOG_FATAL_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_ERROR_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_WARN_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_INFO_IF   (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_VERBOSE_IF(bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_DEBUG_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void fLOG_TRACE_IF  (bool condition, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)        { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, facility, std::move(fmt), std::forward<Args>(args)...); }
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_CHANNEL_KEEP
//
// C++ Modern Format channel LOG - mfLOGxxx
//
template<typename... Args> void cLOG   (                void* instance, int severity, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts   (           instance, severity, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_IF(bool condition, void* instance, int severity, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, instance, severity, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOGF       (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGE       (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGW       (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGI       (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGV       (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGD       (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGT       (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOG_FATAL  (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR  (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARNING(void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO   (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE(void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG  (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE  (void* instance, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOGF_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGE_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGW_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGI_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGV_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGD_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGT_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...)    ; }

template<typename... Args> void cLOG_FATAL_IF  (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR_IF  (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARN_IF   (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO_IF   (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE_IF(bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG_IF  (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE_IF  (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
//
// plus locale
//
template<typename... Args> void cLOG   (                void* instance, int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts   (           instance, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_IF(bool condition, void* instance, int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, instance, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOGF       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGE       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGW       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGI       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGV       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGD       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGT       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...);                            }

template<typename... Args> void cLOG_FATAL  (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR  (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARNING(void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO   (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE(void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG  (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE  (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void cLOGF_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGE_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGW_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGI_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGV_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGD_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOGT_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...    ); }

template<typename... Args> void cLOG_FATAL_IF  (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_ERROR_IF  (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_WARN_IF   (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_INFO_IF   (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_VERBOSE_IF(bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_DEBUG_IF  (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void cLOG_TRACE_IF  (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
#endif /*def GKR_LOG_CHANNEL_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C++ Modern Format general LOG - gLOGxxx
//
template<typename... Args> void gLOG   (                void* instance, int severity, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts   (           instance, severity, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_IF(bool condition, void* instance, int severity, int facility, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, instance, severity, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOGF       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGE       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGW       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGI       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGV       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGD       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGT       (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOG_FATAL  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARNING(void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO   (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE(void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE  (void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, facility, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOGF_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGE_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGW_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGI_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGV_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGD_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGT_IF       (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...)      ; }

template<typename... Args> void gLOG_FATAL_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARN_IF   (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO_IF   (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE_IF(bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE_IF  (bool condition, void* instance, int facility, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, std::move(fmt), std::forward<Args>(args)...); }
//
// plus locale
//
template<typename... Args> void gLOG   (                void* instance, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts   (           instance, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_IF(bool condition, void* instance, int severity, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, instance, severity, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOGF       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGE       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGW       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGI       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGV       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGD       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGT       (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }                                    

template<typename... Args> void gLOG_FATAL  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARNING(void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO   (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE(void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE  (void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)                          { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void gLOGF_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGE_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGW_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGI_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGV_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGD_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOGT_IF       (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...      ); }

template<typename... Args> void gLOG_FATAL_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_ERROR_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_WARN_IF   (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_INFO_IF   (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_VERBOSE_IF(bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_DEBUG_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void gLOG_TRACE_IF  (bool condition, void* instance, int facility, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args)       { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, facility, loc, std::move(fmt), std::forward<Args>(args)...); }
#endif /*def GKR_LOG_GENERAL_KEEP*/

#endif /*def __cpp_lib_format*/
#endif /*ndef GKR_NO_FORMAT_LOGGING*/

#endif /*!defined(LOG_USE_C_DEFS) && !defined(LOG_USE_CPP_LEGACY)*/
#endif /*ndef GKR_LOG_GENERIC_NEW*/
