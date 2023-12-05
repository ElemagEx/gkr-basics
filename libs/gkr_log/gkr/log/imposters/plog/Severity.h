#pragma once

namespace plog
{
enum Severity
{
    none    = 0,
    fatal   = 1,
    error   = 2,
    warning = 3,
    info    = 4,
    debug   = 5,
    verbose = 6
};
inline constexpr const char* severityToString(Severity severity) noexcept
{
    constexpr const char* const SEVERITIES[] = {"NONE", "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "VERB"};
    return (unsigned(severity) < unsigned(sizeof(SEVERITIES)/sizeof(char*)))
        ?  SEVERITIES[severity]
        : *SEVERITIES;
}
inline constexpr Severity severityFromString(const char* str)
{
    if(str) switch(*str)
    {
        case 'F': case 'f': return fatal;
        case 'E': case 'e': return error;
        case 'W': case 'w': return warning;
        case 'I': case 'i': return info;
        case 'D': case 'd': return debug;
        case 'V': case 'v': return verbose;
    }
    return none;
}
}
