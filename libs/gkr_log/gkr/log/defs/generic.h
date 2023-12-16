#pragma once

#include <gkr/log/log.h>

enum {
    LOG_SEVERITY_FATAL   = 0,
    LOG_SEVERITY_ERROR   = 1,
    LOG_SEVERITY_WARNING = 2,
    LOG_SEVERITY_INFO    = 3,
    LOG_SEVERITY_VERBOSE = 4,
    LOG_SEVERITY_DEBUG   = 5,
    LOG_SEVERITY_TRACE   = 6,
};

#define LOG_STR_FATAL   "FATAL"
#define LOG_STR_ERROR   "ERROR"
#define LOG_STR_WARNING "WARNING"
#define LOG_STR_INFO    "INFO"
#define LOG_STR_VERBOSE "VERBOSE"
#define LOG_STR_DEBUG   "DEBUG"
#define LOG_STR_TRACE   "TRACE"

#define LOG_SEVERITIES_NAMES { \
    LOG_STR_FATAL  , \
    LOG_STR_ERROR  , \
    LOG_STR_WARNING, \
    LOG_STR_INFO   , \
    LOG_STR_VERBOSE, \
    LOG_STR_DEBUG  , \
    LOG_STR_TRACE  , \
    };

#define LOG_SEVERITIES_INFOS { \
    {LOG_STR_FATAL  , LOG_SEVERITY_FATAL  }, \
    {LOG_STR_ERROR  , LOG_SEVERITY_ERROR  }, \
    {LOG_STR_WARNING, LOG_SEVERITY_WARNING}, \
    {LOG_STR_INFO   , LOG_SEVERITY_INFO   }, \
    {LOG_STR_VERBOSE, LOG_SEVERITY_VERBOSE}, \
    {LOG_STR_DEBUG  , LOG_SEVERITY_DEBUG  }, \
    {LOG_STR_TRACE  , LOG_SEVERITY_TRACE  }, \
    {nullptr, 0}}

#if !defined(__cplusplus) || defined(GKR_LOG_USE_C_DEFS)

#define LOG(severity, facility, message)    gkr_log_simple_message(nullptr, severity            , facility, message)

#define LOGF(facility, message)             gkr_log_simple_message(nullptr, LOG_SEVERITY_FATAL  , facility, message)
#define LOGE(facility, message)             gkr_log_simple_message(nullptr, LOG_SEVERITY_ERROR  , facility, message)
#define LOGW(facility, message)             gkr_log_simple_message(nullptr, LOG_SEVERITY_WARNING, facility, message)
#define LOGI(facility, message)             gkr_log_simple_message(nullptr, LOG_SEVERITY_INFO   , facility, message)
#define LOGV(facility, message)             gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, facility, message)
#define LOGD(facility, message)             gkr_log_simple_message(nullptr, LOG_SEVERITY_DEBUG  , facility, message)
#define LOGT(facility, message)             gkr_log_simple_message(nullptr, LOG_SEVERITY_TRACE  , facility, message)

#define LOG_FATAL(  facility, message)      gkr_log_simple_message(nullptr, LOG_SEVERITY_FATAL  , facility, message)
#define LOG_ERROR(  facility, message)      gkr_log_simple_message(nullptr, LOG_SEVERITY_ERROR  , facility, message)
#define LOG_WARNING(facility, message)      gkr_log_simple_message(nullptr, LOG_SEVERITY_WARNING, facility, message)
#define LOG_INFO(   facility, message)      gkr_log_simple_message(nullptr, LOG_SEVERITY_INFO   , facility, message)
#define LOG_VERBOSE(facility, message)      gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, facility, message)
#define LOG_DEBUG(  facility, message)      gkr_log_simple_message(nullptr, LOG_SEVERITY_DEBUG  , facility, message)
#define LOG_TRACE(  facility, message)      gkr_log_simple_message(nullptr, LOG_SEVERITY_TRACE  , facility, message)

#define LOG_IF(condition, severity, facility, message)  ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, severity            , facility, message))

#define LOGF_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_FATAL  , facility, message))
#define LOGE_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_ERROR  , facility, message))
#define LOGW_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_WARNING, facility, message))
#define LOGI_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_INFO   , facility, message))
#define LOGV_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, facility, message))
#define LOGD_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_DEBUG  , facility, message))
#define LOGT_IF(condition, facility, message)           ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_TRACE  , facility, message))

#define LOG_FATAL_IF(  condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_FATAL  , facility, message))
#define LOG_ERROR_IF(  condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_ERROR  , facility, message))
#define LOG_WARNING_IF(condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_WARNING, facility, message))
#define LOG_INFO_IF(   condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_INFO   , facility, message))
#define LOG_VERBOSE_IF(condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, facility, message))
#define LOG_DEBUG_IF(  condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_DEBUG  , facility, message))
#define LOG_TRACE_IF(  condition, facility, message)    ((!(condition)) ? 0 : gkr_log_simple_message(nullptr, LOG_SEVERITY_TRACE  , facility, message))

