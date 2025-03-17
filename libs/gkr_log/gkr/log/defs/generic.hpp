#pragma once

#ifndef GKR_LOG_GENERIC_X
#define GKR_LOG_GENERIC_X

#include <gkr/capi/log/defs/generic.h>

namespace gkr
{
namespace log
{
constexpr const char* severities_names[] = LOG_SEVERITIES_NAMES;

inline constexpr const char* severity_name_by_value(int severity)
{
    return (unsigned(severity) < (sizeof(severities_names)/sizeof(*severities_names)))
        ? severities_names[severity]
        : "N/A"
        ;
}
}
}

#ifndef LOG_USE_C_DEFS

#ifdef LOG_SOURCE_LOCATION
#error Log source location is available only for C macros - define LOG_USE_C_DEFS or include "generic_cdefs.hpp" to use them
#endif

#include <gkr/log/log.hpp>

#ifndef LOG_USE_CPP_LEGACY
#ifndef __cpp_if_constexpr
#define LOG_USE_CPP_LEGACY
#endif
#endif

#ifndef GKR_NO_STREAM_LOGGING
#define LOG_FINISH gkr::log::finish
#endif

namespace gkr
{
namespace log
{
inline constexpr bool threshold(int severity)
{
    return (severity < LOG_THRESHOLD_LEVEL);
}

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
#ifndef GKR_NO_STREAM_LOGGING

inline auto stream_message_rts(void* instance, int severity, int facility)
{
    return ostream(instance, severity, facility);
}
inline auto stream_message_rts_if(bool condition, void* instance, int severity, int facility)
{
    if(condition)
    {
        return ostream(instance, severity, facility);
    }
    else
    {
        return ostream(instance);
    }
}

#endif /*ndef GKR_NO_STREAM_LOGGING*/
}
}

#ifndef LOG_USE_CPP_LEGACY
#include <gkr/log/defs/generic_new.hpp>
#else
#include <gkr/log/defs/generic_old.hpp>
#endif

#endif /*ndef LOG_USE_C_DEF*/
#endif /*ndef GKR_LOG_GENERIC_X*/
