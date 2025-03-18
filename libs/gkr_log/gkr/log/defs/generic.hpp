#pragma once

#ifndef GENERIC_HPP
#define GENERIC_HPP

#include <gkr/capi/log/defs/generic.h>

constexpr const char* log_severities_names[] = LOG_SEVERITIES_NAMES;

inline constexpr const char* log_severity_name_by_value(int severity)
{
    return (unsigned(severity) < (sizeof(log_severities_names)/sizeof(*log_severities_names)))
        ? log_severities_names[severity]
        : "N/A"
        ;
}

#ifndef GENERIC_LOG_USE_C_DEFS

#ifdef LOG_SOURCE_LOCATION
#error Log source location is available only for C macros - define GENERIC_LOG_USE_C_DEFS or include "generic_cdefs.hpp" to use them
#endif

#include <gkr/log/log.hpp>

#ifndef GENERIC_LOG_USE_CPP_LEGACY
#ifndef __cpp_if_constexpr
#define GENERIC_LOG_USE_CPP_LEGACY
#endif
#endif

#ifndef GENERIC_LOG_SKIP_STREAM
#define LOG_FINISH gkr::log::finish
#endif

inline constexpr bool _log_threshold(int severity)
{
    return (severity < LOG_THRESHOLD_LEVEL);
}

inline void _log_simple_msg_rts_uc(void* channel, int severity, int facility, const char* msg)
{
    gkr_log_simple_message(channel, severity, facility, msg);
}
inline void _log_simple_msg_rts_if(bool condition, void* channel, int severity, int facility, const char* msg)
{
    if(condition)
    {
        gkr_log_simple_message(channel, severity, facility, msg);
    }
}
#ifndef GENERIC_LOG_SKIP_STREAM

inline auto _log_stream_message_rts_uc(void* channel, int severity, int facility)
{
    return gkr::log::ostream(channel, severity, facility);
}
inline auto _log_stream_message_rts_if(bool condition, void* channel, int severity, int facility)
{
    if(condition)
    {
        return gkr::log::ostream(channel, severity, facility);
    }
    else
    {
        return gkr::log::ostream(channel);
    }
}

#endif /*ndef GENERIC_LOG_SKIP_STREAM*/

#ifndef GENERIC_LOG_USE_CPP_LEGACY
#include <gkr/log/defs/generic_new.hpp>
#else
#include <gkr/log/defs/generic_old.hpp>
#endif

#endif /*ndef LOG_USE_C_DEF*/
#endif /*ndef GENERIC_HPP*/
