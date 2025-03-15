#pragma once

#ifndef GKR_LOG_GENERIC_C_DEFS
#define GKR_LOG_GENERIC_C_DEFS

#ifndef __cplusplus
#ifndef LOG_USE_C_DEFS
#define LOG_USE_C_DEFS
#endif
#endif

#ifndef GKR_LOG_GENERIC
#ifndef LOG_USE_C_DEFS
#define LOG_USE_C_DEFS
#endif
#include <gkr/capi/log/defs/generic.h>
#endif

#ifdef LOG_USE_C_DEFS

inline void gkr_log_simple_message_if(int condition, void* instance, int severity, int facility, const char* message)
{
    if(condition != 0) gkr_log_simple_message(instance, severity, facility, message);
}
inline void gkr_log_printf_message_if(int condition, void* instance, int severity, int facility, const char* format, ...)
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
#define GKR_LOG_X_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_X_PRINTF_MSG_IF gkr_log_printf_message_if

#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_FATAL
#define GKR_LOG_F_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_F_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_F_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_F_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_F_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_F_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_F_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_F_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_ERROR
#define GKR_LOG_E_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_E_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_E_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_E_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_E_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_E_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_E_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_E_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_WARNING
#define GKR_LOG_W_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_W_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_W_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_W_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_W_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_W_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_W_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_W_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_INFO
#define GKR_LOG_I_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_I_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_I_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_I_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_I_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_I_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_I_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_I_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_VERBOSE
#define GKR_LOG_V_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_V_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_V_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_V_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_V_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_V_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_V_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_V_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_DEBUG
#define GKR_LOG_D_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_D_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_D_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_D_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_D_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_D_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_D_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_D_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_TRACE
#define GKR_LOG_T_SIMPLE_MSG    (void)gkr_log_simple_message
#define GKR_LOG_T_PRINTF_MSG    (void)gkr_log_printf_message
#define GKR_LOG_T_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_T_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_T_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_T_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_T_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_T_PRINTF_MSG_IF GKR_LOG_NOOP
#endif

#ifdef GKR_LOG_BASIC_KEEP
//
// C Defs Simple basic LOG - bLOGxxx
//
#define bLOG(              severity, message)   GKR_LOG_X_SIMPLE_MSG(              LOG_INSTANCE, severity, LOG_FACILITY, message)
#define bLOG_IF(condition, severity, message)   GKR_LOG_X_SIMPLE_MSG_IF(condition, LOG_INSTANCE, severity, LOG_FACILITY, message)

#define bLOGF(message)                          GKR_LOG_F_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOGE(message)                          GKR_LOG_E_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOGW(message)                          GKR_LOG_W_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOGI(message)                          GKR_LOG_I_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOGV(message)                          GKR_LOG_V_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOGD(message)                          GKR_LOG_D_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOGT(message)                          GKR_LOG_T_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOG_FATAL(  message)                   GKR_LOG_F_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOG_ERROR(  message)                   GKR_LOG_E_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOG_WARNING(message)                   GKR_LOG_W_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOG_INFO(   message)                   GKR_LOG_I_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOG_VERBOSE(message)                   GKR_LOG_V_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOG_DEBUG(  message)                   GKR_LOG_D_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOG_TRACE(  message)                   GKR_LOG_T_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOGF_IF(condition, message)            GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOGE_IF(condition, message)            GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOGW_IF(condition, message)            GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOGI_IF(condition, message)            GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOGV_IF(condition, message)            GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOGD_IF(condition, message)            GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOGT_IF(condition, message)            GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOG_FATAL_IF(  condition, message)     GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOG_ERROR_IF(  condition, message)     GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOG_WARNING_IF(condition, message)     GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOG_INFO_IF(   condition, message)     GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOG_VERBOSE_IF(condition, message)     GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOG_DEBUG_IF(  condition, message)     GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOG_TRACE_IF(  condition, message)     GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C defs Simple facility LOG - fLOGxxx
//
#define fLOG(              severity, facility, message) GKR_LOG_X_SIMPLE_MSG(              LOG_INSTANCE, severity, facility, message)
#define fLOG_IF(condition, severity, facility, message) GKR_LOG_X_SIMPLE_MSG_IF(condition, LOG_INSTANCE, severity, facility, message)

