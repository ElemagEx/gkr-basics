#pragma once

#ifndef GENERIC_OLD_HPP
#define GENERIC_OLD_HPP

#ifndef GENERIC_HPP
#if !defined(GENERIC_LOG_USE_C_DEFS) && !defined(GENERIC_LOG_USE_CPP_LEGACY)
#define GENERIC_LOG_USE_CPP_LEGACY
#endif
#include <gkr/log/defs/generic.hpp>
#endif

#if !defined(GENERIC_LOG_USE_C_DEFS) && defined(GENERIC_LOG_USE_CPP_LEGACY)

template<int severity, bool>
inline void _log_simple_msg_cts_uc(void* channel, int facility, const char* msg)
{
    gkr_log_simple_message(channel, severity, facility, msg);
}
template<> inline void _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  ,false>(void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  ,false>(void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING,false>(void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   ,false>(void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE,false>(void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  ,false>(void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  ,false>(void*, int, const char*) {}

template<int severity, bool>
inline void _log_simple_msg_cts_if(bool condition, void* channel, int facility, const char* msg)
{
    if(condition)
    {
        gkr_log_simple_message(channel, severity, facility, msg);
    }
}
template<> inline void _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_if<LOG_SEVERITY_WARNING,false>(bool, void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int, const char*) {}
template<> inline void _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int, const char*) {}

#ifndef GENERIC_LOG_SKIP_PRINTF

inline void _log_printf_msg_rts_uc(void* channel, int severity, int facility, const char* format, va_list args)
{
    gkr_log_valist_message(channel, severity, facility, format, args);
}
inline void _log_printf_msg_rts_if(bool condition, void* channel, int severity, int facility, const char* format, va_list args)
{
    if(condition)
    {
        gkr_log_valist_message(channel, severity, facility, format, args);
    }
}

template<int severity, bool>
inline void _log_printf_msg_cts_uc(void* channel, int facility, const char* format, va_list args)
{
    gkr_log_valist_message(channel, severity, facility, format, args);
}
template<> void _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  ,false>(void*, int, const char*, va_list) {}
template<> void _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  ,false>(void*, int, const char*, va_list) {}
template<> void _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING,false>(void*, int, const char*, va_list) {}
template<> void _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   ,false>(void*, int, const char*, va_list) {}
template<> void _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE,false>(void*, int, const char*, va_list) {}
template<> void _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  ,false>(void*, int, const char*, va_list) {}
template<> void _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  ,false>(void*, int, const char*, va_list) {}

template<int severity, bool>
inline void _log_printf_msg_cts_if(bool condition, void* channel, int facility, const char* format, va_list args)
{
    if(condition)
    {
        gkr_log_valist_message(channel, severity, facility, format, args);
    }
}
template<> inline void _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void _log_printf_msg_cts_if<LOG_SEVERITY_WARNING,false>(bool, void*, int, const char*, va_list) {}
template<> inline void _log_printf_msg_cts_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int, const char*, va_list) {}
template<> inline void _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int, const char*, va_list) {}
template<> inline void _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int, const char*, va_list) {}

#endif /*ndef GENERIC_LOG_SKIP_PRINTF*/

#ifndef GENERIC_LOG_SKIP_STREAM

template<int severity, bool>
inline auto _log_stream_msg_cts_uc(void* channel, int facility)
{
    return ostream(channel, severity, facility);
}
template<> auto _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  ,false>(void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  ,false>(void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING,false>(void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   ,false>(void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE,false>(void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  ,false>(void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  ,false>(void*, int) { return gkr::log::impl::dummy_ostream(); }
template<int severity, bool>
inline auto _log_stream_msg_cts_if(bool condition, void* channel, int facility)
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
template<> auto _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_if<LOG_SEVERITY_WARNING,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_if<LOG_SEVERITY_INFO   ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }
template<> auto _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  ,false>(bool, void*, int) { return gkr::log::impl::dummy_ostream(); }

#endif /*ndef GENERIC_LOG_SKIP_STREAM*/

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C++ Legacy Simple basic LOG - bLOGxxx
//
inline void bLOG(int severity, const char* msg)                    { _log_simple_msg_rts_uc(LOG_CHANNEL, severity, LOG_FACILITY, msg); }

inline void bLOGF       (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGE       (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGW       (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGI       (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGV       (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGD       (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGT       (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, LOG_FACILITY, msg); }

inline void bLOG_FATAL  (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_ERROR  (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_WARNING(const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_INFO   (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_VERBOSE(const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_DEBUG  (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_TRACE  (const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, LOG_FACILITY, msg); }

inline void bLOG_IF(bool condition, int severity, const char* msg) { _log_simple_msg_rts_if(condition, LOG_CHANNEL, severity, LOG_FACILITY, msg); }

inline void bLOGF_IF       (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGE_IF       (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGW_IF       (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGI_IF       (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGV_IF       (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGD_IF       (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOGT_IF       (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }

inline void bLOG_FATAL_IF  (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_ERROR_IF  (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_WARNING_IF(bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_INFO_IF   (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_VERBOSE_IF(bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_DEBUG_IF  (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
inline void bLOG_TRACE_IF  (bool condition, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, LOG_FACILITY, msg); }
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C++ Legacy Simple facility LOG - fLOGxxx
//
inline void fLOG(int severity, int facility, const char* msg)                    { _log_simple_msg_rts_uc(LOG_CHANNEL, severity, facility, msg); }

inline void fLOGF       (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, facility, msg); }
inline void fLOGE       (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, facility, msg); }
inline void fLOGW       (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, facility, msg); }
inline void fLOGI       (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, facility, msg); }
inline void fLOGV       (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, facility, msg); }
inline void fLOGD       (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, facility, msg); }
inline void fLOGT       (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, facility, msg); }

inline void fLOG_FATAL  (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, facility, msg); }
inline void fLOG_ERROR  (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, facility, msg); }
inline void fLOG_WARNING(int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, facility, msg); }
inline void fLOG_INFO   (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, facility, msg); }
inline void fLOG_VERBOSE(int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, facility, msg); }
inline void fLOG_DEBUG  (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, facility, msg); }
inline void fLOG_TRACE  (int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, facility, msg); }

inline void fLOG_IF(bool condition, int severity, int facility, const char* msg) { _log_simple_msg_rts_if(condition, LOG_CHANNEL, severity, facility, msg); }

inline void fLOGF_IF       (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGE_IF       (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGW_IF       (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGI_IF       (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGV_IF       (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGD_IF       (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOGT_IF       (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, facility, msg); }

inline void fLOG_FATAL_IF  (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_ERROR_IF  (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_WARNING_IF(bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_INFO_IF   (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_VERBOSE_IF(bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_DEBUG_IF  (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, facility, msg); }
inline void fLOG_TRACE_IF  (bool condition, int facility, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, facility, msg); }
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C++ Legacy Simple channel LOG - cLOGxxx
//
inline void cLOG(void* channel, int severity, const char* msg)                    { _log_simple_msg_rts_uc(channel, severity, LOG_FACILITY, msg); }

inline void cLOGF       (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, LOG_FACILITY, msg); }
inline void cLOGE       (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, LOG_FACILITY, msg); }
inline void cLOGW       (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, LOG_FACILITY, msg); }
inline void cLOGI       (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, LOG_FACILITY, msg); }
inline void cLOGV       (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, LOG_FACILITY, msg); }
inline void cLOGD       (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, LOG_FACILITY, msg); }
inline void cLOGT       (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, LOG_FACILITY, msg); }

inline void cLOG_FATAL  (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, LOG_FACILITY, msg); }
inline void cLOG_ERROR  (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, LOG_FACILITY, msg); }
inline void cLOG_WARNING(void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, LOG_FACILITY, msg); }
inline void cLOG_INFO   (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, LOG_FACILITY, msg); }
inline void cLOG_VERBOSE(void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, LOG_FACILITY, msg); }
inline void cLOG_DEBUG  (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, LOG_FACILITY, msg); }
inline void cLOG_TRACE  (void* channel, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, LOG_FACILITY, msg); }

inline void cLOG_IF(bool condition, void* channel, int severity, const char* msg) { _log_simple_msg_rts_if(condition, channel, severity, LOG_FACILITY, msg); }

inline void cLOGF_IF       (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, LOG_FACILITY, msg); }
inline void cLOGE_IF       (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, LOG_FACILITY, msg); }
inline void cLOGW_IF       (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, LOG_FACILITY, msg); }
inline void cLOGI_IF       (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, LOG_FACILITY, msg); }
inline void cLOGV_IF       (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, LOG_FACILITY, msg); }
inline void cLOGD_IF       (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, LOG_FACILITY, msg); }
inline void cLOGT_IF       (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, LOG_FACILITY, msg); }

inline void cLOG_FATAL_IF  (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_ERROR_IF  (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_WARNING_IF(bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_INFO_IF   (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_VERBOSE_IF(bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_DEBUG_IF  (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, LOG_FACILITY, msg); }
inline void cLOG_TRACE_IF  (bool condition, void* channel, const char* msg)       { _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, LOG_FACILITY, msg); }
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C++ Legacy Simple general LOG - gLOGxxx
//
inline void gLOG(void* channel, int severity, int facility, const char* msg)                    { _log_simple_msg_rts_uc(channel, severity, facility, msg); }

inline void gLOGF       (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, facility, msg); }
inline void gLOGE       (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, facility, msg); }
inline void gLOGW       (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, facility, msg); }
inline void gLOGI       (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, facility, msg); }
inline void gLOGV       (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, facility, msg); }
inline void gLOGD       (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, facility, msg); }
inline void gLOGT       (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, facility, msg); }

