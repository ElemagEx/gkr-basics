#pragma once

#ifndef GENERIC_CDEFS_H
#define GENERIC_CDEFS_H

#ifndef __cplusplus
#ifndef GENERIC_LOG_USE_C_DEFS
#define GENERIC_LOG_USE_C_DEFS
#endif
#endif

#ifndef GENERIC_H
#ifndef GENERIC_LOG_USE_C_DEFS
#define GENERIC_LOG_USE_C_DEFS
#endif
#include <gkr/capi/log/defs/generic.h>
#endif

#ifdef GENERIC_LOG_USE_C_DEFS

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

inline void _log_simple_msg_uc(void* channel LOG_MORE_ARGS, int severity, int facility, const char* message)
{
#ifndef LOG_SOURCE_LOCATION
    gkr_log_simple_message(channel, severity, facility, message);
#else
    gkr_log_simple_message_ex(channel, func, file, line, severity, facility, message);
#endif
}
inline void _log_printf_msg_uc(void* channel LOG_MORE_ARGS, int severity, int facility, const char* format, ...)
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

inline void _log_simple_msg_if(int condition LOG_MORE_ARGS, void* channel, int severity, int facility, const char* message LOG_MORE_ARGS)
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
inline void _log_printf_msg_if(int condition LOG_MORE_ARGS, void* channel, int severity, int facility, const char* format, ...)
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

#define _LOG_NOOP(...)       ((void)0)

#define _LOG_X_SIMPLE_MSG_UC _log_simple_msg_uc
#define _LOG_X_PRINTF_MSG_UC _log_printf_msg_uc
#define _LOG_X_SIMPLE_MSG_IF _log_simple_msg_if
#define _LOG_X_PRINTF_MSG_IF _log_printf_msg_if

#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_FATAL
#define _LOG_F_SIMPLE_MSG_UC _log_simple_msg_uc
#define _LOG_F_PRINTF_MSG_UC _log_printf_msg_uc
#define _LOG_F_SIMPLE_MSG_IF _log_simple_msg_if
#define _LOG_F_PRINTF_MSG_IF _log_printf_msg_if
#else
#define _LOG_F_SIMPLE_MSG_UC _LOG_NOOP
#define _LOG_F_PRINTF_MSG_UC _LOG_NOOP
#define _LOG_F_SIMPLE_MSG_IF _LOG_NOOP
#define _LOG_F_PRINTF_MSG_IF _LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_ERROR
#define _LOG_E_SIMPLE_MSG_UC _log_simple_msg_uc
#define _LOG_E_PRINTF_MSG_UC _log_printf_msg_uc
#define _LOG_E_SIMPLE_MSG_IF _log_simple_msg_if
#define _LOG_E_PRINTF_MSG_IF _log_printf_msg_if
#else
#define _LOG_E_SIMPLE_MSG_UC _LOG_NOOP
#define _LOG_E_PRINTF_MSG_UC _LOG_NOOP
#define _LOG_E_SIMPLE_MSG_IF _LOG_NOOP
#define _LOG_E_PRINTF_MSG_IF _LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_WARNING
#define _LOG_W_SIMPLE_MSG_UC _log_simple_msg_uc
#define _LOG_W_PRINTF_MSG_UC _log_printf_msg_uc
#define _LOG_W_SIMPLE_MSG_IF _log_simple_msg_if
#define _LOG_W_PRINTF_MSG_IF _log_printf_msg_if
#else
#define _LOG_W_SIMPLE_MSG_UC _LOG_NOOP
#define _LOG_W_PRINTF_MSG_UC _LOG_NOOP
#define _LOG_W_SIMPLE_MSG_IF _LOG_NOOP
#define _LOG_W_PRINTF_MSG_IF _LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_INFO
#define _LOG_I_SIMPLE_MSG_UC _log_simple_msg_uc
#define _LOG_I_PRINTF_MSG_UC _log_printf_msg_uc
#define _LOG_I_SIMPLE_MSG_IF _log_simple_msg_if
#define _LOG_I_PRINTF_MSG_IF _log_printf_msg_if
#else
#define _LOG_I_SIMPLE_MSG_UC _LOG_NOOP
#define _LOG_I_PRINTF_MSG_UC _LOG_NOOP
#define _LOG_I_SIMPLE_MSG_IF _LOG_NOOP
#define _LOG_I_PRINTF_MSG_IF _LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_VERBOSE
#define _LOG_V_SIMPLE_MSG_UC _log_simple_msg_uc
#define _LOG_V_PRINTF_MSG_UC _log_printf_msg_uc
#define _LOG_V_SIMPLE_MSG_IF _log_simple_msg_if
#define _LOG_V_PRINTF_MSG_IF _log_printf_msg_if
#else
#define _LOG_V_SIMPLE_MSG_UC _LOG_NOOP
#define _LOG_V_PRINTF_MSG_UC _LOG_NOOP
#define _LOG_V_SIMPLE_MSG_IF _LOG_NOOP
#define _LOG_V_PRINTF_MSG_IF _LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_DEBUG
#define _LOG_D_SIMPLE_MSG_UC _log_simple_msg_uc
#define _LOG_D_PRINTF_MSG_UC _log_printf_msg_uc
#define _LOG_D_SIMPLE_MSG_IF _log_simple_msg_if
#define _LOG_D_PRINTF_MSG_IF _log_printf_msg_if
#else
#define _LOG_D_SIMPLE_MSG_UC _LOG_NOOP
#define _LOG_D_PRINTF_MSG_UC _LOG_NOOP
#define _LOG_D_SIMPLE_MSG_IF _LOG_NOOP
#define _LOG_D_PRINTF_MSG_IF _LOG_NOOP
#endif
#if LOG_THRESHOLD_LEVEL > LOG_SEVERITY_TRACE
#define _LOG_T_SIMPLE_MSG_UC _log_simple_msg_uc
#define _LOG_T_PRINTF_MSG_UC _log_printf_msg_uc
#define _LOG_T_SIMPLE_MSG_IF _log_simple_msg_if
#define _LOG_T_PRINTF_MSG_IF _log_printf_msg_if
#else
#define _LOG_T_SIMPLE_MSG_UC _LOG_NOOP
#define _LOG_T_PRINTF_MSG_UC _LOG_NOOP
#define _LOG_T_SIMPLE_MSG_IF _LOG_NOOP
#define _LOG_T_PRINTF_MSG_IF _LOG_NOOP
#endif

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C Defs Simple basic LOG - bLOGxxx
//
#define bLOG(severity, message)               _LOG_X_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, severity, LOG_FACILITY, message)