#define fLOGF(facility, message)                        GKR_LOG_F_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , facility, message)
#define fLOGE(facility, message)                        GKR_LOG_E_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , facility, message)
#define fLOGW(facility, message)                        GKR_LOG_W_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, facility, message)
#define fLOGI(facility, message)                        GKR_LOG_I_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , facility, message)
#define fLOGV(facility, message)                        GKR_LOG_V_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOGD(facility, message)                        GKR_LOG_D_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOGT(facility, message)                        GKR_LOG_T_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , facility, message)

#define fLOG_FATAL(  facility, message)                 GKR_LOG_F_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , facility, message)
#define fLOG_ERROR(  facility, message)                 GKR_LOG_E_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , facility, message)
#define fLOG_WARNING(facility, message)                 GKR_LOG_W_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, facility, message)
#define fLOG_INFO(   facility, message)                 GKR_LOG_I_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , facility, message)
#define fLOG_VERBOSE(facility, message)                 GKR_LOG_V_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOG_DEBUG(  facility, message)                 GKR_LOG_D_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOG_TRACE(  facility, message)                 GKR_LOG_T_SIMPLE_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , facility, message)

#define fLOGF_IF(condition, facility, message)          GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , facility, message)
#define fLOGE_IF(condition, facility, message)          GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , facility, message)
#define fLOGW_IF(condition, facility, message)          GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, facility, message)
#define fLOGI_IF(condition, facility, message)          GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , facility, message)
#define fLOGV_IF(condition, facility, message)          GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOGD_IF(condition, facility, message)          GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOGT_IF(condition, facility, message)          GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , facility, message)

#define fLOG_FATAL_IF(  condition, facility, message)   GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , facility, message)
#define fLOG_ERROR_IF(  condition, facility, message)   GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , facility, message)
#define fLOG_WARNING_IF(condition, facility, message)   GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, facility, message)
#define fLOG_INFO_IF(   condition, facility, message)   GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , facility, message)
#define fLOG_VERBOSE_IF(condition, facility, message)   GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOG_DEBUG_IF(  condition, facility, message)   GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOG_TRACE_IF(  condition, facility, message)   GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , facility, message)
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_MODULE_KEEP
//
// C Defs Simple module LOG - mLOGxxx
//
#define mLOG(              instance, severity, message) GKR_LOG_X_SIMPLE_MSG(              instance, severity, LOG_FACILITY, message)
#define mLOG_IF(condition, instance, severity, message) GKR_LOG_X_SIMPLE_MSG_IF(condition, instance, severity, LOG_FACILITY, message)

#define mLOGF(instance, message)                        GKR_LOG_F_SIMPLE_MSG(instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define mLOGE(instance, message)                        GKR_LOG_E_SIMPLE_MSG(instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define mLOGW(instance, message)                        GKR_LOG_W_SIMPLE_MSG(instance, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define mLOGI(instance, message)                        GKR_LOG_I_SIMPLE_MSG(instance, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define mLOGV(instance, message)                        GKR_LOG_V_SIMPLE_MSG(instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define mLOGD(instance, message)                        GKR_LOG_D_SIMPLE_MSG(instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define mLOGT(instance, message)                        GKR_LOG_T_SIMPLE_MSG(instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define mLOG_FATAL(  instance, message)                 GKR_LOG_F_SIMPLE_MSG(instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define mLOG_ERROR(  instance, message)                 GKR_LOG_E_SIMPLE_MSG(instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define mLOG_WARNING(instance, message)                 GKR_LOG_W_SIMPLE_MSG(instance, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define mLOG_INFO(   instance, message)                 GKR_LOG_I_SIMPLE_MSG(instance, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define mLOG_VERBOSE(instance, message)                 GKR_LOG_V_SIMPLE_MSG(instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define mLOG_DEBUG(  instance, message)                 GKR_LOG_D_SIMPLE_MSG(instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define mLOG_TRACE(  instance, message)                 GKR_LOG_T_SIMPLE_MSG(instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define mLOGF_IF(condition, instance, message)          GKR_LOG_F_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define mLOGE_IF(condition, instance, message)          GKR_LOG_E_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define mLOGW_IF(condition, instance, message)          GKR_LOG_W_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define mLOGI_IF(condition, instance, message)          GKR_LOG_I_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define mLOGV_IF(condition, instance, message)          GKR_LOG_V_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define mLOGD_IF(condition, instance, message)          GKR_LOG_D_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define mLOGT_IF(condition, instance, message)          GKR_LOG_T_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define mLOG_FATAL_IF(  condition, instance, message)   GKR_LOG_F_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define mLOG_ERROR_IF(  condition, instance, message)   GKR_LOG_E_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define mLOG_WARNING_IF(condition, instance, message)   GKR_LOG_W_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define mLOG_INFO_IF(   condition, instance, message)   GKR_LOG_I_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define mLOG_VERBOSE_IF(condition, instance, message)   GKR_LOG_V_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define mLOG_DEBUG_IF(  condition, instance, message)   GKR_LOG_D_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define mLOG_TRACE_IF(  condition, instance, message)   GKR_LOG_T_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)
#endif /*def GKR_LOG_MODULE_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C defs Simple general LOG - gLOGxxx
//
#define gLOG(              instance, severity, facility, message) GKR_LOG_X_SIMPLE_MSG(              instance, severity, facility, message)
#define gLOG_IF(condition, instance, severity, facility, message) GKR_LOG_X_SIMPLE_MSG_IF(condition, instance, severity, facility, message)