inline void gLOG_FATAL  (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, facility, msg); }
inline void gLOG_ERROR  (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, facility, msg); }
inline void gLOG_WARNING(void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, facility, msg); }
inline void gLOG_INFO   (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, facility, msg); }
inline void gLOG_VERBOSE(void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, facility, msg); }
inline void gLOG_DEBUG  (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, facility, msg); }
inline void gLOG_TRACE  (void* channel, int facility, const char* msg)                          { _log_simple_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, facility, msg); }

inline void gLOG_IF(bool condition, void* channel, int severity, int facility, const char* msg) { _log_simple_msg_rts_if(condition, channel, severity, facility, msg); }

inline void gLOGF_IF        (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, facility, msg); }
inline void gLOGE_IF        (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, facility, msg); }
inline void gLOGW_IF        (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, facility, msg); }
inline void gLOGI_IF        (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, facility, msg); }
inline void gLOGV_IF        (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, facility, msg); }
inline void gLOGD_IF        (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, facility, msg); }
inline void gLOGT_IF        (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, facility, msg); }

inline void gLOG_FATAL_IF   (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, facility, msg); }
inline void gLOG_ERROR_IF   (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, facility, msg); }
inline void gLOG_WARNING_IF (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, facility, msg); }
inline void gLOG_INFO_IF    (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, facility, msg); }
inline void gLOG_VERBOSE_IF (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, facility, msg); }
inline void gLOG_DEBUG_IF   (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, facility, msg); }
inline void gLOG_TRACE_IF   (bool condition, void* channel, int facility, const char* msg)      { _log_simple_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, facility, msg); }
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#ifndef GENERIC_LOG_SKIP_PRINTF

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C++ Legacy PrintF basic LOG - bLOGxxx
//
inline void bLOG_(int severity, const char* format, ...)                    { va_list args; va_start(args, format); _log_printf_msg_rts_uc(LOG_CHANNEL, severity, LOG_FACILITY, format, args); va_end(args); }

