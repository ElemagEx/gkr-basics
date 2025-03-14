#pragma once

#ifndef GKR_LOG_GENERIC
#error  You cannot include this header directly - define LOG_FACILITY as desired facility and then include "generic.h"
#endif

#ifndef LOG_FACILITY
#define LOG_FACILITY 0
#endif

#if !defined(__cplusplus) || defined(LOG_USE_C_DEFS)

#define  LOG(                        severity, message) GKR_LOG_X_SIMPLE_MSG(              LOG_INSTANCE, severity, LOG_FACILITY, message)
#define  LOG_IF(condition,           severity, message) GKR_LOG_X_SIMPLE_MSG_IF(condition, LOG_INSTANCE, severity, LOG_FACILITY, message)
#define CLOG(              instance, severity, message) GKR_LOG_X_SIMPLE_MSG(                  instance, severity, LOG_FACILITY, message)
#define CLOG_IF(condition, instance, severity, message) GKR_LOG_X_SIMPLE_MSG_IF(condition,     instance, severity, LOG_FACILITY, message)

#define LOGF(message)             GKR_LOG_F_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define LOGE(message)             GKR_LOG_E_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define LOGW(message)             GKR_LOG_W_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define LOGI(message)             GKR_LOG_I_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define LOGV(message)             GKR_LOG_V_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define LOGD(message)             GKR_LOG_D_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define LOGT(message)             GKR_LOG_T_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define LOG_FATAL(  message)      GKR_LOG_F_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define LOG_ERROR(  message)      GKR_LOG_E_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define LOG_WARNING(message)      GKR_LOG_W_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define LOG_INFO(   message)      GKR_LOG_I_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define LOG_VERBOSE(message)      GKR_LOG_V_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define LOG_DEBUG(  message)      GKR_LOG_D_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define LOG_TRACE(  message)      GKR_LOG_T_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define LOGF_IF(condition, message)           GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define LOGE_IF(condition, message)           GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define LOGW_IF(condition, message)           GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define LOGI_IF(condition, message)           GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define LOGV_IF(condition, message)           GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define LOGD_IF(condition, message)           GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define LOGT_IF(condition, message)           GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define LOG_FATAL_IF(  condition, message)    GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define LOG_ERROR_IF(  condition, message)    GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define LOG_WARNING_IF(condition, message)    GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define LOG_INFO_IF(   condition, message)    GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define LOG_VERBOSE_IF(condition, message)    GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define LOG_DEBUG_IF(  condition, message)    GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define LOG_TRACE_IF(  condition, message)    GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define CLOGF(instance, message)          GKR_LOG_F_SIMPLE_MSG(instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define CLOGE(instance, message)          GKR_LOG_E_SIMPLE_MSG(instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define CLOGW(instance, message)          GKR_LOG_W_SIMPLE_MSG(instance, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define CLOGI(instance, message)          GKR_LOG_I_SIMPLE_MSG(instance, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define CLOGV(instance, message)          GKR_LOG_V_SIMPLE_MSG(instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define CLOGD(instance, message)          GKR_LOG_D_SIMPLE_MSG(instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define CLOGT(instance, message)          GKR_LOG_T_SIMPLE_MSG(instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define CLOG_FATAL(  instance, message)   GKR_LOG_F_SIMPLE_MSG(instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define CLOG_ERROR(  instance, message)   GKR_LOG_E_SIMPLE_MSG(instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define CLOG_WARNING(instance, message)   GKR_LOG_W_SIMPLE_MSG(instance, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define CLOG_INFO(   instance, message)   GKR_LOG_I_SIMPLE_MSG(instance, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define CLOG_VERBOSE(instance, message)   GKR_LOG_V_SIMPLE_MSG(instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define CLOG_DEBUG(  instance, message)   GKR_LOG_D_SIMPLE_MSG(instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define CLOG_TRACE(  instance, message)   GKR_LOG_T_SIMPLE_MSG(instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define CLOGF_IF(condition, instance, message)            GKR_LOG_F_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define CLOGE_IF(condition, instance, message)            GKR_LOG_E_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define CLOGW_IF(condition, instance, message)            GKR_LOG_W_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define CLOGI_IF(condition, instance, message)            GKR_LOG_I_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define CLOGV_IF(condition, instance, message)            GKR_LOG_V_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define CLOGD_IF(condition, instance, message)            GKR_LOG_D_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define CLOGT_IF(condition, instance, message)            GKR_LOG_T_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define CLOG_FATAL_IF(  condition, instance, message)     GKR_LOG_F_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define CLOG_ERROR_IF(  condition, instance, message)     GKR_LOG_E_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define CLOG_WARNING_IF(condition, instance, message)     GKR_LOG_W_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define CLOG_INFO_IF(   condition, instance, message)     GKR_LOG_I_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define CLOG_VERBOSE_IF(condition, instance, message)     GKR_LOG_V_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define CLOG_DEBUG_IF(  condition, instance, message)     GKR_LOG_D_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define CLOG_TRACE_IF(  condition, instance, message)     GKR_LOG_T_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#ifndef GKR_NO_PRINTF_LOGGING

