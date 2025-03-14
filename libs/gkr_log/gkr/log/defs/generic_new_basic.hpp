#pragma once

#ifndef GKR_LOG_GENERIC_X
#error  You cannot include this header directly - define LOG_FACILITY as desired facility and ensure LOG_CPP_LEGACY is not defined (C++ if constexpr must be available) and then include "generic.hpp"
#endif

#ifdef  LOG_USE_C_DEFS
#error  This should not be defined
#endif

inline void  LOG   (                                int severity, const char* msg) { gkr::log::simple_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY, msg); }
inline void  LOG_IF(bool condition,                 int severity, const char* msg) { gkr::log::simple_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, msg); }
inline void CLOG   (                void* instance, int severity, const char* msg) { gkr::log::simple_message_rts   (               instance, severity, LOG_FACILITY, msg); }
inline void CLOG_IF(bool condition, void* instance, int severity, const char* msg) { gkr::log::simple_message_rts_if(condition,     instance, severity, LOG_FACILITY, msg); }

inline void LOGF       (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGE       (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGW       (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGI       (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGV       (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGD       (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGT       (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, msg); }

inline void LOG_FATAL  (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_ERROR  (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_WARNING(const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_INFO   (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_VERBOSE(const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_DEBUG  (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_TRACE  (const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, msg); }

inline void LOGF_IF        (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGE_IF        (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGW_IF        (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGI_IF        (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGV_IF        (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGD_IF        (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOGT_IF        (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }

inline void LOG_FATAL_IF   (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_ERROR_IF   (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_WARNING_IF (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_INFO_IF    (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_VERBOSE_IF (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_DEBUG_IF   (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }
inline void LOG_TRACE_IF   (bool condition, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, msg); }

inline void CLOGF       (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, msg); }
inline void CLOGE       (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, msg); }
inline void CLOGW       (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, msg); }
inline void CLOGI       (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, msg); }
inline void CLOGV       (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, msg); }
inline void CLOGD       (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, msg); }
inline void CLOGT       (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, msg); }

inline void CLOG_FATAL  (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, msg); }
inline void CLOG_ERROR  (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, msg); }
inline void CLOG_WARNING(void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, msg); }
inline void CLOG_INFO   (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, msg); }
inline void CLOG_VERBOSE(void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, msg); }
inline void CLOG_DEBUG  (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, msg); }
inline void CLOG_TRACE  (void* instance, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, msg); }

inline void CLOGF_IF        (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, msg); }
inline void CLOGE_IF        (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, msg); }
inline void CLOGW_IF        (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, msg); }
inline void CLOGI_IF        (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, msg); }
inline void CLOGV_IF        (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, msg); }
inline void CLOGD_IF        (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, msg); }
inline void CLOGT_IF        (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, msg); }

inline void CLOG_FATAL_IF   (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, msg); }
inline void CLOG_ERROR_IF   (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, msg); }
inline void CLOG_WARNING_IF (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, msg); }
inline void CLOG_INFO_IF    (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, msg); }
inline void CLOG_VERBOSE_IF (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, msg); }
inline void CLOG_DEBUG_IF   (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, msg); }
inline void CLOG_TRACE_IF   (bool condition, void* instance, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, msg); }

#ifndef GKR_NO_PRINTF_LOGGING