inline void bLOGF_       (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGE_       (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGW_       (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGI_       (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGV_       (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGD_       (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGT_       (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }

inline void bLOG_FATAL_  (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_ERROR_  (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_WARNING_(const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_INFO_   (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_VERBOSE_(const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_DEBUG_  (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_TRACE_  (const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }

inline void bLOG_IF_(bool condition, int severity, const char* format, ...) { va_list args; va_start(args, format); _log_printf_msg_rts_if(condition, LOG_CHANNEL, severity, LOG_FACILITY, format, args); va_end(args); }

inline void bLOGF_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGE_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGW_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGI_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGV_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGD_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOGT_IF_       (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }

inline void bLOG_FATAL_IF_  (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_ERROR_IF_  (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_WARNING_IF_(bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_INFO_IF_   (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_VERBOSE_IF_(bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_DEBUG_IF_  (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
inline void bLOG_TRACE_IF_  (bool condition, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, LOG_FACILITY, format, args); va_end(args); }
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C++ Legacy PrintF facility LOG - fLOGxxx
//
inline void fLOG_(int severity, int facility, const char* format, ...)                    { va_list args; va_start(args, format); _log_printf_msg_rts_uc(LOG_CHANNEL, severity, facility, format, args); va_end(args); }

inline void fLOGF_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGE_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGW_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGI_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGV_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGD_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGT_       (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, facility, format, args); va_end(args); }

inline void fLOG_FATAL_  (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_ERROR_  (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_WARNING_(int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_INFO_   (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_VERBOSE_(int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_DEBUG_  (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_TRACE_  (int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, facility, format, args); va_end(args); }

inline void fLOG_IF_(bool condition, int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); _log_printf_msg_rts_if(condition, LOG_CHANNEL, severity, facility, format, args); va_end(args); }

inline void fLOGF_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGE_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGW_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGI_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGV_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGD_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOGT_IF_       (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }

inline void fLOG_FATAL_IF_  (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_ERROR_IF_  (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_WARNING_IF_(bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_INFO_IF_   (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_VERBOSE_IF_(bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_DEBUG_IF_  (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
inline void fLOG_TRACE_IF_  (bool condition, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, facility, format, args); va_end(args); }
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C++ Legacy PrintF channel LOG - cLOGxxx
//
inline void cLOG_(void* channel, int severity, const char* format, ...)                    { va_list args; va_start(args, format); _log_printf_msg_rts_uc(channel, severity, LOG_FACILITY, format, args); va_end(args); }

inline void cLOGF_       (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGE_       (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGW_       (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGI_       (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGV_       (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGD_       (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGT_       (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, LOG_FACILITY, format, args); va_end(args); }

inline void cLOG_FATAL_  (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_ERROR_  (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_WARNING_(void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_INFO_   (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_VERBOSE_(void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_DEBUG_  (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_TRACE_  (void* channel, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, LOG_FACILITY, format, args); va_end(args); }

inline void cLOG_IF_(bool condition, void* channel, int severity, const char* format, ...) { va_list args; va_start(args, format); _log_printf_msg_rts_if(condition, channel, severity, LOG_FACILITY, format, args); va_end(args); }

inline void cLOGF_IF_       (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGE_IF_       (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGW_IF_       (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGI_IF_       (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGV_IF_       (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGD_IF_       (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOGT_IF_       (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }

inline void cLOG_FATAL_IF_  (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_ERROR_IF_  (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_WARNING_IF_(bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_INFO_IF_   (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_VERBOSE_IF_(bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_DEBUG_IF_  (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
inline void cLOG_TRACE_IF_  (bool condition, void* channel, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, LOG_FACILITY, format, args); va_end(args); }
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C++ Legacy PrintF general LOG - gLOGxxx
//
inline void gLOG_(void* channel, int severity, int facility, const char* format, ...)                    { va_list args; va_start(args, format); _log_printf_msg_rts_uc(channel, severity, facility, format, args); va_end(args); }

inline void gLOGF_       (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, facility, format, args); va_end(args); }
inline void gLOGE_       (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, facility, format, args); va_end(args); }
inline void gLOGW_       (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, facility, format, args); va_end(args); }
inline void gLOGI_       (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, facility, format, args); va_end(args); }
inline void gLOGV_       (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, facility, format, args); va_end(args); }
inline void gLOGD_       (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, facility, format, args); va_end(args); }
inline void gLOGT_       (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, facility, format, args); va_end(args); }

inline void gLOG_FATAL_  (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, facility, format, args); va_end(args); }
inline void gLOG_ERROR_  (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, facility, format, args); va_end(args); }
inline void gLOG_WARNING_(void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, facility, format, args); va_end(args); }
inline void gLOG_INFO_   (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, facility, format, args); va_end(args); }
inline void gLOG_VERBOSE_(void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, facility, format, args); va_end(args); }
inline void gLOG_DEBUG_  (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, facility, format, args); va_end(args); }
inline void gLOG_TRACE_  (void* channel, int facility, const char* format, ...)                          { va_list args; va_start(args, format); _log_printf_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, facility, format, args); va_end(args); }

inline void gLOG_IF_(bool condition, void* channel, int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); _log_printf_msg_rts_if(condition, channel, severity, facility, format, args); va_end(args); }

inline void gLOGF_IF_       (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, facility, format, args); va_end(args); }
inline void gLOGE_IF_       (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, facility, format, args); va_end(args); }
inline void gLOGW_IF_       (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, facility, format, args); va_end(args); }
inline void gLOGI_IF_       (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, facility, format, args); va_end(args); }
inline void gLOGV_IF_       (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, facility, format, args); va_end(args); }
inline void gLOGD_IF_       (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, facility, format, args); va_end(args); }
inline void gLOGT_IF_       (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, facility, format, args); va_end(args); }

inline void gLOG_FATAL_IF_  (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, facility, format, args); va_end(args); }
inline void gLOG_ERROR_IF_  (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, facility, format, args); va_end(args); }
inline void gLOG_WARNING_IF_(bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, facility, format, args); va_end(args); }
inline void gLOG_INFO_IF_   (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, facility, format, args); va_end(args); }
inline void gLOG_VERBOSE_IF_(bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, facility, format, args); va_end(args); }
inline void gLOG_DEBUG_IF_  (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, facility, format, args); va_end(args); }
inline void gLOG_TRACE_IF_  (bool condition, void* channel, int facility, const char* format, ...)       { va_list args; va_start(args, format); _log_printf_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, facility, format, args); va_end(args); }
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#endif /*ndef GENERIC_LOG_SKIP_PRINTF*/

#ifndef GENERIC_LOG_SKIP_STREAM

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C++ Legacy Stream basic LOG - bLOGxxx
//
inline auto bLOG(int severity)                    { return _log_stream_msg_rts_uc(LOG_CHANNEL, severity, LOG_FACILITY); }

inline auto bLOGF       ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGE       ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGW       ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGI       ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGV       ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGD       ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGT       ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, LOG_FACILITY); }

inline auto bLOG_FATAL  ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_ERROR  ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_WARNING()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_INFO   ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_VERBOSE()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_DEBUG  ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_TRACE  ()                        { return _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, LOG_FACILITY); }

inline auto bLOG_IF(bool condition, int severity) { return _log_stream_msg_rts_if(condition, LOG_CHANNEL, severity, LOG_FACILITY); }

inline auto bLOGF_IF        (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGE_IF        (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGW_IF        (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGI_IF        (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGV_IF        (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGD_IF        (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOGT_IF        (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, LOG_FACILITY); }

inline auto bLOG_FATAL_IF   (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_ERROR_IF   (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_WARNING_IF (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_INFO_IF    (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_VERBOSE_IF (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_DEBUG_IF   (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, LOG_FACILITY); }
inline auto bLOG_TRACE_IF   (bool condition)      { return _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, LOG_FACILITY); }
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C++ Legacy Stream facility LOG - fLOGxxx
//
inline auto fLOG(int severity, int facility)                    { return _log_stream_msg_rts_uc(LOG_CHANNEL, severity, facility); }

inline auto fLOGF       (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, facility); }
inline auto fLOGE       (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, facility); }
inline auto fLOGW       (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, facility); }
inline auto fLOGI       (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, facility); }
inline auto fLOGV       (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, facility); }
inline auto fLOGD       (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, facility); }
inline auto fLOGT       (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, facility); }

inline auto fLOG_FATAL  (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(LOG_CHANNEL, facility); }
inline auto fLOG_ERROR  (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(LOG_CHANNEL, facility); }
inline auto fLOG_WARNING(int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(LOG_CHANNEL, facility); }
inline auto fLOG_INFO   (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(LOG_CHANNEL, facility); }
inline auto fLOG_VERBOSE(int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(LOG_CHANNEL, facility); }
inline auto fLOG_DEBUG  (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(LOG_CHANNEL, facility); }
inline auto fLOG_TRACE  (int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(LOG_CHANNEL, facility); }

inline auto fLOG_IF(bool condition, int severity, int facility) { return _log_stream_msg_rts_if(condition, LOG_CHANNEL, severity, facility); }

inline auto fLOGF_IF        (bool condition, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, facility); }
inline auto fLOGE_IF        (bool condition, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, facility); }
inline auto fLOGW_IF        (bool condition, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, facility); }
inline auto fLOGI_IF        (bool condition, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, facility); }
inline auto fLOGV_IF        (bool condition, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, facility); }
inline auto fLOGD_IF        (bool condition, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, facility); }
inline auto fLOGT_IF        (bool condition, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, facility); }

inline auto fLOG_FATAL_IF  (bool condition, int facility)       { return _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_CHANNEL, facility); }
inline auto fLOG_ERROR_IF  (bool condition, int facility)       { return _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_CHANNEL, facility); }
inline auto fLOG_WARNING_IF(bool condition, int facility)       { return _log_stream_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, LOG_CHANNEL, facility); }
inline auto fLOG_INFO_IF   (bool condition, int facility)       { return _log_stream_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, LOG_CHANNEL, facility); }
inline auto fLOG_VERBOSE_IF(bool condition, int facility)       { return _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_CHANNEL, facility); }
inline auto fLOG_DEBUG_IF  (bool condition, int facility)       { return _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_CHANNEL, facility); }
inline auto fLOG_TRACE_IF  (bool condition, int facility)       { return _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_CHANNEL, facility); }
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C++ Legacy Stream channel LOG - cLOGxxx
//
inline auto cLOG(void* channel, int severity)                    { return _log_stream_msg_rts_uc(channel, severity, LOG_FACILITY); }

inline auto cLOGF       (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, LOG_FACILITY); }
inline auto cLOGE       (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, LOG_FACILITY); }
inline auto cLOGW       (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, LOG_FACILITY); }
inline auto cLOGI       (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, LOG_FACILITY); }
inline auto cLOGV       (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, LOG_FACILITY); }
inline auto cLOGD       (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, LOG_FACILITY); }
inline auto cLOGT       (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, LOG_FACILITY); }