#define CLOG(instance, severity, facility, message) gkr_log_simple_message(instance, severity            , facility, message)

#define CLOGF(instance, facility, message)          gkr_log_simple_message(instance, LOG_SEVERITY_FATAL  , facility, message)
#define CLOGE(instance, facility, message)          gkr_log_simple_message(instance, LOG_SEVERITY_ERROR  , facility, message)
#define CLOGW(instance, facility, message)          gkr_log_simple_message(instance, LOG_SEVERITY_WARNING, facility, message)
#define CLOGI(instance, facility, message)          gkr_log_simple_message(instance, LOG_SEVERITY_INFO   , facility, message)
#define CLOGV(instance, facility, message)          gkr_log_simple_message(instance, LOG_SEVERITY_VERBOSE, facility, message)
#define CLOGD(instance, facility, message)          gkr_log_simple_message(instance, LOG_SEVERITY_DEBUG  , facility, message)
#define CLOGT(instance, facility, message)          gkr_log_simple_message(instance, LOG_SEVERITY_TRACE  , facility, message)

#define CLOG_FATAL(  instance, facility, message)   gkr_log_simple_message(instance, LOG_SEVERITY_FATAL  , facility, message)
#define CLOG_ERROR(  instance, facility, message)   gkr_log_simple_message(instance, LOG_SEVERITY_ERROR  , facility, message)
#define CLOG_WARNING(instance, facility, message)   gkr_log_simple_message(instance, LOG_SEVERITY_WARNING, facility, message)
#define CLOG_INFO(   instance, facility, message)   gkr_log_simple_message(instance, LOG_SEVERITY_INFO   , facility, message)
#define CLOG_VERBOSE(instance, facility, message)   gkr_log_simple_message(instance, LOG_SEVERITY_VERBOSE, facility, message)
#define CLOG_DEBUG(  instance, facility, message)   gkr_log_simple_message(instance, LOG_SEVERITY_DEBUG  , facility, message)
#define CLOG_TRACE(  instance, facility, message)   gkr_log_simple_message(instance, LOG_SEVERITY_TRACE  , facility, message)

#define CLOG_IF(condition, instance, severity, facility, message)   ((!(condition)) ? 0 : gkr_log_simple_message(instance, severity            , facility, message))

#define CLOGF_IF(condition, instance, facility, message)            ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_FATAL  , facility, message))
#define CLOGE_IF(condition, instance, facility, message)            ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_ERROR  , facility, message))
#define CLOGW_IF(condition, instance, facility, message)            ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_WARNING, facility, message))
#define CLOGI_IF(condition, instance, facility, message)            ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_INFO   , facility, message))
#define CLOGV_IF(condition, instance, facility, message)            ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_VERBOSE, facility, message))
#define CLOGD_IF(condition, instance, facility, message)            ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_DEBUG  , facility, message))
#define CLOGT_IF(condition, instance, facility, message)            ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_TRACE  , facility, message))

#define CLOG_FATAL_IF(  condition, instance, facility, message)     ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_FATAL  , facility, message))
#define CLOG_ERROR_IF(  condition, instance, facility, message)     ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_ERROR  , facility, message))
#define CLOG_WARNING_IF(condition, instance, facility, message)     ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_WARNING, facility, message))
#define CLOG_INFO_IF(   condition, instance, facility, message)     ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_INFO   , facility, message))
#define CLOG_VERBOSE_IF(condition, instance, facility, message)     ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_VERBOSE, facility, message))
#define CLOG_DEBUG_IF(  condition, instance, facility, message)     ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_DEBUG  , facility, message))
#define CLOG_TRACE_IF(  condition, instance, facility, message)     ((!(condition)) ? 0 : gkr_log_simple_message(instance, LOG_SEVERITY_TRACE  , facility, message))

#ifndef GKR_NO_PRINTF_LOGGING

#define LOG_(severity, facility, format, ...)   gkr_log_printf_message(nullptr, severity            , facility, format, __VA_ARGS__)

