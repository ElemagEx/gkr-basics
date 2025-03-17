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

#ifndef LOG_SOURCE_LOCATION
#define LOG_MORE_ARGS_
#define LOG_MORE_ARGS
#define LOG_MORE_VALS
#else
#define LOG_MORE_ARGS_ , const char*, const char*, unsigned
#define LOG_MORE_ARGS  , const char* func, const char* file, unsigned line
#ifdef _MSC_VER
#define LOG_MORE_VALS  , __FUNCSIG__, __FILE__, __LINE__
#else
#define LOG_MORE_VALS  , __PRETTY_FUNCTION__, __FILE__, __LINE__
#endif
#endif

inline void gkr_log_simple_message_uc(void* channel LOG_MORE_ARGS, int severity, int facility, const char* message)
{
#ifndef LOG_SOURCE_LOCATION
    gkr_log_simple_message(channel, severity, facility, message);
#else
    gkr_log_simple_message_ex(channel, func, file, line, severity, facility, message);
#endif
}
inline void gkr_log_printf_message_uc(void* channel LOG_MORE_ARGS, int severity, int facility, const char* format, ...)
{
    va_list args;
    va_start(args, format);
#ifndef LOG_SOURCE_LOCATION
    gkr_log_valist_message(channel, severity, facility, format, args);
#else
    gkr_log_valist_message_ex(channel, func, file, line, severity, facility, message);
#endif
    va_end(args);
}

inline void gkr_log_simple_message_if(int condition LOG_MORE_ARGS, void* channel, int severity, int facility, const char* message LOG_MORE_ARGS)
{
    if(condition != 0)
    {
#ifndef LOG_SOURCE_LOCATION
        gkr_log_simple_message(channel, severity, facility, message);
#else
        gkr_log_simple_message_ex(channel, func, file, line, severity, facility, message);
#endif
    }
}
inline void gkr_log_printf_message_if(int condition LOG_MORE_ARGS, void* channel, int severity, int facility, const char* format, ...)
{
    if(condition != 0)
    {
        va_list args;
        va_start(args, format);
#ifndef LOG_SOURCE_LOCATION
        gkr_log_valist_message(channel, severity, facility, format, args);
#else
        gkr_log_valist_message_ex(channel, func, file, line, severity, facility, message);
#endif
        va_end(args);
    }
}

#define GKR_LOG_NOOP(...)      ((void)0)

#define GKR_LOG_X_SIMPLE_MSG    gkr_log_simple_message_uc
#define GKR_LOG_X_PRINTF_MSG    gkr_log_printf_message_uc
#define GKR_LOG_X_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_X_PRINTF_MSG_IF gkr_log_printf_message_if

#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_FATAL
#define GKR_LOG_F_SIMPLE_MSG    gkr_log_simple_message_uc
#define GKR_LOG_F_PRINTF_MSG    gkr_log_printf_message_uc
#define GKR_LOG_F_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_F_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_F_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_F_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_F_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_F_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_ERROR
#define GKR_LOG_E_SIMPLE_MSG    gkr_log_simple_message_uc
#define GKR_LOG_E_PRINTF_MSG    gkr_log_printf_message_uc
#define GKR_LOG_E_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_E_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_E_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_E_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_E_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_E_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_WARNING
#define GKR_LOG_W_SIMPLE_MSG    gkr_log_simple_message_uc
#define GKR_LOG_W_PRINTF_MSG    gkr_log_printf_message_uc
#define GKR_LOG_W_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_W_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_W_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_W_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_W_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_W_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_INFO
#define GKR_LOG_I_SIMPLE_MSG    gkr_log_simple_message_uc
#define GKR_LOG_I_PRINTF_MSG    gkr_log_printf_message_uc
#define GKR_LOG_I_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_I_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_I_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_I_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_I_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_I_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_VERBOSE
#define GKR_LOG_V_SIMPLE_MSG    gkr_log_simple_message_uc
#define GKR_LOG_V_PRINTF_MSG    gkr_log_printf_message_uc
#define GKR_LOG_V_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_V_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_V_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_V_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_V_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_V_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_DEBUG
#define GKR_LOG_D_SIMPLE_MSG    gkr_log_simple_message_uc
#define GKR_LOG_D_PRINTF_MSG    gkr_log_printf_message_uc
#define GKR_LOG_D_SIMPLE_MSG_IF gkr_log_simple_message_if
#define GKR_LOG_D_PRINTF_MSG_IF gkr_log_printf_message_if
#else
#define GKR_LOG_D_SIMPLE_MSG    GKR_LOG_NOOP
#define GKR_LOG_D_PRINTF_MSG    GKR_LOG_NOOP
#define GKR_LOG_D_SIMPLE_MSG_IF GKR_LOG_NOOP
#define GKR_LOG_D_PRINTF_MSG_IF GKR_LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_TRACE
#define GKR_LOG_T_SIMPLE_MSG    gkr_log_simple_message_uc
#define GKR_LOG_T_PRINTF_MSG    gkr_log_printf_message_uc
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
#define bLOG(severity, message)               GKR_LOG_X_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, severity, LOG_FACILITY, message)