inline auto cLOG_FATAL  (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, LOG_FACILITY); }
inline auto cLOG_ERROR  (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, LOG_FACILITY); }
inline auto cLOG_WARNING(void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, LOG_FACILITY); }
inline auto cLOG_INFO   (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, LOG_FACILITY); }
inline auto cLOG_VERBOSE(void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, LOG_FACILITY); }
inline auto cLOG_DEBUG  (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, LOG_FACILITY); }
inline auto cLOG_TRACE  (void* channel)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, LOG_FACILITY); }

inline auto cLOG_IF(bool condition, void* channel, int severity) { return _log_stream_msg_rts_if(condition, channel, severity, LOG_FACILITY); }

inline auto cLOGF_IF        (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, LOG_FACILITY); }
inline auto cLOGE_IF        (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, LOG_FACILITY); }
inline auto cLOGW_IF        (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, LOG_FACILITY); }
inline auto cLOGI_IF        (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, LOG_FACILITY); }
inline auto cLOGV_IF        (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, LOG_FACILITY); }
inline auto cLOGD_IF        (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, LOG_FACILITY); }
inline auto cLOGT_IF        (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, LOG_FACILITY); }

inline auto cLOG_FATAL_IF   (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, LOG_FACILITY); }
inline auto cLOG_ERROR_IF   (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, LOG_FACILITY); }
inline auto cLOG_WARNING_IF (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, LOG_FACILITY); }
inline auto cLOG_INFO_IF    (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, LOG_FACILITY); }
inline auto cLOG_VERBOSE_IF (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, LOG_FACILITY); }
inline auto cLOG_DEBUG_IF   (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, LOG_FACILITY); }
inline auto cLOG_TRACE_IF   (bool condition, void* channel)      { return _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, LOG_FACILITY); }
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C++ Legacy Stream general LOG - gLOGxxx
//
inline auto gLOG(void* channel, int severity, int facility)                    { return _log_stream_msg_rts_uc(channel, severity, facility); }

