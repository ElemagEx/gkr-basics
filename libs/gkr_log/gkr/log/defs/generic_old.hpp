#pragma once

#ifndef GKR_LOG_GENERIC_OLD
#define GKR_LOG_GENERIC_OLD

#ifndef GKR_LOG_GENERIC_X
#if !defined(LOG_USE_C_DEFS) && !defined(LOG_USE_CPP_LEGACY)
#define LOG_USE_CPP_LEGACY
#endif
#include <gkr/log/defs/generic.hpp>
#endif

#if !defined(LOG_USE_C_DEFS) && defined(LOG_USE_CPP_LEGACY)

namespace gkr
{
namespace log
{
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

#ifdef GKR_LOG_BASIC_KEEP
//
// C++ Legacy Simple basic LOG - bLOGxxx
//
inline void bLOG   (                int severity, const char* msg) { gkr::log::simple_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY, msg); }
inline void bLOG_IF(bool condition, int severity, const char* msg) { gkr::log::simple_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, msg); }

inline void bLOGF       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGE       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGW       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGI       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGV       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGD       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGT       (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, LOG_FACILITY, msg); }

inline void bLOG_FATAL  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_ERROR  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_WARNING(const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_INFO   (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_VERBOSE(const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_DEBUG  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_TRACE  (const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, LOG_FACILITY, msg); }

inline void bLOGF_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGE_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGW_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGI_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGV_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGD_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOGT_IF       (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }

inline void bLOG_FATAL_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_ERROR_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_WARNING_IF(bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_INFO_IF   (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_VERBOSE_IF(bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_DEBUG_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void bLOG_TRACE_IF  (bool condition, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C++ Legacy Simple facility LOG - fLOGxxx
//
inline void fLOG   (                int severity, int facility, const char* msg) { gkr::log::simple_message_rts   (           LOG_INSTANCE, severity, facility, msg); }
inline void fLOG_IF(bool condition, int severity, int facility, const char* msg) { gkr::log::simple_message_rts_if(condition, LOG_INSTANCE, severity, facility, msg); }

inline void fLOGF       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, msg); }
inline void fLOGE       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, msg); }
inline void fLOGW       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, msg); }
inline void fLOGI       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, msg); }
inline void fLOGV       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, msg); }
inline void fLOGD       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, msg); }
inline void fLOGT       (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, msg); }

inline void fLOG_FATAL  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, msg); }
inline void fLOG_ERROR  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, msg); }
inline void fLOG_WARNING(int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, msg); }
inline void fLOG_INFO   (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, msg); }
inline void fLOG_VERBOSE(int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, msg); }
inline void fLOG_DEBUG  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, msg); }
inline void fLOG_TRACE  (int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, msg); }

inline void fLOGF_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGE_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGW_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGI_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGV_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGD_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOGT_IF       (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, msg); }

inline void fLOG_FATAL_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_ERROR_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_WARNING_IF(bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_INFO_IF   (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_VERBOSE_IF(bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_DEBUG_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, msg); }
inline void fLOG_TRACE_IF  (bool condition, int facility, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, msg); }
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_MODULE_KEEP
//
// C++ Legacy Simple module LOG - mLOGxxx
//
inline void mLOG   (                void* instance, int severity, const char* msg) { gkr::log::simple_message_rts   (           instance, severity, LOG_FACILITY, msg); }
inline void mLOG_IF(bool condition, void* instance, int severity, const char* msg) { gkr::log::simple_message_rts_if(condition, instance, severity, LOG_FACILITY, msg); }

inline void mLOGF       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, LOG_FACILITY, msg); }
inline void mLOGE       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, LOG_FACILITY, msg); }
inline void mLOGW       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, LOG_FACILITY, msg); }
inline void mLOGI       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, LOG_FACILITY, msg); }
inline void mLOGV       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, LOG_FACILITY, msg); }
inline void mLOGD       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, LOG_FACILITY, msg); }
inline void mLOGT       (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, LOG_FACILITY, msg); }

inline void mLOG_FATAL  (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, LOG_FACILITY, msg); }
inline void mLOG_ERROR  (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, LOG_FACILITY, msg); }
inline void mLOG_WARNING(void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, LOG_FACILITY, msg); }
inline void mLOG_INFO   (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, LOG_FACILITY, msg); }
inline void mLOG_VERBOSE(void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, LOG_FACILITY, msg); }
inline void mLOG_DEBUG  (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, LOG_FACILITY, msg); }
inline void mLOG_TRACE  (void* instance, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, LOG_FACILITY, msg); }

