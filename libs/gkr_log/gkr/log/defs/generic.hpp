#pragma once

#include <gkr/log/defs/generic.h>
#include <gkr/log/log.hpp>

#ifndef GKR_NO_STREAM_LOGGING
#define LOG_FINISH gkr::log::finish
#endif

#ifndef LOG_NS
#define LOG_NS       detail::log
#define LOG_NS_BEGIN namespace detail { namespace log
#define LOG_NS_END   }
#else
#ifndef LOG_NS_BEGIN
#define LOG_NS_BEGIN namespace LOG_NS
//#pragma message
#endif
#ifndef LOG_NS_END
#define LOG_NS_END
//#pragma message
#endif
#endif

LOG_NS_BEGIN
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
    return (severity < LOG_THRESHOLD_LEVEL);
}

}
LOG_NS_END

#ifdef __cpp_if_constexpr
#include <gkr/log/defs/generic_new.hpp>
#else
#include <gkr/log/defs/generic_old.hpp>
#endif
