#pragma once

#ifndef GKR_LOG_GENERIC_X
#define GKR_LOG_GENERIC_X
#endif

#include <gkr/capi/log/defs/generic.h>

#ifndef LOG_USE_C_DEFS

#include <gkr/log/log.hpp>

#ifndef GKR_NO_STREAM_LOGGING
#define LOG_FINISH gkr::log::finish
#endif

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

inline constexpr bool threshold(int severity)
{
    return (severity < LOG_THRESHOLD_LEVEL);
}

}
}

#if !defined(LOG_GENERIC_DEF_OLD) && !defined(LOG_GENERIC_DEF_NEW)
#ifdef __cpp_if_constexpr
#include <gkr/log/defs/generic_new.hpp>
#else
#include <gkr/log/defs/generic_old.hpp>
#endif
#endif

#if !defined(LOG_FACILITY) && defined(__cpp_if_constexpr)
#include <gkr/log/defs/generic_new.hpp>
#else
#include <gkr/log/defs/generic_old.hpp>
#endif

#endif /* ndef LOG_USE_C_DEF */
