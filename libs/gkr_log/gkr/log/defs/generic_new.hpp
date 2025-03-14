#pragma once

#ifndef GKR_LOG_GENERIC_X
#error  You cannot include this header directly - ensure LOG_CPP_LEGACY is not defined (C++ if constexpr must be available) and then include "generic.hpp"
#endif

#ifdef  LOG_USE_C_DEFS
#error  This should not be defined
#endif

#ifndef __cpp_if_constexpr
#error  C++ constexpr if conditions are NOT available
#endif

namespace gkr
{
namespace log
{
inline void simple_message_rts(void* instance, int severity, int facility, const char* msg)
{
    gkr_log_simple_message(instance, severity, facility, msg);
}
inline void simple_message_rts_if(bool condition, void* instance, int severity, int facility, const char* msg)
{
    if(condition)
    {
        gkr_log_simple_message(instance, severity, facility, msg);
    }
}
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
inline auto stream_message_rts(void* intance, int severity, int facility)
{
    return ostream(intance, severity, facility);
}
inline auto stream_message_rts_if(bool condition, void* intance, int severity, int facility)
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

#ifdef LOG_FACILITY
#include <gkr/log/defs/generic_new_basic.hpp>
#else
#include <gkr/log/defs/generic_new_extra.hpp>
#endif