#define bLOGF(message)                        GKR_LOG_F_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOGE(message)                        GKR_LOG_E_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOGW(message)                        GKR_LOG_W_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOGI(message)                        GKR_LOG_I_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOGV(message)                        GKR_LOG_V_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOGD(message)                        GKR_LOG_D_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOGT(message)                        GKR_LOG_T_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOG_FATAL(  message)                 GKR_LOG_F_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOG_ERROR(  message)                 GKR_LOG_E_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOG_WARNING(message)                 GKR_LOG_W_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOG_INFO(   message)                 GKR_LOG_I_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOG_VERBOSE(message)                 GKR_LOG_V_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOG_DEBUG(  message)                 GKR_LOG_D_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOG_TRACE(  message)                 GKR_LOG_T_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOG_IF(condition, severity, message) GKR_LOG_X_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, severity, LOG_FACILITY, message)

#define bLOGF_IF(condition, message)          GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOGE_IF(condition, message)          GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOGW_IF(condition, message)          GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOGI_IF(condition, message)          GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOGV_IF(condition, message)          GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOGD_IF(condition, message)          GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOGT_IF(condition, message)          GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOG_FATAL_IF(  condition, message)   GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOG_ERROR_IF(  condition, message)   GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOG_WARNING_IF(condition, message)   GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOG_INFO_IF(   condition, message)   GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOG_VERBOSE_IF(condition, message)   GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOG_DEBUG_IF(  condition, message)   GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOG_TRACE_IF(  condition, message)   GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C defs Simple facility LOG - fLOGxxx
//
#define fLOG(severity, facility, message)               GKR_LOG_X_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, severity, facility, message)

#define fLOGF(facility, message)                        GKR_LOG_F_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define fLOGE(facility, message)                        GKR_LOG_E_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define fLOGW(facility, message)                        GKR_LOG_W_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define fLOGI(facility, message)                        GKR_LOG_I_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define fLOGV(facility, message)                        GKR_LOG_V_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOGD(facility, message)                        GKR_LOG_D_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOGT(facility, message)                        GKR_LOG_T_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define fLOG_FATAL(  facility, message)                 GKR_LOG_F_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define fLOG_ERROR(  facility, message)                 GKR_LOG_E_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define fLOG_WARNING(facility, message)                 GKR_LOG_W_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define fLOG_INFO(   facility, message)                 GKR_LOG_I_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define fLOG_VERBOSE(facility, message)                 GKR_LOG_V_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOG_DEBUG(  facility, message)                 GKR_LOG_D_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOG_TRACE(  facility, message)                 GKR_LOG_T_SIMPLE_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define fLOG_IF(condition, severity, facility, message) GKR_LOG_X_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, severity, facility, message)