#define gLOGF(instance, facility, message)                      GKR_LOG_F_SIMPLE_MSG(instance, LOG_SEVERITY_FATAL  , facility, message)
#define gLOGE(instance, facility, message)                      GKR_LOG_E_SIMPLE_MSG(instance, LOG_SEVERITY_ERROR  , facility, message)
#define gLOGW(instance, facility, message)                      GKR_LOG_W_SIMPLE_MSG(instance, LOG_SEVERITY_WARNING, facility, message)
#define gLOGI(instance, facility, message)                      GKR_LOG_I_SIMPLE_MSG(instance, LOG_SEVERITY_INFO   , facility, message)
#define gLOGV(instance, facility, message)                      GKR_LOG_V_SIMPLE_MSG(instance, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOGD(instance, facility, message)                      GKR_LOG_D_SIMPLE_MSG(instance, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOGT(instance, facility, message)                      GKR_LOG_T_SIMPLE_MSG(instance, LOG_SEVERITY_TRACE  , facility, message)

#define gLOG_FATAL(  instance, facility, message)               GKR_LOG_F_SIMPLE_MSG(instance, LOG_SEVERITY_FATAL  , facility, message)
#define gLOG_ERROR(  instance, facility, message)               GKR_LOG_E_SIMPLE_MSG(instance, LOG_SEVERITY_ERROR  , facility, message)
#define gLOG_WARNING(instance, facility, message)               GKR_LOG_W_SIMPLE_MSG(instance, LOG_SEVERITY_WARNING, facility, message)
#define gLOG_INFO(   instance, facility, message)               GKR_LOG_I_SIMPLE_MSG(instance, LOG_SEVERITY_INFO   , facility, message)
#define gLOG_VERBOSE(instance, facility, message)               GKR_LOG_V_SIMPLE_MSG(instance, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOG_DEBUG(  instance, facility, message)               GKR_LOG_D_SIMPLE_MSG(instance, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOG_TRACE(  instance, facility, message)               GKR_LOG_T_SIMPLE_MSG(instance, LOG_SEVERITY_TRACE  , facility, message)

#define gLOGF_IF(condition, instance, facility, message)        GKR_LOG_F_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , facility, message)
#define gLOGE_IF(condition, instance, facility, message)        GKR_LOG_E_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , facility, message)
#define gLOGW_IF(condition, instance, facility, message)        GKR_LOG_W_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, facility, message)
#define gLOGI_IF(condition, instance, facility, message)        GKR_LOG_I_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , facility, message)
#define gLOGV_IF(condition, instance, facility, message)        GKR_LOG_V_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOGD_IF(condition, instance, facility, message)        GKR_LOG_D_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOGT_IF(condition, instance, facility, message)        GKR_LOG_T_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , facility, message)