inline void mLOGF_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, LOG_FACILITY, msg); }
inline void mLOGE_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, LOG_FACILITY, msg); }
inline void mLOGW_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, LOG_FACILITY, msg); }
inline void mLOGI_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, LOG_FACILITY, msg); }
inline void mLOGV_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, LOG_FACILITY, msg); }
inline void mLOGD_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, LOG_FACILITY, msg); }
inline void mLOGT_IF       (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, LOG_FACILITY, msg); }

inline void mLOG_FATAL_IF  (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, LOG_FACILITY, msg); }
inline void mLOG_ERROR_IF  (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, LOG_FACILITY, msg); }
inline void mLOG_WARNING_IF(bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, LOG_FACILITY, msg); }
inline void mLOG_INFO_IF   (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, LOG_FACILITY, msg); }
inline void mLOG_VERBOSE_IF(bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, LOG_FACILITY, msg); }
inline void mLOG_DEBUG_IF  (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, LOG_FACILITY, msg); }
inline void mLOG_TRACE_IF  (bool condition, void* instance, const char* msg)       { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, LOG_FACILITY, msg); }
#endif /*def GKR_LOG_MODULE_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C++ Legacy Simple general LOG - gLOGxxx
//
inline void gLOG   (                void* instance, int severity, int facility, const char* msg) { gkr::log::simple_message_rts   (           instance, severity, facility, msg); }
inline void gLOG_IF(bool condition, void* instance, int severity, int facility, const char* msg) { gkr::log::simple_message_rts_if(condition, instance, severity, facility, msg); }

inline void gLOGF       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, msg); }
inline void gLOGE       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, msg); }
inline void gLOGW       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, msg); }
inline void gLOGI       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, msg); }
inline void gLOGV       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, msg); }
inline void gLOGD       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, msg); }
inline void gLOGT       (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, msg); }

inline void gLOG_FATAL  (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, msg); }
inline void gLOG_ERROR  (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, msg); }
inline void gLOG_WARNING(void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, msg); }
inline void gLOG_INFO   (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, msg); }
inline void gLOG_VERBOSE(void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, msg); }
inline void gLOG_DEBUG  (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, msg); }
inline void gLOG_TRACE  (void* instance, int facility, const char* msg)                          { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, msg); }

inline void gLOGF_IF        (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, msg); }
inline void gLOGE_IF        (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, msg); }
inline void gLOGW_IF        (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, msg); }
inline void gLOGI_IF        (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, msg); }
inline void gLOGV_IF        (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, msg); }
inline void gLOGD_IF        (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, msg); }
inline void gLOGT_IF        (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, msg); }

inline void gLOG_FATAL_IF   (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, msg); }
inline void gLOG_ERROR_IF   (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, msg); }
inline void gLOG_WARNING_IF (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, msg); }
inline void gLOG_INFO_IF    (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, msg); }
inline void gLOG_VERBOSE_IF (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, msg); }
inline void gLOG_DEBUG_IF   (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, msg); }
inline void gLOG_TRACE_IF   (bool condition, void* instance, int facility, const char* msg)      { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, msg); }
#endif /*def GKR_LOG_GENERAL_KEEP*/

#ifndef GKR_NO_PRINTF_LOGGING

#ifdef GKR_LOG_BASIC_KEEP
//
// C++ Legacy PrintF basic LOG - bLOGxxx
//
inline void bLOG_   (                int severity, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_IF_(bool condition, int severity, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, format, args); va_end(args); }

