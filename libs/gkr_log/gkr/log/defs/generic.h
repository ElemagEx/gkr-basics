#pragma once

#include <gkr/log/log.h>

enum {
    LOG_SEVERITY_FATAL   = 0,
    LOG_SEVERITY_ERROR   = 1,
    LOG_SEVERITY_WARN    = 2,
    LOG_SEVERITY_INFO    = 3,
    LOG_SEVERITY_VERBOSE = 4,
    LOG_SEVERITY_DEBUG   = 5,
    LOG_SEVERITY_TRACE   = 6,
};

#define LOG_STR_FATAL   "FATAL"
#define LOG_STR_ERROR   "ERROR"
#define LOG_STR_WARN    "WARN"
#define LOG_STR_INFO    "INFO"
#define LOG_STR_VERBOSE "VERBOSE"
#define LOG_STR_DEBUG   "DEBUG"
#define LOG_STR_TRACE   "TRACE"

#define LOG_SEVERITIES { \
    LOG_STR_FATAL  , \
    LOG_STR_ERROR  , \
    LOG_STR_WARN   , \
    LOG_STR_INFO   , \
    LOG_STR_VERBOSE, \
    LOG_STR_DEBUG  , \
    LOG_STR_TRACE  , \
    };

#define LOG_SEVERITIES_INFOS { \
    {LOG_STR_FATAL  , LOG_SEVERITY_FATAL  }, \
    {LOG_STR_ERROR  , LOG_SEVERITY_ERROR  }, \
    {LOG_STR_WARN   , LOG_SEVERITY_WARN   }, \
    {LOG_STR_INFO   , LOG_SEVERITY_INFO   }, \
    {LOG_STR_VERBOSE, LOG_SEVERITY_VERBOSE}, \
    {LOG_STR_DEBUG  , LOG_SEVERITY_DEBUG  }, \
    {LOG_STR_TRACE  , LOG_SEVERITY_TRACE  }, \
    {nullptr, 0}}

#if !defined(__cplusplus) || defined(GKR_LOG_USE_C_DEFS)

#define LOG(severity, facility, message)    gkr_log_simple_message(severity            , facility, message)

#define LOGF(facility, message)             gkr_log_simple_message(LOG_SEVERITY_FATAL  , facility, message)
#define LOGE(facility, message)             gkr_log_simple_message(LOG_SEVERITY_ERROR  , facility, message)
#define LOGW(facility, message)             gkr_log_simple_message(LOG_SEVERITY_WARN   , facility, message)
#define LOGI(facility, message)             gkr_log_simple_message(LOG_SEVERITY_INFO   , facility, message)
#define LOGV(facility, message)             gkr_log_simple_message(LOG_SEVERITY_VERBOSE, facility, message)
#define LOGD(facility, message)             gkr_log_simple_message(LOG_SEVERITY_DEBUG  , facility, message)
#define LOGT(facility, message)             gkr_log_simple_message(LOG_SEVERITY_TRACE  , facility, message)

#define LOG_FATAL(  facility, message)      gkr_log_simple_message(LOG_SEVERITY_FATAL  , facility, message)
#define LOG_ERROR(  facility, message)      gkr_log_simple_message(LOG_SEVERITY_ERROR  , facility, message)
#define LOG_WARN(   facility, message)      gkr_log_simple_message(LOG_SEVERITY_WARN   , facility, message)
#define LOG_INFO(   facility, message)      gkr_log_simple_message(LOG_SEVERITY_INFO   , facility, message)
#define LOG_VERBOSE(facility, message)      gkr_log_simple_message(LOG_SEVERITY_VERBOSE, facility, message)
#define LOG_DEBUG(  facility, message)      gkr_log_simple_message(LOG_SEVERITY_DEBUG  , facility, message)
#define LOG_TRACE(  facility, message)      gkr_log_simple_message(LOG_SEVERITY_TRACE  , facility, message)

#define LOG_IF(condition, severity, facility, message)  ((!(condition)) ? 0 : gkr_log_simple_message(severity            , facility, message))

#define LOGF_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_FATAL  , facility, message))
#define LOGE_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_ERROR  , facility, message))
#define LOGW_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_WARN   , facility, message))
#define LOGI_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_INFO   , facility, message))
#define LOGV_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_VERBOSE, facility, message))
#define LOGD_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_DEBUG  , facility, message))
#define LOGT_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_TRACE  , facility, message))

#define LOG_FATAL_IF(  condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_FATAL  , facility, message))
#define LOG_ERROR_IF(  condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_ERROR  , facility, message))
#define LOG_WARN_IF(   condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_WARN   , facility, message))
#define LOG_INFO_IF(   condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_INFO   , facility, message))
#define LOG_VERBOSE_IF(condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_VERBOSE, facility, message))
#define LOG_DEBUG_IF(  condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_DEBUG  , facility, message))
#define LOG_TRACE_IF(  condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(LOG_SEVERITY_TRACE  , facility, message))

#ifndef GKR_NO_PRINTF_LOGGING

#define LOG_(severity, facility, format, ...)   gkr_log_printf_message(severity            , facility, format, __VA_ARGS__)

#define LOGF_(facility, format, ...)            gkr_log_printf_message(LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define LOGE_(facility, format, ...)            gkr_log_printf_message(LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define LOGW_(facility, format, ...)            gkr_log_printf_message(LOG_SEVERITY_WARN   , facility, format, __VA_ARGS__)
#define LOGI_(facility, format, ...)            gkr_log_printf_message(LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define LOGV_(facility, format, ...)            gkr_log_printf_message(LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define LOGD_(facility, format, ...)            gkr_log_printf_message(LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define LOGT_(facility, format, ...)            gkr_log_printf_message(LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define LOG_FATAL_(  facility, message, ...)    gkr_log_printf_message(LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define LOG_ERROR_(  facility, message, ...)    gkr_log_printf_message(LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define LOG_WARN_(   facility, message, ...)    gkr_log_printf_message(LOG_SEVERITY_WARN   , facility, format, __VA_ARGS__)
#define LOG_INFO_(   facility, message, ...)    gkr_log_printf_message(LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define LOG_VERBOSE_(facility, message, ...)    gkr_log_printf_message(LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define LOG_DEBUG_(  facility, message, ...)    gkr_log_printf_message(LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define LOG_TRACE_(  facility, message, ...)    gkr_log_printf_message(LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define LOG_IF_(condition, severity, facility, format, ...)      ((!(condition)) ? 0 : gkr_log_printf_message(severity            , facility, format, __VA_ARGS__));

#define LOGF_IF_(condition, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__))
#define LOGE_IF_(condition, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__))
#define LOGW_IF_(condition, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_WARN   , facility, format, __VA_ARGS__))
#define LOGI_IF_(condition, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__))
#define LOGV_IF_(condition, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__))
#define LOGD_IF_(condition, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__))
#define LOGT_IF_(condition, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__))

#define LOG_FATAL_IF_(  condition, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__))
#define LOG_ERROR_IF_(  condition, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__))
#define LOG_WARN_IF_(   condition, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_WARN   , facility, format, __VA_ARGS__))
#define LOG_INFO_IF_(   condition, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__))
#define LOG_VERBOSE_IF_(condition, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__))
#define LOG_DEBUG_IF_(  condition, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__))
#define LOG_TRACE_IF_(  condition, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__))

#endif /*GKR_NO_PRINTF_LOGGING*/

#endif /*!defined(__cplusplus) || defined(GKR_LOG_USE_C_DEFS)*/