#define gLOG_FATAL_IF(  condition, instance, facility, message) GKR_LOG_F_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , facility, message)
#define gLOG_ERROR_IF(  condition, instance, facility, message) GKR_LOG_E_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , facility, message)
#define gLOG_WARNING_IF(condition, instance, facility, message) GKR_LOG_W_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, facility, message)
#define gLOG_INFO_IF(   condition, instance, facility, message) GKR_LOG_I_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , facility, message)
#define gLOG_VERBOSE_IF(condition, instance, facility, message) GKR_LOG_V_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOG_DEBUG_IF(  condition, instance, facility, message) GKR_LOG_D_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOG_TRACE_IF(  condition, instance, facility, message) GKR_LOG_T_SIMPLE_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , facility, message)
#endif /*def GKR_LOG_GENERAL_KEEP*/

#ifndef GKR_NO_PRINTF_LOGGING

#ifdef GKR_LOG_BASIC_KEEP
//
// C Defs PrintF basic LOG - bLOGxxx
//
#define bLOG_(              severity, format, ...)  GKR_LOG_X_PRINTF_MSG(              LOG_INSTANCE, severity, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_IF_(condition, severity, format, ...)  GKR_LOG_X_PRINTF_MSG_IF(condition, LOG_INSTANCE, severity, LOG_FACILITY, format, __VA_ARGS__)

#define bLOGF_(format, ...)                         GKR_LOG_F_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGE_(format, ...)                         GKR_LOG_E_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGW_(format, ...)                         GKR_LOG_W_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGI_(format, ...)                         GKR_LOG_I_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGV_(format, ...)                         GKR_LOG_V_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGD_(format, ...)                         GKR_LOG_D_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGT_(format, ...)                         GKR_LOG_T_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOG_FATAL_(  message, ...)                 GKR_LOG_F_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_ERROR_(  message, ...)                 GKR_LOG_E_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_WARNING_(message, ...)                 GKR_LOG_W_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_INFO_(   message, ...)                 GKR_LOG_I_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_VERBOSE_(message, ...)                 GKR_LOG_V_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_DEBUG_(  message, ...)                 GKR_LOG_D_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_TRACE_(  message, ...)                 GKR_LOG_T_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOGF_IF_(condition, format, ...)           GKR_LOG_F_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGE_IF_(condition, format, ...)           GKR_LOG_E_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGW_IF_(condition, format, ...)           GKR_LOG_W_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGI_IF_(condition, format, ...)           GKR_LOG_I_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGV_IF_(condition, format, ...)           GKR_LOG_V_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGD_IF_(condition, format, ...)           GKR_LOG_D_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGT_IF_(condition, format, ...)           GKR_LOG_T_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOG_FATAL_IF_(  condition, format, ...)    GKR_LOG_F_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_ERROR_IF_(  condition, format, ...)    GKR_LOG_E_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_WARNING_IF_(condition, format, ...)    GKR_LOG_W_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_INFO_IF_(   condition, format, ...)    GKR_LOG_I_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_VERBOSE_IF_(condition, format, ...)    GKR_LOG_V_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_DEBUG_IF_(  condition, format, ...)    GKR_LOG_D_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_TRACE_IF_(  condition, format, ...)    GKR_LOG_T_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C defs PrintF facility LOG - fLOGxxx
//
#define fLOG_(              severity, facility, format, ...)    GKR_LOG_X_PRINTF_MSG(              LOG_INSTANCE, severity, facility, format, __VA_ARGS__)
#define fLOG_IF_(condition, severity, facility, format, ...)    GKR_LOG_X_PRINTF_MSG_IF(condition, LOG_INSTANCE, severity, facility, format, __VA_ARGS__)