#define bLOGF(message)                        _LOG_F_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOGE(message)                        _LOG_E_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOGW(message)                        _LOG_W_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOGI(message)                        _LOG_I_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOGV(message)                        _LOG_V_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOGD(message)                        _LOG_D_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOGT(message)                        _LOG_T_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOG_FATAL(  message)                 _LOG_F_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOG_ERROR(  message)                 _LOG_E_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOG_WARNING(message)                 _LOG_W_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOG_INFO(   message)                 _LOG_I_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOG_VERBOSE(message)                 _LOG_V_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOG_DEBUG(  message)                 _LOG_D_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOG_TRACE(  message)                 _LOG_T_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOG_IF(condition, severity, message) _LOG_X_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, severity, LOG_FACILITY, message)

#define bLOGF_IF(condition, message)          _LOG_F_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOGE_IF(condition, message)          _LOG_E_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOGW_IF(condition, message)          _LOG_W_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOGI_IF(condition, message)          _LOG_I_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOGV_IF(condition, message)          _LOG_V_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOGD_IF(condition, message)          _LOG_D_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOGT_IF(condition, message)          _LOG_T_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define bLOG_FATAL_IF(  condition, message)   _LOG_F_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define bLOG_ERROR_IF(  condition, message)   _LOG_E_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define bLOG_WARNING_IF(condition, message)   _LOG_W_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define bLOG_INFO_IF(   condition, message)   _LOG_I_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define bLOG_VERBOSE_IF(condition, message)   _LOG_V_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define bLOG_DEBUG_IF(  condition, message)   _LOG_D_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define bLOG_TRACE_IF(  condition, message)   _LOG_T_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C defs Simple facility LOG - fLOGxxx
//
#define fLOG(severity, facility, message)               _LOG_X_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, severity, facility, message)