inline auto gLOGF       (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, facility); }
inline auto gLOGE       (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, facility); }
inline auto gLOGW       (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, facility); }
inline auto gLOGI       (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, facility); }
inline auto gLOGV       (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, facility); }
inline auto gLOGD       (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, facility); }
inline auto gLOGT       (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, facility); }

inline auto gLOG_FATAL  (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(channel, facility); }
inline auto gLOG_ERROR  (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(channel, facility); }
inline auto gLOG_WARNING(void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(channel, facility); }
inline auto gLOG_INFO   (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(channel, facility); }
inline auto gLOG_VERBOSE(void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(channel, facility); }
inline auto gLOG_DEBUG  (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(channel, facility); }
inline auto gLOG_TRACE  (void* channel, int facility)                          { return _log_stream_msg_cts_uc<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(channel, facility); }

inline auto gLOG_IF(bool condition, void* channel, int severity, int facility) { return _log_stream_msg_rts_if(condition, channel, severity, facility); }

inline auto gLOGF_IF        (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, facility); }
inline auto gLOGE_IF        (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, facility); }
inline auto gLOGW_IF        (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, facility); }
inline auto gLOGI_IF        (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, facility); }
inline auto gLOGV_IF        (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, facility); }
inline auto gLOGD_IF        (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, facility); }
inline auto gLOGT_IF        (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, facility); }