#define fLOGF_IF(condition, facility, message)          GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define fLOGE_IF(condition, facility, message)          GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define fLOGW_IF(condition, facility, message)          GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define fLOGI_IF(condition, facility, message)          GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define fLOGV_IF(condition, facility, message)          GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOGD_IF(condition, facility, message)          GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOGT_IF(condition, facility, message)          GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define fLOG_FATAL_IF(  condition, facility, message)   GKR_LOG_F_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define fLOG_ERROR_IF(  condition, facility, message)   GKR_LOG_E_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define fLOG_WARNING_IF(condition, facility, message)   GKR_LOG_W_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define fLOG_INFO_IF(   condition, facility, message)   GKR_LOG_I_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define fLOG_VERBOSE_IF(condition, facility, message)   GKR_LOG_V_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOG_DEBUG_IF(  condition, facility, message)   GKR_LOG_D_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOG_TRACE_IF(  condition, facility, message)   GKR_LOG_T_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_CHANNEL_KEEP
//
// C Defs Simple channel LOG - cLOGxxx
//
#define cLOG(channel, severity, message)               GKR_LOG_X_SIMPLE_MSG(channel LOG_MORE_VALS, severity, LOG_FACILITY, message)

#define cLOGF(channel, message)                        GKR_LOG_F_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define cLOGE(channel, message)                        GKR_LOG_E_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define cLOGW(channel, message)                        GKR_LOG_W_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define cLOGI(channel, message)                        GKR_LOG_I_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define cLOGV(channel, message)                        GKR_LOG_V_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define cLOGD(channel, message)                        GKR_LOG_D_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define cLOGT(channel, message)                        GKR_LOG_T_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define cLOG_FATAL(  channel, message)                 GKR_LOG_F_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define cLOG_ERROR(  channel, message)                 GKR_LOG_E_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define cLOG_WARNING(channel, message)                 GKR_LOG_W_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define cLOG_INFO(   channel, message)                 GKR_LOG_I_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define cLOG_VERBOSE(channel, message)                 GKR_LOG_V_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define cLOG_DEBUG(  channel, message)                 GKR_LOG_D_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define cLOG_TRACE(  channel, message)                 GKR_LOG_T_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define cLOG_IF(condition, channel, severity, message) GKR_LOG_X_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, severity, LOG_FACILITY, message)

#define cLOGF_IF(condition, channel, message)          GKR_LOG_F_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define cLOGE_IF(condition, channel, message)          GKR_LOG_E_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define cLOGW_IF(condition, channel, message)          GKR_LOG_W_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define cLOGI_IF(condition, channel, message)          GKR_LOG_I_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define cLOGV_IF(condition, channel, message)          GKR_LOG_V_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define cLOGD_IF(condition, channel, message)          GKR_LOG_D_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define cLOGT_IF(condition, channel, message)          GKR_LOG_T_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define cLOG_FATAL_IF(  condition, channel, message)   GKR_LOG_F_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define cLOG_ERROR_IF(  condition, channel, message)   GKR_LOG_E_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define cLOG_WARNING_IF(condition, channel, message)   GKR_LOG_W_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define cLOG_INFO_IF(   condition, channel, message)   GKR_LOG_I_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define cLOG_VERBOSE_IF(condition, channel, message)   GKR_LOG_V_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define cLOG_DEBUG_IF(  condition, channel, message)   GKR_LOG_D_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define cLOG_TRACE_IF(  condition, channel, message)   GKR_LOG_T_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)
#endif /*def GKR_LOG_CHANNEL_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C defs Simple general LOG - gLOGxxx
//
#define gLOG(channel, severity, facility, message)               GKR_LOG_X_SIMPLE_MSG(channel LOG_MORE_VALS, severity, facility, message)

#define gLOGF(channel, facility, message)                        GKR_LOG_F_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define gLOGE(channel, facility, message)                        GKR_LOG_E_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define gLOGW(channel, facility, message)                        GKR_LOG_W_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define gLOGI(channel, facility, message)                        GKR_LOG_I_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define gLOGV(channel, facility, message)                        GKR_LOG_V_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOGD(channel, facility, message)                        GKR_LOG_D_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOGT(channel, facility, message)                        GKR_LOG_T_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define gLOG_FATAL(  channel, facility, message)                 GKR_LOG_F_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define gLOG_ERROR(  channel, facility, message)                 GKR_LOG_E_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define gLOG_WARNING(channel, facility, message)                 GKR_LOG_W_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define gLOG_INFO(   channel, facility, message)                 GKR_LOG_I_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define gLOG_VERBOSE(channel, facility, message)                 GKR_LOG_V_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOG_DEBUG(  channel, facility, message)                 GKR_LOG_D_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOG_TRACE(  channel, facility, message)                 GKR_LOG_T_SIMPLE_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define gLOG_IF(condition, channel, severity, facility, message) GKR_LOG_X_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, severity, facility, message)