#define fLOGF(facility, message)                        _LOG_F_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define fLOGE(facility, message)                        _LOG_E_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define fLOGW(facility, message)                        _LOG_W_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define fLOGI(facility, message)                        _LOG_I_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define fLOGV(facility, message)                        _LOG_V_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOGD(facility, message)                        _LOG_D_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOGT(facility, message)                        _LOG_T_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define fLOG_FATAL(  facility, message)                 _LOG_F_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define fLOG_ERROR(  facility, message)                 _LOG_E_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define fLOG_WARNING(facility, message)                 _LOG_W_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define fLOG_INFO(   facility, message)                 _LOG_I_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define fLOG_VERBOSE(facility, message)                 _LOG_V_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOG_DEBUG(  facility, message)                 _LOG_D_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOG_TRACE(  facility, message)                 _LOG_T_SIMPLE_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define fLOG_IF(condition, severity, facility, message) _LOG_X_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, severity, facility, message)

#define fLOGF_IF(condition, facility, message)          _LOG_F_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define fLOGE_IF(condition, facility, message)          _LOG_E_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define fLOGW_IF(condition, facility, message)          _LOG_W_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define fLOGI_IF(condition, facility, message)          _LOG_I_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define fLOGV_IF(condition, facility, message)          _LOG_V_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOGD_IF(condition, facility, message)          _LOG_D_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOGT_IF(condition, facility, message)          _LOG_T_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define fLOG_FATAL_IF(  condition, facility, message)   _LOG_F_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define fLOG_ERROR_IF(  condition, facility, message)   _LOG_E_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define fLOG_WARNING_IF(condition, facility, message)   _LOG_W_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define fLOG_INFO_IF(   condition, facility, message)   _LOG_I_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define fLOG_VERBOSE_IF(condition, facility, message)   _LOG_V_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define fLOG_DEBUG_IF(  condition, facility, message)   _LOG_D_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define fLOG_TRACE_IF(  condition, facility, message)   _LOG_T_SIMPLE_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C Defs Simple channel LOG - cLOGxxx
//
#define cLOG(channel, severity, message)               _LOG_X_SIMPLE_MSG_UC(channel LOG_MORE_VALS, severity, LOG_FACILITY, message)

#define cLOGF(channel, message)                        _LOG_F_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define cLOGE(channel, message)                        _LOG_E_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define cLOGW(channel, message)                        _LOG_W_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define cLOGI(channel, message)                        _LOG_I_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define cLOGV(channel, message)                        _LOG_V_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define cLOGD(channel, message)                        _LOG_D_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define cLOGT(channel, message)                        _LOG_T_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define cLOG_FATAL(  channel, message)                 _LOG_F_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define cLOG_ERROR(  channel, message)                 _LOG_E_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define cLOG_WARNING(channel, message)                 _LOG_W_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define cLOG_INFO(   channel, message)                 _LOG_I_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define cLOG_VERBOSE(channel, message)                 _LOG_V_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define cLOG_DEBUG(  channel, message)                 _LOG_D_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define cLOG_TRACE(  channel, message)                 _LOG_T_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define cLOG_IF(condition, channel, severity, message) _LOG_X_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, severity, LOG_FACILITY, message)

#define cLOGF_IF(condition, channel, message)          _LOG_F_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define cLOGE_IF(condition, channel, message)          _LOG_E_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define cLOGW_IF(condition, channel, message)          _LOG_W_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define cLOGI_IF(condition, channel, message)          _LOG_I_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define cLOGV_IF(condition, channel, message)          _LOG_V_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define cLOGD_IF(condition, channel, message)          _LOG_D_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define cLOGT_IF(condition, channel, message)          _LOG_T_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)

#define cLOG_FATAL_IF(  condition, channel, message)   _LOG_F_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, message)
#define cLOG_ERROR_IF(  condition, channel, message)   _LOG_E_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, message)
#define cLOG_WARNING_IF(condition, channel, message)   _LOG_W_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, message)
#define cLOG_INFO_IF(   condition, channel, message)   _LOG_I_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, message)
#define cLOG_VERBOSE_IF(condition, channel, message)   _LOG_V_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, message)
#define cLOG_DEBUG_IF(  condition, channel, message)   _LOG_D_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, message)
#define cLOG_TRACE_IF(  condition, channel, message)   _LOG_T_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, message)
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C defs Simple general LOG - gLOGxxx
//
#define gLOG(channel, severity, facility, message)               _LOG_X_SIMPLE_MSG_UC(channel LOG_MORE_VALS, severity, facility, message)