#define fLOGF_(facility, format, ...)                           GKR_LOG_F_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOGE_(facility, format, ...)                           GKR_LOG_E_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOGW_(facility, format, ...)                           GKR_LOG_W_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOGI_(facility, format, ...)                           GKR_LOG_I_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOGV_(facility, format, ...)                           GKR_LOG_V_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOGD_(facility, format, ...)                           GKR_LOG_D_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOGT_(facility, format, ...)                           GKR_LOG_T_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOG_FATAL_(  facility, message, ...)                   GKR_LOG_F_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOG_ERROR_(  facility, message, ...)                   GKR_LOG_E_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOG_WARNING_(facility, message, ...)                   GKR_LOG_W_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOG_INFO_(   facility, message, ...)                   GKR_LOG_I_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOG_VERBOSE_(facility, message, ...)                   GKR_LOG_V_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOG_DEBUG_(  facility, message, ...)                   GKR_LOG_D_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOG_TRACE_(  facility, message, ...)                   GKR_LOG_T_PRINTF_MSG(LOG_INSTANCE, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOGF_IF_(condition, facility, format, ...)             GKR_LOG_F_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOGE_IF_(condition, facility, format, ...)             GKR_LOG_E_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOGW_IF_(condition, facility, format, ...)             GKR_LOG_W_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOGI_IF_(condition, facility, format, ...)             GKR_LOG_I_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOGV_IF_(condition, facility, format, ...)             GKR_LOG_V_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOGD_IF_(condition, facility, format, ...)             GKR_LOG_D_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOGT_IF_(condition, facility, format, ...)             GKR_LOG_T_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOG_FATAL_IF_(  condition, facility, format, ...)      GKR_LOG_F_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOG_ERROR_IF_(  condition, facility, format, ...)      GKR_LOG_E_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOG_WARNING_IF_(condition, facility, format, ...)      GKR_LOG_W_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOG_INFO_IF_(   condition, facility, format, ...)      GKR_LOG_I_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOG_VERBOSE_IF_(condition, facility, format, ...)      GKR_LOG_V_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOG_DEBUG_IF_(  condition, facility, format, ...)      GKR_LOG_D_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOG_TRACE_IF_(  condition, facility, format, ...)      GKR_LOG_T_PRINTF_MSG_IF(condition, LOG_INSTANCE, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_MODULE_KEEP
//
// C defs PrintF module LOG - mLOGxxx
//
#define mLOG_(              instance, severity, format, ...)    GKR_LOG_X_PRINTF_MSG(              instance, severity, LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_IF_(condition, instance, severity, format, ...)    GKR_LOG_X_PRINTF_MSG_IF(condition, instance, severity, LOG_FACILITY, format, __VA_ARGS__)

#define mLOGF_(instance, format, ...)                           GKR_LOG_F_PRINTF_MSG(instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOGE_(instance, format, ...)                           GKR_LOG_E_PRINTF_MSG(instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOGW_(instance, format, ...)                           GKR_LOG_W_PRINTF_MSG(instance, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define mLOGI_(instance, format, ...)                           GKR_LOG_I_PRINTF_MSG(instance, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define mLOGV_(instance, format, ...)                           GKR_LOG_V_PRINTF_MSG(instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define mLOGD_(instance, format, ...)                           GKR_LOG_D_PRINTF_MSG(instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOGT_(instance, format, ...)                           GKR_LOG_T_PRINTF_MSG(instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define mLOG_FATAL_(  instance, message, ...)                   GKR_LOG_F_PRINTF_MSG(instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_ERROR_(  instance, message, ...)                   GKR_LOG_E_PRINTF_MSG(instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_WARNING_(instance, message, ...)                   GKR_LOG_W_PRINTF_MSG(instance, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_INFO_(   instance, message, ...)                   GKR_LOG_I_PRINTF_MSG(instance, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_VERBOSE_(instance, message, ...)                   GKR_LOG_V_PRINTF_MSG(instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_DEBUG_(  instance, message, ...)                   GKR_LOG_D_PRINTF_MSG(instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_TRACE_(  instance, message, ...)                   GKR_LOG_T_PRINTF_MSG(instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define mLOGF_IF_(condition, instance, format, ...)             GKR_LOG_F_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOGE_IF_(condition, instance, format, ...)             GKR_LOG_E_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOGW_IF_(condition, instance, format, ...)             GKR_LOG_W_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define mLOGI_IF_(condition, instance, format, ...)             GKR_LOG_I_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define mLOGV_IF_(condition, instance, format, ...)             GKR_LOG_V_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define mLOGD_IF_(condition, instance, format, ...)             GKR_LOG_D_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOGT_IF_(condition, instance, format, ...)             GKR_LOG_T_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define mLOG_FATAL_IF_(  condition, instance, format, ...)      GKR_LOG_F_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_ERROR_IF_(  condition, instance, format, ...)      GKR_LOG_E_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_WARNING_IF_(condition, instance, format, ...)      GKR_LOG_W_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_INFO_IF_(   condition, instance, format, ...)      GKR_LOG_I_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_VERBOSE_IF_(condition, instance, format, ...)      GKR_LOG_V_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_DEBUG_IF_(  condition, instance, format, ...)      GKR_LOG_D_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define mLOG_TRACE_IF_(  condition, instance, format, ...)      GKR_LOG_T_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)
#endif /*def GKR_LOG_MODULE_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C defs PrintF general LOG - gLOGxxx
//
#define gLOG_(              instance, severity, facility, format, ...)  GKR_LOG_X_PRINTF_MSG(              instance, severity, facility, format, __VA_ARGS__)
#define gLOG_IF_(condition, instance, severity, facility, format, ...)  GKR_LOG_X_PRINTF_MSG_IF(condition, instance, severity, facility, format, __VA_ARGS__)