#define gLOGF_IF(condition, channel, facility, message)          GKR_LOG_F_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define gLOGE_IF(condition, channel, facility, message)          GKR_LOG_E_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define gLOGW_IF(condition, channel, facility, message)          GKR_LOG_W_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define gLOGI_IF(condition, channel, facility, message)          GKR_LOG_I_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define gLOGV_IF(condition, channel, facility, message)          GKR_LOG_V_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOGD_IF(condition, channel, facility, message)          GKR_LOG_D_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOGT_IF(condition, channel, facility, message)          GKR_LOG_T_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define gLOG_FATAL_IF(  condition, channel, facility, message)   GKR_LOG_F_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define gLOG_ERROR_IF(  condition, channel, facility, message)   GKR_LOG_E_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define gLOG_WARNING_IF(condition, channel, facility, message)   GKR_LOG_W_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define gLOG_INFO_IF(   condition, channel, facility, message)   GKR_LOG_I_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define gLOG_VERBOSE_IF(condition, channel, facility, message)   GKR_LOG_V_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOG_DEBUG_IF(  condition, channel, facility, message)   GKR_LOG_D_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOG_TRACE_IF(  condition, channel, facility, message)   GKR_LOG_T_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)
#endif /*def GKR_LOG_GENERAL_KEEP*/

#ifndef GKR_NO_PRINTF_LOGGING

#ifdef GKR_LOG_BASIC_KEEP
//
// C Defs PrintF basic LOG - bLOGxxx
//
#define bLOG_(severity, format, ...)               GKR_LOG_X_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, severity, LOG_FACILITY, format, __VA_ARGS__)

#define bLOGF_(format, ...)                        GKR_LOG_F_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGE_(format, ...)                        GKR_LOG_E_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGW_(format, ...)                        GKR_LOG_W_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGI_(format, ...)                        GKR_LOG_I_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGV_(format, ...)                        GKR_LOG_V_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGD_(format, ...)                        GKR_LOG_D_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGT_(format, ...)                        GKR_LOG_T_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOG_FATAL_(  message, ...)                GKR_LOG_F_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_ERROR_(  message, ...)                GKR_LOG_E_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_WARNING_(message, ...)                GKR_LOG_W_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_INFO_(   message, ...)                GKR_LOG_I_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_VERBOSE_(message, ...)                GKR_LOG_V_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_DEBUG_(  message, ...)                GKR_LOG_D_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_TRACE_(  message, ...)                GKR_LOG_T_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOG_IF_(condition, severity, format, ...) GKR_LOG_X_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, severity, LOG_FACILITY, format, __VA_ARGS__)

#define bLOGF_IF_(condition, format, ...)          GKR_LOG_F_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGE_IF_(condition, format, ...)          GKR_LOG_E_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGW_IF_(condition, format, ...)          GKR_LOG_W_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGI_IF_(condition, format, ...)          GKR_LOG_I_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGV_IF_(condition, format, ...)          GKR_LOG_V_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGD_IF_(condition, format, ...)          GKR_LOG_D_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGT_IF_(condition, format, ...)          GKR_LOG_T_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOG_FATAL_IF_(  condition, format, ...)   GKR_LOG_F_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_ERROR_IF_(  condition, format, ...)   GKR_LOG_E_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_WARNING_IF_(condition, format, ...)   GKR_LOG_W_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_INFO_IF_(   condition, format, ...)   GKR_LOG_I_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_VERBOSE_IF_(condition, format, ...)   GKR_LOG_V_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_DEBUG_IF_(  condition, format, ...)   GKR_LOG_D_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_TRACE_IF_(  condition, format, ...)   GKR_LOG_T_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)
#endif /*def GKR_LOG_BASIC_KEEP*/

#ifdef GKR_LOG_FACILITY_KEEP
//
// C defs PrintF facility LOG - fLOGxxx
//
#define fLOG_(severity, facility, format, ...)               GKR_LOG_X_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, severity, facility, format, __VA_ARGS__)