#define gLOGF(channel, facility, message)                        _LOG_F_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define gLOGE(channel, facility, message)                        _LOG_E_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define gLOGW(channel, facility, message)                        _LOG_W_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define gLOGI(channel, facility, message)                        _LOG_I_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define gLOGV(channel, facility, message)                        _LOG_V_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOGD(channel, facility, message)                        _LOG_D_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOGT(channel, facility, message)                        _LOG_T_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define gLOG_FATAL(  channel, facility, message)                 _LOG_F_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define gLOG_ERROR(  channel, facility, message)                 _LOG_E_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define gLOG_WARNING(channel, facility, message)                 _LOG_W_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define gLOG_INFO(   channel, facility, message)                 _LOG_I_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define gLOG_VERBOSE(channel, facility, message)                 _LOG_V_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOG_DEBUG(  channel, facility, message)                 _LOG_D_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOG_TRACE(  channel, facility, message)                 _LOG_T_SIMPLE_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define gLOG_IF(condition, channel, severity, facility, message) _LOG_X_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, severity, facility, message)

#define gLOGF_IF(condition, channel, facility, message)          _LOG_F_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define gLOGE_IF(condition, channel, facility, message)          _LOG_E_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define gLOGW_IF(condition, channel, facility, message)          _LOG_W_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define gLOGI_IF(condition, channel, facility, message)          _LOG_I_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define gLOGV_IF(condition, channel, facility, message)          _LOG_V_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOGD_IF(condition, channel, facility, message)          _LOG_D_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOGT_IF(condition, channel, facility, message)          _LOG_T_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)

#define gLOG_FATAL_IF(  condition, channel, facility, message)   _LOG_F_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, message)
#define gLOG_ERROR_IF(  condition, channel, facility, message)   _LOG_E_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, message)
#define gLOG_WARNING_IF(condition, channel, facility, message)   _LOG_W_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, message)
#define gLOG_INFO_IF(   condition, channel, facility, message)   _LOG_I_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, message)
#define gLOG_VERBOSE_IF(condition, channel, facility, message)   _LOG_V_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, message)
#define gLOG_DEBUG_IF(  condition, channel, facility, message)   _LOG_D_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, message)
#define gLOG_TRACE_IF(  condition, channel, facility, message)   _LOG_T_SIMPLE_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, message)
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#ifndef GENERIC_LOG_SKIP_PRINTF

#ifdef GENERIC_LOG_BASIC_KEEP
//
// C Defs PrintF basic LOG - bLOGxxx
//
#define bLOG_(severity, format, ...)               _LOG_X_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, severity, LOG_FACILITY, format, __VA_ARGS__)

#define bLOGF_(format, ...)                        _LOG_F_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGE_(format, ...)                        _LOG_E_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGW_(format, ...)                        _LOG_W_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGI_(format, ...)                        _LOG_I_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGV_(format, ...)                        _LOG_V_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGD_(format, ...)                        _LOG_D_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGT_(format, ...)                        _LOG_T_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOG_FATAL_(  message, ...)                _LOG_F_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_ERROR_(  message, ...)                _LOG_E_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_WARNING_(message, ...)                _LOG_W_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_INFO_(   message, ...)                _LOG_I_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_VERBOSE_(message, ...)                _LOG_V_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_DEBUG_(  message, ...)                _LOG_D_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_TRACE_(  message, ...)                _LOG_T_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOG_IF_(condition, severity, format, ...) _LOG_X_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, severity, LOG_FACILITY, format, __VA_ARGS__)

#define bLOGF_IF_(condition, format, ...)          _LOG_F_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGE_IF_(condition, format, ...)          _LOG_E_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGW_IF_(condition, format, ...)          _LOG_W_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGI_IF_(condition, format, ...)          _LOG_I_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGV_IF_(condition, format, ...)          _LOG_V_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOGD_IF_(condition, format, ...)          _LOG_D_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOGT_IF_(condition, format, ...)          _LOG_T_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define bLOG_FATAL_IF_(  condition, format, ...)   _LOG_F_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_ERROR_IF_(  condition, format, ...)   _LOG_E_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_WARNING_IF_(condition, format, ...)   _LOG_W_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_INFO_IF_(   condition, format, ...)   _LOG_I_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_VERBOSE_IF_(condition, format, ...)   _LOG_V_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_DEBUG_IF_(  condition, format, ...)   _LOG_D_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define bLOG_TRACE_IF_(  condition, format, ...)   _LOG_T_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)
#endif /*def GENERIC_LOG_BASIC_KEEP*/

#ifdef GENERIC_LOG_FACILITY_KEEP
//
// C defs PrintF facility LOG - fLOGxxx
//
#define fLOG_(severity, facility, format, ...)               _LOG_X_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, severity, facility, format, __VA_ARGS__)