#define gLOGF_(instance, facility, format, ...)                         GKR_LOG_F_PRINTF_MSG(instance, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOGE_(instance, facility, format, ...)                         GKR_LOG_E_PRINTF_MSG(instance, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOGW_(instance, facility, format, ...)                         GKR_LOG_W_PRINTF_MSG(instance, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOGI_(instance, facility, format, ...)                         GKR_LOG_I_PRINTF_MSG(instance, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOGV_(instance, facility, format, ...)                         GKR_LOG_V_PRINTF_MSG(instance, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOGD_(instance, facility, format, ...)                         GKR_LOG_D_PRINTF_MSG(instance, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOGT_(instance, facility, format, ...)                         GKR_LOG_T_PRINTF_MSG(instance, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOG_FATAL_(  instance, facility, message, ...)                 GKR_LOG_F_PRINTF_MSG(instance, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOG_ERROR_(  instance, facility, message, ...)                 GKR_LOG_E_PRINTF_MSG(instance, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOG_WARNING_(instance, facility, message, ...)                 GKR_LOG_W_PRINTF_MSG(instance, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOG_INFO_(   instance, facility, message, ...)                 GKR_LOG_I_PRINTF_MSG(instance, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOG_VERBOSE_(instance, facility, message, ...)                 GKR_LOG_V_PRINTF_MSG(instance, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOG_DEBUG_(  instance, facility, message, ...)                 GKR_LOG_D_PRINTF_MSG(instance, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOG_TRACE_(  instance, facility, message, ...)                 GKR_LOG_T_PRINTF_MSG(instance, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOGF_IF_(condition, instance, facility, format, ...)           GKR_LOG_F_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOGE_IF_(condition, instance, facility, format, ...)           GKR_LOG_E_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOGW_IF_(condition, instance, facility, format, ...)           GKR_LOG_W_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOGI_IF_(condition, instance, facility, format, ...)           GKR_LOG_I_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOGV_IF_(condition, instance, facility, format, ...)           GKR_LOG_V_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOGD_IF_(condition, instance, facility, format, ...)           GKR_LOG_D_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOGT_IF_(condition, instance, facility, format, ...)           GKR_LOG_T_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOG_FATAL_IF_(  condition, instance, facility, format, ...)    GKR_LOG_F_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOG_ERROR_IF_(  condition, instance, facility, format, ...)    GKR_LOG_E_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOG_WARNING_IF_(condition, instance, facility, format, ...)    GKR_LOG_W_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOG_INFO_IF_(   condition, instance, facility, format, ...)    GKR_LOG_I_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOG_VERBOSE_IF_(condition, instance, facility, format, ...)    GKR_LOG_V_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOG_DEBUG_IF_(  condition, instance, facility, format, ...)    GKR_LOG_D_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOG_TRACE_IF_(  condition, instance, facility, format, ...)    GKR_LOG_T_PRINTF_MSG_IF(condition, instance, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)
#endif /*def GKR_LOG_GENERAL_KEEP*/

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#endif /*def LOG_USE_C_DEFS*/
#endif /*ndef GKR_LOG_GENERIC_C_DEFS*/