#define fLOGF_(facility, format, ...)                        GKR_LOG_F_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOGE_(facility, format, ...)                        GKR_LOG_E_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOGW_(facility, format, ...)                        GKR_LOG_W_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOGI_(facility, format, ...)                        GKR_LOG_I_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOGV_(facility, format, ...)                        GKR_LOG_V_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOGD_(facility, format, ...)                        GKR_LOG_D_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOGT_(facility, format, ...)                        GKR_LOG_T_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOG_FATAL_(  facility, message, ...)                GKR_LOG_F_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOG_ERROR_(  facility, message, ...)                GKR_LOG_E_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOG_WARNING_(facility, message, ...)                GKR_LOG_W_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOG_INFO_(   facility, message, ...)                GKR_LOG_I_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOG_VERBOSE_(facility, message, ...)                GKR_LOG_V_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOG_DEBUG_(  facility, message, ...)                GKR_LOG_D_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOG_TRACE_(  facility, message, ...)                GKR_LOG_T_PRINTF_MSG(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOG_IF_(condition, severity, facility, format, ...) GKR_LOG_X_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, severity, facility, format, __VA_ARGS__)

#define fLOGF_IF_(condition, facility, format, ...)          GKR_LOG_F_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOGE_IF_(condition, facility, format, ...)          GKR_LOG_E_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOGW_IF_(condition, facility, format, ...)          GKR_LOG_W_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOGI_IF_(condition, facility, format, ...)          GKR_LOG_I_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOGV_IF_(condition, facility, format, ...)          GKR_LOG_V_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOGD_IF_(condition, facility, format, ...)          GKR_LOG_D_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOGT_IF_(condition, facility, format, ...)          GKR_LOG_T_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOG_FATAL_IF_(  condition, facility, format, ...)   GKR_LOG_F_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOG_ERROR_IF_(  condition, facility, format, ...)   GKR_LOG_E_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOG_WARNING_IF_(condition, facility, format, ...)   GKR_LOG_W_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOG_INFO_IF_(   condition, facility, format, ...)   GKR_LOG_I_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOG_VERBOSE_IF_(condition, facility, format, ...)   GKR_LOG_V_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOG_DEBUG_IF_(  condition, facility, format, ...)   GKR_LOG_D_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOG_TRACE_IF_(  condition, facility, format, ...)   GKR_LOG_T_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)
#endif /*def GKR_LOG_FACILITY_KEEP*/

#ifdef GKR_LOG_CHANNEL_KEEP
//
// C defs PrintF channel LOG - cLOGxxx
//
#define cLOG_(channel, severity, format, ...)               GKR_LOG_X_PRINTF_MSG(channel LOG_MORE_VALS, severity, LOG_FACILITY, format, __VA_ARGS__)

#define cLOGF_(channel, format, ...)                        GKR_LOG_F_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGE_(channel, format, ...)                        GKR_LOG_E_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGW_(channel, format, ...)                        GKR_LOG_W_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define cLOGI_(channel, format, ...)                        GKR_LOG_I_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGV_(channel, format, ...)                        GKR_LOG_V_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define cLOGD_(channel, format, ...)                        GKR_LOG_D_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGT_(channel, format, ...)                        GKR_LOG_T_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define cLOG_FATAL_(  channel, message, ...)                GKR_LOG_F_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_ERROR_(  channel, message, ...)                GKR_LOG_E_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_WARNING_(channel, message, ...)                GKR_LOG_W_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_INFO_(   channel, message, ...)                GKR_LOG_I_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_VERBOSE_(channel, message, ...)                GKR_LOG_V_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_DEBUG_(  channel, message, ...)                GKR_LOG_D_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_TRACE_(  channel, message, ...)                GKR_LOG_T_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define cLOG_IF_(condition, channel, severity, format, ...) GKR_LOG_X_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, severity, LOG_FACILITY, format, __VA_ARGS__)

