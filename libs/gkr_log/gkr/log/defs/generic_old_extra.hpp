#pragma once

#ifndef GKR_LOG_GENERIC_X
#error  You cannot include this header directly - ensure LOG_FACILITY is not defined and LOG_CPP_LEGACY is defined (or C++ if constexpr is not available) and then include "generic.hpp" 
#endif

#ifdef  LOG_USE_C_DEFS
#error  This should not be defined
#endif

inline void LOG    (                                int facility, const char* msg) { gkr::log::simple_message_rts   (           LOG_INSTANCE, severity, facility, msg); }
inline void LOG_IF (bool condition,                 int facility, const char* msg) { gkr::log::simple_message_rts_if(condition, LOG_INSTANCE, severity, facility, msg); }
inline void CLOG   (                void* instance, int facility, const char* msg) { gkr::log::simple_message_rts   (               instance, severity, facility, msg); }
inline void CLOG_IF(bool condition, void* instance, int facility, const char* msg) { gkr::log::simple_message_rts_if(condition,     instance, severity, facility, msg); }

inline void LOGF       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, msg); }
inline void LOGE       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, msg); }
inline void LOGW       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, msg); }
inline void LOGI       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, msg); }
inline void LOGV       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, msg); }
inline void LOGD       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, msg); }
inline void LOGT       (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, msg); }

inline void LOG_FATAL  (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, msg); }
inline void LOG_ERROR  (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, msg); }
inline void LOG_WARNING(int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, msg); }
inline void LOG_INFO   (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, msg); }
inline void LOG_VERBOSE(int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, msg); }
inline void LOG_DEBUG  (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, msg); }
inline void LOG_TRACE  (int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, msg); }

inline void LOGF_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, msg); }
inline void LOGE_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, msg); }
inline void LOGW_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, msg); }
inline void LOGI_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, msg); }
inline void LOGV_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, msg); }
inline void LOGD_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, msg); }
inline void LOGT_IF        (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, msg); }

inline void LOG_FATAL_IF   (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_ERROR_IF   (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_WARNING_IF (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_INFO_IF    (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_VERBOSE_IF (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_DEBUG_IF   (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, msg); }
inline void LOG_TRACE_IF   (bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, msg); }

inline void CLOGF       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, msg); }
inline void CLOGE       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, msg); }
inline void CLOGW       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, msg); }
inline void CLOGI       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, msg); }
inline void CLOGV       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, msg); }
inline void CLOGD       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, msg); }
inline void CLOGT       (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, msg); }

inline void CLOG_FATAL  (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, msg); }
inline void CLOG_ERROR  (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, msg); }
inline void CLOG_WARNING(void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, msg); }
inline void CLOG_INFO   (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, msg); }
inline void CLOG_VERBOSE(void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, msg); }
inline void CLOG_DEBUG  (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, msg); }
inline void CLOG_TRACE  (void* instance, int facility, const char* msg) { gkr::log::simple_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, msg); }

inline void CLOGF_IF        (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, msg); }
inline void CLOGE_IF        (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, msg); }
inline void CLOGW_IF        (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, msg); }
inline void CLOGI_IF        (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, msg); }
inline void CLOGV_IF        (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, msg); }
inline void CLOGD_IF        (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, msg); }
inline void CLOGT_IF        (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, msg); }

inline void CLOG_FATAL_IF   (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, msg); }
inline void CLOG_ERROR_IF   (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, msg); }
inline void CLOG_WARNING_IF (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, msg); }
inline void CLOG_INFO_IF    (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, msg); }
inline void CLOG_VERBOSE_IF (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, msg); }
inline void CLOG_DEBUG_IF   (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, msg); }
inline void CLOG_TRACE_IF   (void* instance, bool condition, int facility, const char* msg) { gkr::log::simple_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, msg); }

#ifndef GKR_NO_PRINTF_LOGGING

inline void LOG_    (                                int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts   (           LOG_INSTANCE, severity, facility, format, args); va_end(args); }
inline void LOG_IF_ (bool condition,                 int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts_if(condition, LOG_INSTANCE, severity, facility, format, args); va_end(args); }
inline void CLOG_   (                void* instance, int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts   (               instance, severity, facility, format, args); va_end(args); }
inline void CLOG_IF_(bool condition, void* instance, int severity, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_rts_if(condition,     instance, severity, facility, format, args); va_end(args); }

inline void LOGF_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGE_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGW_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGI_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGV_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGD_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGT_       (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, format, args); va_end(args); }

inline void LOG_FATAL_  (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_ERROR_  (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_WARNING_(int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_INFO_   (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_VERBOSE_(int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_DEBUG_  (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_TRACE_  (int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility, format, args); va_end(args); }

inline void LOGF_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGE_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGW_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGI_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGV_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGD_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOGT_IF_       (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }

inline void LOG_FATAL_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_ERROR_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_WARNING_IF_(bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_INFO_IF_   (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_VERBOSE_IF_(bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_DEBUG_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }
inline void LOG_TRACE_IF_  (bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility, format, args); va_end(args); }

inline void CLOGF_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, format, args); va_end(args); }
inline void CLOGE_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, format, args); va_end(args); }
inline void CLOGW_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, format, args); va_end(args); }
inline void CLOGI_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, format, args); va_end(args); }
inline void CLOGV_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, format, args); va_end(args); }
inline void CLOGD_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, format, args); va_end(args); }
inline void CLOGT_       (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, format, args); va_end(args); }

