#pragma once

#ifndef GKR_LOG_GENERIC_X
#error  You cannot include this header directly - define LOG_CPP_LEGACY (or C++ if constexpr is not available) and then include "generic.hpp"
#endif

#ifdef  LOG_USE_C_DEFS
#error  This should not be defined
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

template<int severity, bool>
inline void simple_message(void* instance, int facility, const char* msg)
{
    gkr_log_simple_message(instance, severity, facility, msg);
}
template<> inline void simple_message<LOG_SEVERITY_FATAL  ,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_ERROR  ,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_WARNING,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_INFO   ,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_VERBOSE,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_DEBUG  ,false>(void*, int, const char*) {}
template<> inline void simple_message<LOG_SEVERITY_TRACE  ,false>(void*, int, const char*) {}

template<int severity, bool>
inline void simple_message_if(bool condition, void* instance, int facility, const char* msg)
{
    if(condition)
    {
        gkr_log_simple_message(instance, severity, facility, msg);
    }
}
template<> inline void simple_message_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_WARNING,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int, const char*) {}
template<> inline void simple_message_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int, const char*) {}
}
}

#ifndef GKR_NO_PRINTF_LOGGING

namespace gkr
{
namespace log
{
inline void printf_message_rts(void* instance, int severity, int facility, const char* format, va_list args)
{
    gkr_log_valist_message(instance, severity, facility, format, args);
}
inline void printf_message_rts_if(bool condition, void* instance, int severity, int facility, const char* format, va_list args)
{
    if(condition)
    {
        gkr_log_valist_message(instance, severity, facility, format, args);
    }
}

template<int severity, bool>
inline void printf_message(void* instance, int facility, const char* format, va_list args)
{
    gkr_log_valist_message(instance, severity, facility, format, args);
}
template<> void printf_message<LOG_SEVERITY_FATAL  ,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_ERROR  ,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_WARNING,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_INFO   ,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_VERBOSE,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_DEBUG  ,false>(void*, int, const char*, va_list) {}
template<> void printf_message<LOG_SEVERITY_TRACE  ,false>(void*, int, const char*, va_list) {}

template<int severity, bool>
inline void printf_message_if(bool condition, void* instance, int facility, const char* format, va_list args)
{
    if(condition)
    {
        gkr_log_valist_message(instance, severity, facility, format, args);
    }
}
template<> inline void printf_message_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_WARNING,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void printf_message_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int, const char*, va_list) {}
}
}

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

namespace gkr
{
namespace log
{
template<int severity, bool>
inline auto stream_message(void* instance, int facility)
{
    return ostream(instance, severity, facility);
}
template<> auto stream_message<LOG_SEVERITY_FATAL  ,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_ERROR  ,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_WARNING,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_INFO   ,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_VERBOSE,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_DEBUG  ,false>(void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message<LOG_SEVERITY_TRACE  ,false>(void*, int) { return impl::dummy_ostream(); }
template<int severity, bool>
inline auto stream_message_if(bool condition, void* instance, int facility)
{
    if(condition)
    {
        return gkr::log::ostream(instance, severity, facility);
    }
    else
    {
        return gkr::log::ostream(instance);
    }
}
template<> auto stream_message_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_WARNING,false>(bool, void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int) { return impl::dummy_ostream(); }
template<> auto stream_message_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int) { return impl::dummy_ostream(); }
}
}

#endif /*ndef GKR_NO_STREAM_LOGGING*/

#ifdef LOG_FACILITY
#include <gkr/log/defs/generic_old_basic.hpp>
#else
#include <gkr/log/defs/generic_old_extra.hpp>
#endif
