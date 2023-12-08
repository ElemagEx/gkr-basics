#pragma once

#include <gkr/log/defs/generic.h>
#include <gkr/log/log.hpp>

constexpr const char* log_severities[] = LOG_SEVERITIES;

constexpr const char* log_severity_name_by_value(int severity) {
    return (unsigned(severity) < (sizeof(log_severities)/sizeof(*log_severities)))
        ? log_severities[severity]
        : "N/A"
        ;
}

#ifndef GKR_LOG_THRESHOLD_LEVEL
#define GKR_LOG_THRESHOLD_LEVEL 100
#endif
#ifndef GKR_NO_STREAM_LOGGING
#define LOG_FINISH gkr::log::finish
#endif

inline constexpr bool log_threshold(int severity)
{
    return (severity < GKR_LOG_THRESHOLD_LEVEL);
}

#ifndef GKR_LOG_NS
#define GKR_LOG_NS impl
#endif

#ifdef __cpp_if_constexpr
#include <gkr/log/defs/generic_new.hpp>
#else
#include <gkr/log/defs/generic_old.hpp>
#endif