inline auto gLOG_FATAL_IF   (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_FATAL  , _log_threshold(LOG_SEVERITY_FATAL  )>(condition, channel, facility); }
inline auto gLOG_ERROR_IF   (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_ERROR  , _log_threshold(LOG_SEVERITY_ERROR  )>(condition, channel, facility); }
inline auto gLOG_WARNING_IF (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_WARNING, _log_threshold(LOG_SEVERITY_WARNING)>(condition, channel, facility); }
inline auto gLOG_INFO_IF    (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_INFO   , _log_threshold(LOG_SEVERITY_INFO   )>(condition, channel, facility); }
inline auto gLOG_VERBOSE_IF (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_VERBOSE, _log_threshold(LOG_SEVERITY_VERBOSE)>(condition, channel, facility); }
inline auto gLOG_DEBUG_IF   (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_DEBUG  , _log_threshold(LOG_SEVERITY_DEBUG  )>(condition, channel, facility); }
inline auto gLOG_TRACE_IF   (bool condition, void* channel, int facility)      { return _log_stream_msg_cts_if<LOG_SEVERITY_TRACE  , _log_threshold(LOG_SEVERITY_TRACE  )>(condition, channel, facility); }
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#endif /*ndef GENERIC_LOG_SKIP_STREAM*/

#endif /*!defined(GENERIC_LOG_USE_C_DEFS) && defined(GENERIC_LOG_USE_CPP_LEGACY)*/
#endif /*ndef GENERIC_OLD_HPP*/