#define fLOGF_(facility, format, ...)                        _LOG_F_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOGE_(facility, format, ...)                        _LOG_E_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOGW_(facility, format, ...)                        _LOG_W_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOGI_(facility, format, ...)                        _LOG_I_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOGV_(facility, format, ...)                        _LOG_V_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOGD_(facility, format, ...)                        _LOG_D_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOGT_(facility, format, ...)                        _LOG_T_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOG_FATAL_(  facility, message, ...)                _LOG_F_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOG_ERROR_(  facility, message, ...)                _LOG_E_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOG_WARNING_(facility, message, ...)                _LOG_W_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOG_INFO_(   facility, message, ...)                _LOG_I_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOG_VERBOSE_(facility, message, ...)                _LOG_V_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOG_DEBUG_(  facility, message, ...)                _LOG_D_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOG_TRACE_(  facility, message, ...)                _LOG_T_PRINTF_MSG_UC(LOG_CHANNEL LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOG_IF_(condition, severity, facility, format, ...) _LOG_X_PRINTF_MSG_IF(condition, LOG_CHANNEL LOG_MORE_VALS, severity, facility, format, __VA_ARGS__)

#define fLOGF_IF_(condition, facility, format, ...)          _LOG_F_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOGE_IF_(condition, facility, format, ...)          _LOG_E_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOGW_IF_(condition, facility, format, ...)          _LOG_W_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOGI_IF_(condition, facility, format, ...)          _LOG_I_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOGV_IF_(condition, facility, format, ...)          _LOG_V_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOGD_IF_(condition, facility, format, ...)          _LOG_D_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOGT_IF_(condition, facility, format, ...)          _LOG_T_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define fLOG_FATAL_IF_(  condition, facility, format, ...)   _LOG_F_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define fLOG_ERROR_IF_(  condition, facility, format, ...)   _LOG_E_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define fLOG_WARNING_IF_(condition, facility, format, ...)   _LOG_W_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define fLOG_INFO_IF_(   condition, facility, format, ...)   _LOG_I_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define fLOG_VERBOSE_IF_(condition, facility, format, ...)   _LOG_V_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define fLOG_DEBUG_IF_(  condition, facility, format, ...)   _LOG_D_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define fLOG_TRACE_IF_(  condition, facility, format, ...)   _LOG_T_PRINTF_MSG_IF(condition LOG_MORE_VALS, LOG_CHANNEL, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)
#endif /*def GENERIC_LOG_FACILITY_KEEP*/

#ifdef GENERIC_LOG_CHANNEL_KEEP
//
// C defs PrintF channel LOG - cLOGxxx
//
#define cLOG_(channel, severity, format, ...)               _LOG_X_PRINTF_MSG_UC(channel LOG_MORE_VALS, severity, LOG_FACILITY, format, __VA_ARGS__)

#define cLOGF_(channel, format, ...)                        _LOG_F_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGE_(channel, format, ...)                        _LOG_E_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGW_(channel, format, ...)                        _LOG_W_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define cLOGI_(channel, format, ...)                        _LOG_I_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGV_(channel, format, ...)                        _LOG_V_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define cLOGD_(channel, format, ...)                        _LOG_D_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGT_(channel, format, ...)                        _LOG_T_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define cLOG_FATAL_(  channel, message, ...)                _LOG_F_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_ERROR_(  channel, message, ...)                _LOG_E_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_WARNING_(channel, message, ...)                _LOG_W_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_INFO_(   channel, message, ...)                _LOG_I_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_VERBOSE_(channel, message, ...)                _LOG_V_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_DEBUG_(  channel, message, ...)                _LOG_D_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_TRACE_(  channel, message, ...)                _LOG_T_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define cLOG_IF_(condition, channel, severity, format, ...) _LOG_X_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, severity, LOG_FACILITY, format, __VA_ARGS__)

#define cLOGF_IF_(condition, channel, format, ...)          _LOG_F_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGE_IF_(condition, channel, format, ...)          _LOG_E_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGW_IF_(condition, channel, format, ...)          _LOG_W_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define cLOGI_IF_(condition, channel, format, ...)          _LOG_I_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGV_IF_(condition, channel, format, ...)          _LOG_V_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define cLOGD_IF_(condition, channel, format, ...)          _LOG_D_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOGT_IF_(condition, channel, format, ...)          _LOG_T_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)

