#pragma once

#include <gkr/log/defs/generic.h>
#include <gkr/log/log.hpp>

#ifndef GKR_LOG_THRESHOLD_LEVEL
#define GKR_LOG_THRESHOLD_LEVEL 100
#endif
#ifndef GKR_NO_STREAM_LOGGING
#define LOG_FINISH gkr::log::finish
#endif

#ifndef GKR_LOG_NS
#define GKR_LOG_NS detail::log
#endif

namespace GKR_LOG_NS
{
constexpr const char* severities_names[] = LOG_SEVERITIES_NAMES;

constexpr const char* severity_name_by_value(int severity)
{
    return (unsigned(severity) < (sizeof(severities_names)/sizeof(*severities_names)))
        ? severities_names[severity]
        : "N/A"
        ;
}

inline constexpr bool threshold(int severity)
{
    return (severity < GKR_LOG_THRESHOLD_LEVEL);
}
}

#ifdef __cpp_if_constexpr
#include <gkr/log/defs/generic_new.hpp>
#else
#include <gkr/log/defs/generic_old.hpp>
#endif