inline void bLOGF_       (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGE_       (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGW_       (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGI_       (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGV_       (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGD_       (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGT_       (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }

inline void bLOG_FATAL_  (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_ERROR_  (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_WARNING_(const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_INFO_   (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_VERBOSE_(const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_DEBUG_  (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_TRACE_  (const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }

inline void bLOGF_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGE_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGW_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGI_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGV_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGD_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGT_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }

inline void bLOG_FATAL_IF_  (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_ERROR_IF_  (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_WARNING_IF_(bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_INFO_IF_   (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_VERBOSE_IF_(bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_DEBUG_IF_  (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_TRACE_IF_  (bool condition, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, LOG_FACILITY, format, args); va_end(args); }
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C++ Legacy PrintF facility LOG - fLOGxxx
//
inline void fLOG_   (                int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts   (           LOG_INSTANCE, severity, facility, format, args); va_end(args); }
inline void fLOG_IF_(bool condition, int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts_if(condition, LOG_INSTANCE, severity, facility, format, args); va_end(args); }

inline void fLOGF_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGE_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGW_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGI_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGV_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGD_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGT_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, format, args); va_end(args); }

inline void fLOG_FATAL_  (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_ERROR_  (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_WARNING_(int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_INFO_   (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_VERBOSE_(int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_DEBUG_  (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_TRACE_  (int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, format, args); va_end(args); }

inline void fLOGF_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGE_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGW_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGI_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGV_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGD_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOGT_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }

inline void fLOG_FATAL_IF_  (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_ERROR_IF_  (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_WARNING_IF_(bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_INFO_IF_   (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_VERBOSE_IF_(bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_DEBUG_IF_  (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void fLOG_TRACE_IF_  (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_MODULE_KEEP
//
// C++ Legacy PrintF module LOG - mLOGxxx
//
inline void mLOG_   (                void* instance, int severity, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts   (           instance, severity, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_IF_(bool condition, void* instance, int severity, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts_if(condition, instance, severity, LOG_FACILITY, format, args); va_end(args); }

inline void mLOGF_       (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGE_       (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGW_       (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGI_       (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGV_       (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGD_       (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGT_       (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, LOG_FACILITY, format, args); va_end(args); }

inline void mLOG_FATAL_  (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_ERROR_  (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_WARNING_(void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_INFO_   (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_VERBOSE_(void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_DEBUG_  (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_TRACE_  (void* instance, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, LOG_FACILITY, format, args); va_end(args); }

inline void mLOGF_IF_       (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGE_IF_       (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGW_IF_       (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGI_IF_       (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGV_IF_       (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGD_IF_       (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOGT_IF_       (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }

inline void mLOG_FATAL_IF_  (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_ERROR_IF_  (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_WARNING_IF_(bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_INFO_IF_   (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_VERBOSE_IF_(bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_DEBUG_IF_  (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
inline void mLOG_TRACE_IF_  (bool condition, void* instance, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, LOG_FACILITY, format, args); va_end(args); }
#endif /*def GKR_LOG_MODULE_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C++ Legacy PrintF general LOG - gLOGxxx
//
inline void gLOG_   (                void* instance, int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts   (           instance, severity, facility, format, args); va_end(args); }
inline void gLOG_IF_(bool condition, void* instance, int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts_if(condition, instance, severity, facility, format, args); va_end(args); }

inline void gLOGF_       (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, format, args); va_end(args); }
inline void gLOGE_       (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, format, args); va_end(args); }
inline void gLOGW_       (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, format, args); va_end(args); }
inline void gLOGI_       (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, format, args); va_end(args); }
inline void gLOGV_       (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, format, args); va_end(args); }
inline void gLOGD_       (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, format, args); va_end(args); }
inline void gLOGT_       (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, format, args); va_end(args); }

inline void gLOG_FATAL_  (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, format, args); va_end(args); }
inline void gLOG_ERROR_  (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, format, args); va_end(args); }
inline void gLOG_WARNING_(void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, format, args); va_end(args); }
inline void gLOG_INFO_   (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, format, args); va_end(args); }
inline void gLOG_VERBOSE_(void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, format, args); va_end(args); }
inline void gLOG_DEBUG_  (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, format, args); va_end(args); }
inline void gLOG_TRACE_  (void* instance, int facility, const char* format, ...)                          { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, format, args); va_end(args); }

inline void gLOGF_IF_       (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, format, args); va_end(args); }
inline void gLOGE_IF_       (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, format, args); va_end(args); }
inline void gLOGW_IF_       (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, format, args); va_end(args); }
inline void gLOGI_IF_       (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, format, args); va_end(args); }
inline void gLOGV_IF_       (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, format, args); va_end(args); }
inline void gLOGD_IF_       (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, format, args); va_end(args); }
inline void gLOGT_IF_       (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, format, args); va_end(args); }

inline void gLOG_FATAL_IF_  (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, format, args); va_end(args); }
inline void gLOG_ERROR_IF_  (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, format, args); va_end(args); }
inline void gLOG_WARNING_IF_(bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, format, args); va_end(args); }
inline void gLOG_INFO_IF_   (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, format, args); va_end(args); }
inline void gLOG_VERBOSE_IF_(bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, format, args); va_end(args); }
inline void gLOG_DEBUG_IF_  (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, format, args); va_end(args); }
inline void gLOG_TRACE_IF_  (bool condition, void* instance, int facility, const char* format, ...)       { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, format, args); va_end(args); }
#endif /*def GKR_LOG_GENERAL_KEEP*/

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

#ifdef GKR_LOG_BASIC_KEEP
//
// C++ Legacy Stream basic LOG - bLOGxxx
//
inline auto bLOG   (                int severity) { return gkr::log::stream_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY); }
inline auto bLOG_IF(bool condition, int severity) { return gkr::log::stream_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY); }

inline auto bLOGF       ()                        { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGE       ()                        { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGW       ()                        { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGI       ()                        { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGV       ()                        { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGD       ()                        { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGT       ()                        { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, LOG_FACILITY); }

inline auto bLOG_FATAL  ()                        { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_ERROR  ()                        { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_WARNING()                        { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_INFO   ()                        { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_VERBOSE()                        { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_DEBUG  ()                        { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_TRACE  ()                        { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, LOG_FACILITY); }

inline auto bLOGF_IF        (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGE_IF        (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGW_IF        (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGI_IF        (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGV_IF        (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGD_IF        (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOGT_IF        (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, LOG_FACILITY); }

inline auto bLOG_FATAL_IF   (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_ERROR_IF   (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_WARNING_IF (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_INFO_IF    (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_VERBOSE_IF (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_DEBUG_IF   (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto bLOG_TRACE_IF   (bool condition)      { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, LOG_FACILITY); }
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C++ Legacy Stream facility LOG - fLOGxxx
//
inline auto fLOG   (                int severity, int facility) { return gkr::log::stream_message_rts   (           LOG_INSTANCE, severity, facility); }
inline auto fLOG_IF(bool condition, int severity, int facility) { return gkr::log::stream_message_rts_if(condition, LOG_INSTANCE, severity, facility); }

inline auto fLOGF       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility); }
inline auto fLOGE       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility); }
inline auto fLOGW       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility); }
inline auto fLOGI       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility); }
inline auto fLOGV       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility); }
inline auto fLOGD       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility); }
inline auto fLOGT       (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility); }

inline auto fLOG_FATAL  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility); }
inline auto fLOG_ERROR  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility); }
inline auto fLOG_WARNING(int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility); }
inline auto fLOG_INFO   (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility); }
inline auto fLOG_VERBOSE(int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility); }
inline auto fLOG_DEBUG  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility); }
inline auto fLOG_TRACE  (int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility); }

inline auto fLOGF_IF        (bool condition, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility); }
inline auto fLOGE_IF        (bool condition, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility); }
inline auto fLOGW_IF        (bool condition, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility); }
inline auto fLOGI_IF        (bool condition, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility); }
inline auto fLOGV_IF        (bool condition, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility); }
inline auto fLOGD_IF        (bool condition, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility); }
inline auto fLOGT_IF        (bool condition, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility); }

inline auto fLOG_FATAL_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility); }
inline auto fLOG_ERROR_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility); }
inline auto fLOG_WARNING_IF(bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility); }
inline auto fLOG_INFO_IF   (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility); }
inline auto fLOG_VERBOSE_IF(bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility); }
inline auto fLOG_DEBUG_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility); }
inline auto fLOG_TRACE_IF  (bool condition, int facility)       { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility); }
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_MODULE_KEEP
//
// C++ Legacy Stream module LOG - mLOGxxx
//
inline auto mLOG   (                void* instance, int severity) { return gkr::log::stream_message_rts   (           instance, severity, LOG_FACILITY); }
inline auto mLOG_IF(bool condition, void* instance, int severity) { return gkr::log::stream_message_rts_if(condition, instance, severity, LOG_FACILITY); }