#define LOGF_(facility, format, ...)            gkr_log_printf_message(nullptr, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define LOGE_(facility, format, ...)            gkr_log_printf_message(nullptr, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define LOGW_(facility, format, ...)            gkr_log_printf_message(nullptr, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define LOGI_(facility, format, ...)            gkr_log_printf_message(nullptr, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define LOGV_(facility, format, ...)            gkr_log_printf_message(nullptr, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define LOGD_(facility, format, ...)            gkr_log_printf_message(nullptr, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define LOGT_(facility, format, ...)            gkr_log_printf_message(nullptr, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define LOG_FATAL_(  facility, message, ...)    gkr_log_printf_message(nullptr, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define LOG_ERROR_(  facility, message, ...)    gkr_log_printf_message(nullptr, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define LOG_WARNING_(facility, message, ...)    gkr_log_printf_message(nullptr, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define LOG_INFO_(   facility, message, ...)    gkr_log_printf_message(nullptr, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define LOG_VERBOSE_(facility, message, ...)    gkr_log_printf_message(nullptr, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define LOG_DEBUG_(  facility, message, ...)    gkr_log_printf_message(nullptr, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define LOG_TRACE_(  facility, message, ...)    gkr_log_printf_message(nullptr, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define LOG_IF_(condition, severity, facility, format, ...) ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, severity            , facility, format, __VA_ARGS__));

#define LOGF_IF_(condition, facility, format, ...)          ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__))
#define LOGE_IF_(condition, facility, format, ...)          ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__))
#define LOGW_IF_(condition, facility, format, ...)          ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__))
#define LOGI_IF_(condition, facility, format, ...)          ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__))
#define LOGV_IF_(condition, facility, format, ...)          ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__))
#define LOGD_IF_(condition, facility, format, ...)          ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__))
#define LOGT_IF_(condition, facility, format, ...)          ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__))

#define LOG_FATAL_IF_(  condition, facility, format, ...)   ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__))
#define LOG_ERROR_IF_(  condition, facility, format, ...)   ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__))
#define LOG_WARNING_IF_(condition, facility, format, ...)   ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__))
#define LOG_INFO_IF_(   condition, facility, format, ...)   ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__))
#define LOG_VERBOSE_IF_(condition, facility, format, ...)   ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__))
#define LOG_DEBUG_IF_(  condition, facility, format, ...)   ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__))
#define LOG_TRACE_IF_(  condition, facility, format, ...)   ((!(condition)) ? 0 : gkr_log_printf_message(nullptr, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__))

#define CLOG_(instance, severity, facility, format, ...)   gkr_log_printf_message(instance, severity            , facility, format, __VA_ARGS__)

#define CLOGF_(instance, facility, format, ...)            gkr_log_printf_message(instance, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define CLOGE_(instance, facility, format, ...)            gkr_log_printf_message(instance, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define CLOGW_(instance, facility, format, ...)            gkr_log_printf_message(instance, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define CLOGI_(instance, facility, format, ...)            gkr_log_printf_message(instance, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define CLOGV_(instance, facility, format, ...)            gkr_log_printf_message(instance, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define CLOGD_(instance, facility, format, ...)            gkr_log_printf_message(instance, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define CLOGT_(instance, facility, format, ...)            gkr_log_printf_message(instance, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define CLOG_FATAL_(  instance, facility, message, ...)    gkr_log_printf_message(instance, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define CLOG_ERROR_(  instance, facility, message, ...)    gkr_log_printf_message(instance, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define CLOG_WARNING_(instance, facility, message, ...)    gkr_log_printf_message(instance, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define CLOG_INFO_(   instance, facility, message, ...)    gkr_log_printf_message(instance, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define CLOG_VERBOSE_(instance, facility, message, ...)    gkr_log_printf_message(instance, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define CLOG_DEBUG_(  instance, facility, message, ...)    gkr_log_printf_message(instance, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define CLOG_TRACE_(  instance, facility, message, ...)    gkr_log_printf_message(instance, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define CLOG_IF_(condition, instance, severity, facility, format, ...)      ((!(condition)) ? 0 : gkr_log_printf_message(instance, severity            , facility, format, __VA_ARGS__));

#define CLOGF_IF_(condition, instance, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__))
#define CLOGE_IF_(condition, instance, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__))
#define CLOGW_IF_(condition, instance, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__))
#define CLOGI_IF_(condition, instance, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__))
#define CLOGV_IF_(condition, instance, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__))
#define CLOGD_IF_(condition, instance, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__))
#define CLOGT_IF_(condition, instance, facility, format, ...)               ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__))

#define CLOG_FATAL_IF_(  condition, instance, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__))
#define CLOG_ERROR_IF_(  condition, instance, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__))
#define CLOG_WARNING_IF_(condition, instance, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__))
#define CLOG_INFO_IF_(   condition, instance, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__))
#define CLOG_VERBOSE_IF_(condition, instance, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__))
#define CLOG_DEBUG_IF_(  condition, instance, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__))
#define CLOG_TRACE_IF_(  condition, instance, facility, format, ...)        ((!(condition)) ? 0 : gkr_log_printf_message(instance, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__))

#endif /*GKR_NO_PRINTF_LOGGING*/

#endif /*!defined(__cplusplus) || defined(GKR_LOG_USE_C_DEFS)*/
