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
namespace impl
{
constexpr const char* SEVERITIES[] = {"NONE", "FATAL", "ERROR", "WARN", "INFO", "DEBUG", "VERB"};
}
inline constexpr const char* severityToString(Severity severity) noexcept
{
    return (unsigned(severity) < unsigned(sizeof(impl::SEVERITIES)/sizeof(char*)))
        ?  impl::SEVERITIES[severity]
        : *impl::SEVERITIES
        ;
}
inline Severity severityFromString(const char* str)
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