inline auto mLOGF       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, LOG_FACILITY); }
inline auto mLOGE       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, LOG_FACILITY); }
inline auto mLOGW       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, LOG_FACILITY); }
inline auto mLOGI       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, LOG_FACILITY); }
inline auto mLOGV       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, LOG_FACILITY); }
inline auto mLOGD       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, LOG_FACILITY); }
inline auto mLOGT       (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, LOG_FACILITY); }

inline auto mLOG_FATAL  (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, LOG_FACILITY); }
inline auto mLOG_ERROR  (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, LOG_FACILITY); }
inline auto mLOG_WARNING(void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, LOG_FACILITY); }
inline auto mLOG_INFO   (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, LOG_FACILITY); }
inline auto mLOG_VERBOSE(void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, LOG_FACILITY); }
inline auto mLOG_DEBUG  (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, LOG_FACILITY); }
inline auto mLOG_TRACE  (void* instance)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, LOG_FACILITY); }

inline auto mLOGF_IF        (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, LOG_FACILITY); }
inline auto mLOGE_IF        (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, LOG_FACILITY); }
inline auto mLOGW_IF        (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, LOG_FACILITY); }
inline auto mLOGI_IF        (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, LOG_FACILITY); }
inline auto mLOGV_IF        (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, LOG_FACILITY); }
inline auto mLOGD_IF        (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, LOG_FACILITY); }
inline auto mLOGT_IF        (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, LOG_FACILITY); }