#define cLOG_FATAL_IF_(  condition, channel, format, ...)   _LOG_F_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_FATAL  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_ERROR_IF_(  condition, channel, format, ...)   _LOG_E_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_ERROR  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_WARNING_IF_(condition, channel, format, ...)   _LOG_W_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_WARNING, LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_INFO_IF_(   condition, channel, format, ...)   _LOG_I_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_INFO   , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_VERBOSE_IF_(condition, channel, format, ...)   _LOG_V_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_VERBOSE, LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_DEBUG_IF_(  condition, channel, format, ...)   _LOG_D_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_DEBUG  , LOG_FACILITY, format, __VA_ARGS__)
#define cLOG_TRACE_IF_(  condition, channel, format, ...)   _LOG_T_PRINTF_MSG_IF(condition LOG_MORE_VALS, channel, LOG_SEVERITY_TRACE  , LOG_FACILITY, format, __VA_ARGS__)
#endif /*def GENERIC_LOG_CHANNEL_KEEP*/

#ifdef GENERIC_LOG_GENERAL_KEEP
//
// C defs PrintF general LOG - gLOGxxx
//
#define gLOG_(channel, severity, facility, format, ...)               _LOG_X_PRINTF_MSG_UC(channel LOG_MORE_VALS, severity, facility, format, __VA_ARGS__)

#define gLOGF_(channel, facility, format, ...)                        _LOG_F_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOGE_(channel, facility, format, ...)                        _LOG_E_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOGW_(channel, facility, format, ...)                        _LOG_W_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOGI_(channel, facility, format, ...)                        _LOG_I_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOGV_(channel, facility, format, ...)                        _LOG_V_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOGD_(channel, facility, format, ...)                        _LOG_D_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOGT_(channel, facility, format, ...)                        _LOG_T_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOG_FATAL_(  channel, facility, message, ...)                _LOG_F_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOG_ERROR_(  channel, facility, message, ...)                _LOG_E_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOG_WARNING_(channel, facility, message, ...)                _LOG_W_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOG_INFO_(   channel, facility, message, ...)                _LOG_I_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOG_VERBOSE_(channel, facility, message, ...)                _LOG_V_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOG_DEBUG_(  channel, facility, message, ...)                _LOG_D_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOG_TRACE_(  channel, facility, message, ...)                _LOG_T_PRINTF_MSG_UC(channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOG_IF_(condition, channel, severity, facility, format, ...) _LOG_X_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, severity, facility, format, __VA_ARGS__)

#define gLOGF_IF_(condition, channel, facility, format, ...)          _LOG_F_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOGE_IF_(condition, channel, facility, format, ...)          _LOG_E_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOGW_IF_(condition, channel, facility, format, ...)          _LOG_W_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOGI_IF_(condition, channel, facility, format, ...)          _LOG_I_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOGV_IF_(condition, channel, facility, format, ...)          _LOG_V_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOGD_IF_(condition, channel, facility, format, ...)          _LOG_D_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOGT_IF_(condition, channel, facility, format, ...)          _LOG_T_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)

#define gLOG_FATAL_IF_(  condition, channel, facility, format, ...)   _LOG_F_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_FATAL  , facility, format, __VA_ARGS__)
#define gLOG_ERROR_IF_(  condition, channel, facility, format, ...)   _LOG_E_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_ERROR  , facility, format, __VA_ARGS__)
#define gLOG_WARNING_IF_(condition, channel, facility, format, ...)   _LOG_W_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_WARNING, facility, format, __VA_ARGS__)
#define gLOG_INFO_IF_(   condition, channel, facility, format, ...)   _LOG_I_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_INFO   , facility, format, __VA_ARGS__)
#define gLOG_VERBOSE_IF_(condition, channel, facility, format, ...)   _LOG_V_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_VERBOSE, facility, format, __VA_ARGS__)
#define gLOG_DEBUG_IF_(  condition, channel, facility, format, ...)   _LOG_D_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_DEBUG  , facility, format, __VA_ARGS__)
#define gLOG_TRACE_IF_(  condition, channel, facility, format, ...)   _LOG_T_PRINTF_MSG_IF(condition, channel LOG_MORE_VALS, LOG_SEVERITY_TRACE  , facility, format, __VA_ARGS__)
#endif /*def GENERIC_LOG_GENERAL_KEEP*/

#endif /*ndef GENERIC_LOG_SKIP_PRINTF*/

#endif /*def GENERIC_LOG_USE_C_DEFS*/
#endif /*ndef GENERIC_CDEFS_H*/