#define cLOGF_IF_(condition, channel, format, ...)          GKR_LOG_F_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGE_IF_(condition, channel, format, ...)          GKR_LOG_E_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGW_IF_(condition, channel, format, ...)          GKR_LOG_W_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define cLOGI_IF_(condition, channel, format, ...)          GKR_LOG_I_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGV_IF_(condition, channel, format, ...)          GKR_LOG_V_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define cLOGD_IF_(condition, channel, format, ...)          GKR_LOG_D_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGT_IF_(condition, channel, format, ...)          GKR_LOG_T_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define cLOG_FATAL_IF_(  condition, channel, format, ...)   GKR_LOG_F_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_ERROR_IF_(  condition, channel, format, ...)   GKR_LOG_E_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_WARNING_IF_(condition, channel, format, ...)   GKR_LOG_W_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_INFO_IF_(   condition, channel, format, ...)   GKR_LOG_I_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_VERBOSE_IF_(condition, channel, format, ...)   GKR_LOG_V_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_DEBUG_IF_(  condition, channel, format, ...)   GKR_LOG_D_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_TRACE_IF_(  condition, channel, format, ...)   GKR_LOG_T_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)
#endif /*def GKR_LOG_CHANNEL_KEEP*/

#ifdef GKR_LOG_GENERAL_KEEP
//
// C defs PrintF general LOG - gLOGxxx
//
#define gLOG_(channel, severity, facility, format, ...)               GKR_LOG_X_PRINTF_MSG(channel LOG_MORE_VALS, severity, facility, format, __VA_ARGS__)

#define gLOGF_(channel, facility, format, ...)                        GKR_LOG_F_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOGE_(channel, facility, format, ...)                        GKR_LOG_E_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOGW_(channel, facility, format, ...)                        GKR_LOG_W_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOGI_(channel, facility, format, ...)                        GKR_LOG_I_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOGV_(channel, facility, format, ...)                        GKR_LOG_V_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOGD_(channel, facility, format, ...)                        GKR_LOG_D_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOGT_(channel, facility, format, ...)                        GKR_LOG_T_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOG_FATAL_(  channel, facility, message, ...)                GKR_LOG_F_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOG_ERROR_(  channel, facility, message, ...)                GKR_LOG_E_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOG_WARNING_(channel, facility, message, ...)                GKR_LOG_W_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOG_INFO_(   channel, facility, message, ...)                GKR_LOG_I_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOG_VERBOSE_(channel, facility, message, ...)                GKR_LOG_V_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOG_DEBUG_(  channel, facility, message, ...)                GKR_LOG_D_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOG_TRACE_(  channel, facility, message, ...)                GKR_LOG_T_PRINTF_MSG(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOG_IF_(condition, channel, severity, facility, format, ...) GKR_LOG_X_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, severity, facility, format, __VA_ARGS__)

#define gLOGF_IF_(condition, channel, facility, format, ...)          GKR_LOG_F_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOGE_IF_(condition, channel, facility, format, ...)          GKR_LOG_E_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOGW_IF_(condition, channel, facility, format, ...)          GKR_LOG_W_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOGI_IF_(condition, channel, facility, format, ...)          GKR_LOG_I_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOGV_IF_(condition, channel, facility, format, ...)          GKR_LOG_V_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOGD_IF_(condition, channel, facility, format, ...)          GKR_LOG_D_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOGT_IF_(condition, channel, facility, format, ...)          GKR_LOG_T_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOG_FATAL_IF_(  condition, channel, facility, format, ...)   GKR_LOG_F_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOG_ERROR_IF_(  condition, channel, facility, format, ...)   GKR_LOG_E_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOG_WARNING_IF_(condition, channel, facility, format, ...)   GKR_LOG_W_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOG_INFO_IF_(   condition, channel, facility, format, ...)   GKR_LOG_I_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOG_VERBOSE_IF_(condition, channel, facility, format, ...)   GKR_LOG_V_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOG_DEBUG_IF_(  condition, channel, facility, format, ...)   GKR_LOG_D_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOG_TRACE_IF_(  condition, channel, facility, format, ...)   GKR_LOG_T_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)
#endif /*def GKR_LOG_GENERAL_KEEP*/

#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#endif /*def LOG_USE_C_DEFS*/
#endif /*ndef GKR_LOG_GENERIC_C_DEFS*/