template<typename... Args> void LOG_    (                                int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_IF_ (bool condition,                 int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_   (                void* instance, int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts   (               instance, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_IF_(bool condition, void* instance, int severity, const char* format, Args&&... args) { gkr::log::printf_message_rts_if(condition,     instance, severity, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void LOGF_       (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGE_       (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGW_       (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGI_       (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGV_       (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGD_       (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGT_       (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_  (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_  (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARNING_(const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_   (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_(const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_  (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_  (const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF_       (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF_       (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF_       (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF_       (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF_       (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF_       (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF_       (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF_  (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF_  (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARNING_IF_(bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF_   (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF_(bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF_  (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF_  (bool condition, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_       (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_       (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_       (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_       (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_       (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_       (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_       (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_  (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_  (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARNING_(void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_   (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_(void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_  (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_  (void* instance, const char* format, Args&&... args) { gkr::log::printf_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_IF_       (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_IF_       (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_IF_       (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_IF_       (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_IF_       (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_IF_       (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_IF_       (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_IF_  (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_IF_  (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARNING_IF_(bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_IF_   (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_IF_(bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_IF_  (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_IF_  (bool condition, void* instance, const char* format, Args&&... args) { gkr::log::printf_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, format, std::forward<Args>(args)...); }

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

inline auto LOG    (                                int severity) { return gkr::log::stream_message_rts   (           LOG_INSTANCE, severity, LOG_FACILITY); }
inline auto LOG_IF (bool condition,                 int severity) { return gkr::log::stream_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY); }
inline auto CLOG   (                void* instance, int severity) { return gkr::log::stream_message_rts   (               instance, severity, LOG_FACILITY); }
inline auto CLOG_IF(bool condition, void* instance, int severity) { return gkr::log::stream_message_rts_if(condition,     instance, severity, LOG_FACILITY); }

inline auto LOGF       () { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGE       () { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGW       () { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGI       () { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGV       () { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGD       () { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGT       () { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY); }

inline auto LOG_FATAL  () { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_ERROR  () { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_WARNING() { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_INFO   () { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_VERBOSE() { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_DEBUG  () { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_TRACE  () { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY); }

inline auto LOGF_IF        (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGE_IF        (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGW_IF        (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGI_IF        (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGV_IF        (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGD_IF        (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOGT_IF        (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY); }

inline auto LOG_FATAL_IF   (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_ERROR_IF   (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_WARNING_IF (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_INFO_IF    (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_VERBOSE_IF (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_DEBUG_IF   (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY); }
inline auto LOG_TRACE_IF   (bool condition) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY); }

inline auto CLOGF       (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY); }
inline auto CLOGE       (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY); }
inline auto CLOGW       (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY); }
inline auto CLOGI       (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY); }
inline auto CLOGV       (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY); }
inline auto CLOGD       (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY); }
inline auto CLOGT       (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY); }

inline auto CLOG_FATAL  (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY); }
inline auto CLOG_ERROR  (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY); }
inline auto CLOG_WARNING(void* instance) { return gkr::log::stream_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY); }
inline auto CLOG_INFO   (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY); }
inline auto CLOG_VERBOSE(void* instance) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY); }
inline auto CLOG_DEBUG  (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY); }
inline auto CLOG_TRACE  (void* instance) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY); }

inline auto CLOGF_IF        (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY); }
inline auto CLOGE_IF        (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY); }
inline auto CLOGW_IF        (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY); }
inline auto CLOGI_IF        (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY); }
inline auto CLOGV_IF        (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY); }
inline auto CLOGD_IF        (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY); }
inline auto CLOGT_IF        (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY); }

inline auto CLOG_FATAL_IF   (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY); }
inline auto CLOG_ERROR_IF   (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY); }
inline auto CLOG_WARNING_IF (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY); }
inline auto CLOG_INFO_IF    (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY); }
inline auto CLOG_VERBOSE_IF (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY); }
inline auto CLOG_DEBUG_IF   (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY); }
inline auto CLOG_TRACE_IF   (bool condition, void* instance) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY); }

#endif /*ndef GKR_NO_STREAM_LOGGING*/

#ifndef GKR_NO_FORMAT_LOGGING
#ifdef __cpp_lib_format

template<typename... Args> void LOG    (                                int severity,                         std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts  (            LOG_INSTANCE, severity, LOG_FACILITY,      std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG    (                                int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts  (            LOG_INSTANCE, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_IF (bool condition,                 int severity,                         std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY,      std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_IF (bool condition,                 int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if(condition, LOG_INSTANCE, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG   (                void* instance, int severity,                         std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts   (               instance, severity, LOG_FACILITY,      std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG   (                void* instance, int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts   (               instance, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_IF(bool condition, void* instance, int severity,                         std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if((condition,    instance, severity, LOG_FACILITY,      std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_IF(bool condition, void* instance, int severity, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_rts_if((condition,    instance, severity, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF       (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE       (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW       (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI       (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV       (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD       (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT       (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL  (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR  (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARNING(std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO   (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE(std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG  (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE  (std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT       (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARNING(const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO   (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE(const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE  (const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(LOG_INSTANCE, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF       (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARN_IF   (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF   (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF(bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF  (bool condition, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOGF_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGE_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGW_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGI_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGV_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGD_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOGT_IF       (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void LOG_FATAL_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_ERROR_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_WARN_IF   (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_INFO_IF   (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_VERBOSE_IF(bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_DEBUG_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void LOG_TRACE_IF  (bool condition, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, LOG_INSTANCE, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF       (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE       (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW       (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI       (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV       (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD       (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT       (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL  (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR  (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARNING(void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO   (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE(void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG  (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE  (void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT       (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL  (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_FATAL  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR  (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_ERROR  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARNING(void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_WARNING>(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO   (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_INFO   >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE(void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_VERBOSE>(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG  (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_DEBUG  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE  (void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message<LOG_SEVERITY_TRACE  >(instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_IF       (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_IF  (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_IF  (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARN_IF   (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_IF   (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_IF(bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_IF  (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_IF  (bool condition, void* instance, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOGF_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGE_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGW_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGI_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGV_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGD_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOGT_IF       (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

template<typename... Args> void CLOG_FATAL_IF  (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_FATAL  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_ERROR_IF  (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_ERROR  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_WARN_IF   (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_WARNING>(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_INFO_IF   (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_INFO   >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_VERBOSE_IF(bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_VERBOSE>(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_DEBUG_IF  (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_DEBUG  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }
template<typename... Args> void CLOG_TRACE_IF  (bool condition, void* instance, const std::locale& loc, std::format_string<Args...> fmt, Args&&... args) { gkr::log::format_message_if<LOG_SEVERITY_TRACE  >(condition, instance, LOG_FACILITY, loc, std::move(fmt), std::forward<Args>(args)...); }

#endif /*def __cpp_lib_format*/
#endif /*ndef GKR_NO_FORMAT_LOGGING*/