inline void CLOG_FATAL_  (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility, format, args); va_end(args); }
inline void CLOG_ERROR_  (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility, format, args); va_end(args); }
inline void CLOG_WARNING_(void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility, format, args); va_end(args); }
inline void CLOG_INFO_   (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility, format, args); va_end(args); }
inline void CLOG_VERBOSE_(void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility, format, args); va_end(args); }
inline void CLOG_DEBUG_  (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility, format, args); va_end(args); }
inline void CLOG_TRACE_  (void* instance, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility, format, args); va_end(args); }

inline void CLOGF_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGE_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGW_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGI_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGV_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGD_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOGT_IF_       (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, format, args); va_end(args); }

inline void CLOG_FATAL_IF_  (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_ERROR_IF_  (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_WARNING_IF_(void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_INFO_IF_   (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_VERBOSE_IF_(void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_DEBUG_IF_  (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility, format, args); va_end(args); }
inline void CLOG_TRACE_IF_  (void* instance, bool condition, int facility, const char* format, ...) { va_list args; va_start(args, format); gkr::log::printf_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility, format, args); va_end(args); }

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#ifndef GKR_NO_STREAM_LOGGING

inline auto LOG    (                                int severity, int facility) { return gkr::log::stream_message_rts   (           LOG_INSTANCE, severity, facility); }
inline auto LOG_IF (bool condition,                 int severity, int facility) { return gkr::log::stream_message_rts_if(condition, LOG_INSTANCE, severity, facility); }
inline auto CLOG   (                void* instance, int severity, int facility) { return gkr::log::stream_message_rts   (               instance, severity, facility); }
inline auto CLOG_IF(bool condition, void* instance, int severity, int facility) { return gkr::log::stream_message_rts_if(condition,     instance, severity, facility); }

inline auto LOGF       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility); }
inline auto LOGE       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility); }
inline auto LOGW       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility); }
inline auto LOGI       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility); }
inline auto LOGV       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility); }
inline auto LOGD       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility); }
inline auto LOGT       (int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility); }

inline auto LOG_FATAL  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(LOG_INSTANCE, facility); }
inline auto LOG_ERROR  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(LOG_INSTANCE, facility); }
inline auto LOG_WARNING(int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(LOG_INSTANCE, facility); }
inline auto LOG_INFO   (int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(LOG_INSTANCE, facility); }
inline auto LOG_VERBOSE(int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(LOG_INSTANCE, facility); }
inline auto LOG_DEBUG  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(LOG_INSTANCE, facility); }
inline auto LOG_TRACE  (int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(LOG_INSTANCE, facility); }

inline auto LOGF_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility); }
inline auto LOGE_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility); }
inline auto LOGW_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility); }
inline auto LOGI_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility); }
inline auto LOGV_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility); }
inline auto LOGD_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility); }
inline auto LOGT_IF        (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility); }

inline auto LOG_FATAL_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, LOG_INSTANCE, facility); }
inline auto LOG_ERROR_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, LOG_INSTANCE, facility); }
inline auto LOG_WARNING_IF (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, LOG_INSTANCE, facility); }
inline auto LOG_INFO_IF    (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, LOG_INSTANCE, facility); }
inline auto LOG_VERBOSE_IF (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, LOG_INSTANCE, facility); }
inline auto LOG_DEBUG_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, LOG_INSTANCE, facility); }
inline auto LOG_TRACE_IF   (bool condition, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, LOG_INSTANCE, facility); }

inline auto CLOGF       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility); }
inline auto CLOGE       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility); }
inline auto CLOGW       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility); }
inline auto CLOGI       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility); }
inline auto CLOGV       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility); }
inline auto CLOGD       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility); }
inline auto CLOGT       (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility); }

inline auto CLOG_FATAL  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(instance, facility); }
inline auto CLOG_ERROR  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(instance, facility); }
inline auto CLOG_WARNING(void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(instance, facility); }
inline auto CLOG_INFO   (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(instance, facility); }
inline auto CLOG_VERBOSE(void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(instance, facility); }
inline auto CLOG_DEBUG  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(instance, facility); }
inline auto CLOG_TRACE  (void* instance, int facility) { return gkr::log::stream_message<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(instance, facility); }

inline auto CLOGF_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility); }
inline auto CLOGE_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility); }
inline auto CLOGW_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility); }
inline auto CLOGI_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility); }
inline auto CLOGV_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility); }
inline auto CLOGD_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility); }
inline auto CLOGT_IF        (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility); }

inline auto CLOG_FATAL_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_FATAL  , gkr::log::threshold(LOG_SEVERITY_FATAL  )>(condition, instance, facility); }
inline auto CLOG_ERROR_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_ERROR  , gkr::log::threshold(LOG_SEVERITY_ERROR  )>(condition, instance, facility); }
inline auto CLOG_WARNING_IF (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_WARNING, gkr::log::threshold(LOG_SEVERITY_WARNING)>(condition, instance, facility); }
inline auto CLOG_INFO_IF    (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_INFO   , gkr::log::threshold(LOG_SEVERITY_INFO   )>(condition, instance, facility); }
inline auto CLOG_VERBOSE_IF (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_VERBOSE, gkr::log::threshold(LOG_SEVERITY_VERBOSE)>(condition, instance, facility); }
inline auto CLOG_DEBUG_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_DEBUG  , gkr::log::threshold(LOG_SEVERITY_DEBUG  )>(condition, instance, facility); }
inline auto CLOG_TRACE_IF   (bool condition, void* instance, int facility) { return gkr::log::stream_message_if<LOG_SEVERITY_TRACE  , gkr::log::threshold(LOG_SEVERITY_TRACE  )>(condition, instance, facility); }

#endif /*ndef GKR_NO_STREAM_LOGGING*/