#define LOG_(                         severity, format, ...) GKR_LOG_X_PRINTF_MSG(              LOG_INSTANCE, severity, LOG_FACILITY, format, __VA_ARGS__)
#define LOG_IF_(condition,            severity, format, ...) GKR_LOG_X_PRINTF_MSG_IF(condition, LOG_INSTANCE, severity, LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_(              instance, severity, format, ...) GKR_LOG_X_PRINTF_MSG(                  instance, severity, LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_IF_(condition, instance, severity, format, ...) GKR_LOG_X_PRINTF_MSG_IF(condition,     instance, severity, LOG_FACILITY, format, __VA_ARGS__)

#define LOGF_(format, ...)            GKR_LOG_F_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define LOGE_(format, ...)            GKR_LOG_E_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define LOGW_(format, ...)            GKR_LOG_W_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define LOGI_(format, ...)            GKR_LOG_I_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define LOGV_(format, ...)            GKR_LOG_V_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define LOGD_(format, ...)            GKR_LOG_D_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define LOGT_(format, ...)            GKR_LOG_T_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define LOG_FATAL_(  message, ...)    GKR_LOG_F_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define LOG_ERROR_(  message, ...)    GKR_LOG_E_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define LOG_WARNING_(message, ...)    GKR_LOG_W_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define LOG_INFO_(   message, ...)    GKR_LOG_I_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define LOG_VERBOSE_(message, ...)    GKR_LOG_V_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define LOG_DEBUG_(  message, ...)    GKR_LOG_D_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define LOG_TRACE_(  message, ...)    GKR_LOG_T_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define LOGF_IF_(condition, format, ...)          GKR_LOG_F_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define LOGE_IF_(condition, format, ...)          GKR_LOG_E_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define LOGW_IF_(condition, format, ...)          GKR_LOG_W_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define LOGI_IF_(condition, format, ...)          GKR_LOG_I_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define LOGV_IF_(condition, format, ...)          GKR_LOG_V_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define LOGD_IF_(condition, format, ...)          GKR_LOG_D_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define LOGT_IF_(condition, format, ...)          GKR_LOG_T_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define LOG_FATAL_IF_(  condition, format, ...)   GKR_LOG_F_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define LOG_ERROR_IF_(  condition, format, ...)   GKR_LOG_E_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define LOG_WARNING_IF_(condition, format, ...)   GKR_LOG_W_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define LOG_INFO_IF_(   condition, format, ...)   GKR_LOG_I_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define LOG_VERBOSE_IF_(condition, format, ...)   GKR_LOG_V_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define LOG_DEBUG_IF_(  condition, format, ...)   GKR_LOG_D_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define LOG_TRACE_IF_(  condition, format, ...)   GKR_LOG_T_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define CLOGF_(instance, format, ...)             GKR_LOG_F_PRINTF_MSG(instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOGE_(instance, format, ...)             GKR_LOG_E_PRINTF_MSG(instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOGW_(instance, format, ...)             GKR_LOG_W_PRINTF_MSG(instance, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define CLOGI_(instance, format, ...)             GKR_LOG_I_PRINTF_MSG(instance, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define CLOGV_(instance, format, ...)             GKR_LOG_V_PRINTF_MSG(instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define CLOGD_(instance, format, ...)             GKR_LOG_D_PRINTF_MSG(instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOGT_(instance, format, ...)             GKR_LOG_T_PRINTF_MSG(instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define CLOG_FATAL_(  instance, message, ...)     GKR_LOG_F_PRINTF_MSG(instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_ERROR_(  instance, message, ...)     GKR_LOG_E_PRINTF_MSG(instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_WARNING_(instance, message, ...)     GKR_LOG_W_PRINTF_MSG(instance, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_INFO_(   instance, message, ...)     GKR_LOG_I_PRINTF_MSG(instance, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_VERBOSE_(instance, message, ...)     GKR_LOG_V_PRINTF_MSG(instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_DEBUG_(  instance, message, ...)     GKR_LOG_D_PRINTF_MSG(instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_TRACE_(  instance, message, ...)     GKR_LOG_T_PRINTF_MSG(instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define CLOGF_IF_(condition, instance, format, ...)           GKR_LOG_F_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOGE_IF_(condition, instance, format, ...)           GKR_LOG_E_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOGW_IF_(condition, instance, format, ...)           GKR_LOG_W_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define CLOGI_IF_(condition, instance, format, ...)           GKR_LOG_I_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define CLOGV_IF_(condition, instance, format, ...)           GKR_LOG_V_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define CLOGD_IF_(condition, instance, format, ...)           GKR_LOG_D_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOGT_IF_(condition, instance, format, ...)           GKR_LOG_T_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define CLOG_FATAL_IF_(  condition, instance, format, ...)    GKR_LOG_F_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_ERROR_IF_(  condition, instance, format, ...)    GKR_LOG_E_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_WARNING_IF_(condition, instance, format, ...)    GKR_LOG_W_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_INFO_IF_(   condition, instance, format, ...)    GKR_LOG_I_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_VERBOSE_IF_(condition, instance, format, ...)    GKR_LOG_V_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_DEBUG_IF_(  condition, instance, format, ...)    GKR_LOG_D_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define CLOG_TRACE_IF_(  condition, instance, format, ...)    GKR_LOG_T_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#endif /*!defined(__cplusplus) || defined(LOG_USE_C_DEFS)*/