inline auto mLOG_FATAL_IF   (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, LOG_FACILITY); }
inline auto mLOG_ERROR_IF   (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, LOG_FACILITY); }
inline auto mLOG_WARNING_IF (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, LOG_FACILITY); }
inline auto mLOG_INFO_IF    (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, LOG_FACILITY); }
inline auto mLOG_VERBOSE_IF (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, LOG_FACILITY); }
inline auto mLOG_DEBUG_IF   (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, LOG_FACILITY); }
inline auto mLOG_TRACE_IF   (bool condition, void* instance)      { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, LOG_FACILITY); }
#endif /*def GKR_LOG_MODULE_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C++ Legacy Stream general LOG - gLOGxxx
//
inline auto gLOG   (                void* instance, int severity, int facility) { return gkr::log::stream_message_rts   (           instance, severity, facility); }
inline auto gLOG_IF(bool condition, void* instance, int severity, int facility) { return gkr::log::stream_message_rts_if(condition, instance, severity, facility); }

inline auto gLOGF       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility); }
inline auto gLOGE       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility); }
inline auto gLOGW       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility); }
inline auto gLOGI       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility); }
inline auto gLOGV       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility); }
inline auto gLOGD       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility); }
inline auto gLOGT       (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility); }

inline auto gLOG_FATAL  (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility); }
inline auto gLOG_ERROR  (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility); }
inline auto gLOG_WARNING(void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility); }
inline auto gLOG_INFO   (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility); }
inline auto gLOG_VERBOSE(void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility); }
inline auto gLOG_DEBUG  (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility); }
inline auto gLOG_TRACE  (void* instance, int facility)                          { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility); }

inline auto gLOGF_IF        (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility); }
inline auto gLOGE_IF        (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility); }
inline auto gLOGW_IF        (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility); }
inline auto gLOGI_IF        (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility); }
inline auto gLOGV_IF        (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility); }
inline auto gLOGD_IF        (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility); }
inline auto gLOGT_IF        (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility); }

inline auto gLOG_FATAL_IF   (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility); }
inline auto gLOG_ERROR_IF   (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility); }
inline auto gLOG_WARNING_IF (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility); }
inline auto gLOG_INFO_IF    (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility); }
inline auto gLOG_VERBOSE_IF (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility); }
inline auto gLOG_DEBUG_IF   (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility); }
inline auto gLOG_TRACE_IF   (bool condition, void* instance, int facility)      { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility); }
#endif /*def GKR_LOG_GENERAL_KEEP*/

#endif /*ndef GKR_NO_STREAM_LOGGING*/

#endif /*!defined(LOG_USE_C_DEFS) && defined(LOG_USE_CPP_LEGACY)*/
#endif /*ndef GKR_LOG_GENERIC_OLD*/
