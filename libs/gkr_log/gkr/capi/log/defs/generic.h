#pragma once

#ifndef GKR_LOG_GENERIC
#define GKR_LOG_GENERIC
#endif

#include <gkr/capi/log/log.h>

#define LOG_SEVERITY_FATAL      0
#define LOG_SEVERITY_ERROR      1
#define LOG_SEVERITY_WARNING    2
#define LOG_SEVERITY_INFO       3
#define LOG_SEVERITY_VERBOSE    4
#define LOG_SEVERITY_DEBUG      5
#define LOG_SEVERITY_TRACE      6

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

#ifndef LOG_THRESHOLD_LEVEL
#define LOG_THRESHOLD_LEVEL 100
#endif
#ifndef LOG_FACILITY
#define LOG_FACILITY 0
#endif
#ifndef LOG_INSTANCE
#ifdef __cplusplus
#define LOG_INSTANCE nullptr
#else
#define LOG_INSTANCE ((void*)0)
#endif
#endif

#if !defined(__cplusplus) || defined(LOG_USE_C_DEFS)

inline void gkr_log_cond_simple_message(int condition, void* instance, int severity, int facility, const char* message)
{
    if(condition != 0) gkr_log_simple_message(instance, severity, facility, message);
}
inline void gkr_log_cond_printf_message(int condition, void* instance, int severity, int facility, const char* format, ...)
{
    if(condition != 0)
    {
        va_list args;
        va_start(args, format);
        gkr_log_valist_message(instance, severity, facility, format, args);
        va_end(args);
    }
}

#define GKR_LOG_NOOP(...)      ((void)0)

#define GKR_LOG_X_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_X_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_X_SIMPLE_MSG_IF gkr_log_cond_simple_message
#define GKR_LOG_X_PRINTF_MSG_IF gkr_log_cond_printf_message

#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_FATAL
#define GKR_LOG_F_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_F_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_F_SIMPLE_MSG_IF gkr_log_cond_simple_message
#define GKR_LOG_F_PRINTF_MSG_IF gkr_log_cond_printf_message
#else
#define GKR_LOG_F_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_F_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_F_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_F_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_ERROR
#define GKR_LOG_E_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_E_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_E_SIMPLE_MSG_IF gkr_log_cond_simple_message
#define GKR_LOG_E_PRINTF_MSG_IF gkr_log_cond_printf_message
#else
#define GKR_LOG_E_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_E_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_E_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_E_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_WARNING
#define GKR_LOG_W_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_W_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_W_SIMPLE_MSG_IF gkr_log_cond_simple_message
#define GKR_LOG_W_PRINTF_MSG_IF gkr_log_cond_printf_message
#else
#define GKR_LOG_W_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_W_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_W_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_W_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_INFO
#define GKR_LOG_I_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_I_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_I_SIMPLE_MSG_IF gkr_log_cond_simple_message
#define GKR_LOG_I_PRINTF_MSG_IF gkr_log_cond_printf_message
#else
#define GKR_LOG_I_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_I_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_I_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_I_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_VERBOSE
#define GKR_LOG_V_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_V_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_V_SIMPLE_MSG_IF gkr_log_cond_simple_message
#define GKR_LOG_V_PRINTF_MSG_IF gkr_log_cond_printf_message
#else
#define GKR_LOG_V_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_V_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_V_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_V_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_DEBUG
#define GKR_LOG_D_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_D_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_D_SIMPLE_MSG_IF gkr_log_cond_simple_message
#define GKR_LOG_D_PRINTF_MSG_IF gkr_log_cond_printf_message
#else
#define GKR_LOG_D_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_D_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_D_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_D_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_TRACE
#define GKR_LOG_T_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_T_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_T_SIMPLE_MSG_IF gkr_log_cond_simple_message
#define GKR_LOG_T_PRINTF_MSG_IF gkr_log_cond_printf_message
#else
#define GKR_LOG_T_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_T_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_T_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_T_PRINTF_MSG_IF GKR_LOG_NOOP
#endif

#ifndef GKR_LOG_GENERIC_C_DEFS
#include <gkr/capi/log/defs/generic_basic.h>
#endif

#endif /*!defined(__cplusplus) || defined(LOG_USE_C_DEFS)*/
