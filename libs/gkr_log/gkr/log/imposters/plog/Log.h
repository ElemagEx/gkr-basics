#pragma once

#include <plog/Logger.h>

#ifdef PLOG_CAPTURE_FILE
#define PLOG_FILE __FILE__
#else
#define PLOG_FILE nullptr
#endif

#ifdef PLOG_DISABLE_LOGGING
#define PLOG_FUNC nullptr
#define IF_PLOG_(instanceId, severity) if (true) {;} else
#else
#define PLOG_FUNC __FUNCTION__
#define IF_PLOG_(instanceId, severity) if (plog::skipSeverity<instanceId>(severity)) {;} else
#endif

#define IF_PLOG(severity) IF_PLOG_(PLOG_DEFAULT_INSTANCE_ID, severity)

#define PLOG_(instanceId, severity)      IF_PLOG_(instanceId, severity) plog::Record(severity, PLOG_FUNC, __LINE__, PLOG_FILE, nullptr, instanceId)
#define PLOG(severity)                   PLOG_(PLOG_DEFAULT_INSTANCE_ID, severity)

#define PLOG_VERBOSE                     PLOG(plog::verbose)
#define PLOG_DEBUG                       PLOG(plog::debug)
#define PLOG_INFO                        PLOG(plog::info)
#define PLOG_WARNING                     PLOG(plog::warning)
#define PLOG_ERROR                       PLOG(plog::error)
#define PLOG_FATAL                       PLOG(plog::fatal)
#define PLOG_NONE                        PLOG(plog::none)

#define PLOG_VERBOSE_(instanceId)        PLOG_(instanceId, plog::verbose)
#define PLOG_DEBUG_(instanceId)          PLOG_(instanceId, plog::debug)
#define PLOG_INFO_(instanceId)           PLOG_(instanceId, plog::info)
#define PLOG_WARNING_(instanceId)        PLOG_(instanceId, plog::warning)
#define PLOG_ERROR_(instanceId)          PLOG_(instanceId, plog::error)
#define PLOG_FATAL_(instanceId)          PLOG_(instanceId, plog::fatal)
#define PLOG_NONE_(instanceId)           PLOG_(instanceId, plog::none)

#define PLOGV                            PLOG_VERBOSE
#define PLOGD                            PLOG_DEBUG
#define PLOGI                            PLOG_INFO
#define PLOGW                            PLOG_WARNING
#define PLOGE                            PLOG_ERROR
#define PLOGF                            PLOG_FATAL
#define PLOGN                            PLOG_NONE

#define PLOGV_(instanceId)               PLOG_VERBOSE_(instanceId)
#define PLOGD_(instanceId)               PLOG_DEBUG_(instanceId)
#define PLOGI_(instanceId)               PLOG_INFO_(instanceId)
#define PLOGW_(instanceId)               PLOG_WARNING_(instanceId)
#define PLOGE_(instanceId)               PLOG_ERROR_(instanceId)
#define PLOGF_(instanceId)               PLOG_FATAL_(instanceId)
#define PLOGN_(instanceId)               PLOG_NONE_(instanceId)

#define PLOG_IF_(instanceId, severity, condition)  if (!(condition)) {;} else PLOG_(instanceId, severity)
#define PLOG_IF(severity, condition)               PLOG_IF_(PLOG_DEFAULT_INSTANCE_ID, severity, condition)

#define PLOG_VERBOSE_IF(condition)               PLOG_IF(plog::verbose, condition)
#define PLOG_DEBUG_IF(condition)                 PLOG_IF(plog::debug, condition)
#define PLOG_INFO_IF(condition)                  PLOG_IF(plog::info, condition)
#define PLOG_WARNING_IF(condition)               PLOG_IF(plog::warning, condition)
#define PLOG_ERROR_IF(condition)                 PLOG_IF(plog::error, condition)
#define PLOG_FATAL_IF(condition)                 PLOG_IF(plog::fatal, condition)
#define PLOG_NONE_IF(condition)                  PLOG_IF(plog::none, condition)

#define PLOG_VERBOSE_IF_(instanceId, condition)  PLOG_IF_(instanceId, plog::verbose, condition)
#define PLOG_DEBUG_IF_(instanceId, condition)    PLOG_IF_(instanceId, plog::debug, condition)
#define PLOG_INFO_IF_(instanceId, condition)     PLOG_IF_(instanceId, plog::info, condition)
#define PLOG_WARNING_IF_(instanceId, condition)  PLOG_IF_(instanceId, plog::warning, condition)
#define PLOG_ERROR_IF_(instanceId, condition)    PLOG_IF_(instanceId, plog::error, condition)
#define PLOG_FATAL_IF_(instanceId, condition)    PLOG_IF_(instanceId, plog::fatal, condition)
#define PLOG_NONE_IF_(instanceId, condition)     PLOG_IF_(instanceId, plog::none, condition)

#define PLOGV_IF(condition)                      PLOG_VERBOSE_IF(condition)
#define PLOGD_IF(condition)                      PLOG_DEBUG_IF(condition)
#define PLOGI_IF(condition)                      PLOG_INFO_IF(condition)
#define PLOGW_IF(condition)                      PLOG_WARNING_IF(condition)
#define PLOGE_IF(condition)                      PLOG_ERROR_IF(condition)
#define PLOGF_IF(condition)                      PLOG_FATAL_IF(condition)
#define PLOGN_IF(condition)                      PLOG_NONE_IF(condition)

#define PLOGV_IF_(instanceId, condition)         PLOG_VERBOSE_IF_(instanceId, condition)
#define PLOGD_IF_(instanceId, condition)         PLOG_DEBUG_IF_(instanceId, condition)
#define PLOGI_IF_(instanceId, condition)         PLOG_INFO_IF_(instanceId, condition)
#define PLOGW_IF_(instanceId, condition)         PLOG_WARNING_IF_(instanceId, condition)
#define PLOGE_IF_(instanceId, condition)         PLOG_ERROR_IF_(instanceId, condition)
#define PLOGF_IF_(instanceId, condition)         PLOG_FATAL_IF_(instanceId, condition)
#define PLOGN_IF_(instanceId, condition)         PLOG_NONE_IF_(instanceId, condition)

#ifndef PLOG_OMIT_LOG_DEFINES

#define LOG_(instanceId, severity)      IF_PLOG_(instanceId, severity) (*plog::get<instanceId>()) += plog::Record(severity, PLOG_GET_FUNC(), __LINE__, PLOG_GET_FILE(), PLOG_GET_THIS(), instanceId).ref()
#define LOG(severity)                   PLOG_(PLOG_DEFAULT_INSTANCE_ID, severity)

#define LOG_VERBOSE                     PLOG(plog::verbose)
#define LOG_DEBUG                       PLOG(plog::debug)
#define LOG_INFO                        PLOG(plog::info)
#define LOG_WARNING                     PLOG(plog::warning)
#define LOG_ERROR                       PLOG(plog::error)
#define LOG_FATAL                       PLOG(plog::fatal)
#define LOG_NONE                        PLOG(plog::none)

#define LOG_VERBOSE_(instanceId)        PLOG_(instanceId, plog::verbose)
#define LOG_DEBUG_(instanceId)          PLOG_(instanceId, plog::debug)
#define LOG_INFO_(instanceId)           PLOG_(instanceId, plog::info)
#define LOG_WARNING_(instanceId)        PLOG_(instanceId, plog::warning)
#define LOG_ERROR_(instanceId)          PLOG_(instanceId, plog::error)
#define LOG_FATAL_(instanceId)          PLOG_(instanceId, plog::fatal)
#define LOG_NONE_(instanceId)           PLOG_(instanceId, plog::none)

#define LOGV                            PLOG_VERBOSE
#define LOGD                            PLOG_DEBUG
#define LOGI                            PLOG_INFO
#define LOGW                            PLOG_WARNING
#define LOGE                            PLOG_ERROR
#define LOGF                            PLOG_FATAL
#define LOGN                            PLOG_NONE

#define LOGV_(instanceId)               PLOG_VERBOSE_(instanceId)
#define LOGD_(instanceId)               PLOG_DEBUG_(instanceId)
#define LOGI_(instanceId)               PLOG_INFO_(instanceId)
#define LOGW_(instanceId)               PLOG_WARNING_(instanceId)
#define LOGE_(instanceId)               PLOG_ERROR_(instanceId)
#define LOGF_(instanceId)               PLOG_FATAL_(instanceId)
#define LOGN_(instanceId)               PLOG_NONE_(instanceId)

#define LOG_IF_(instanceId, severity, condition)  if (!(condition)) {;} else PLOG_(instanceId, severity)
#define LOG_IF(severity, condition)               PLOG_IF_(PLOG_DEFAULT_INSTANCE_ID, severity, condition)

#define LOG_VERBOSE_IF(condition)               PLOG_IF(plog::verbose, condition)
#define LOG_DEBUG_IF(condition)                 PLOG_IF(plog::debug, condition)
#define LOG_INFO_IF(condition)                  PLOG_IF(plog::info, condition)
#define LOG_WARNING_IF(condition)               PLOG_IF(plog::warning, condition)
#define LOG_ERROR_IF(condition)                 PLOG_IF(plog::error, condition)
#define LOG_FATAL_IF(condition)                 PLOG_IF(plog::fatal, condition)
#define LOG_NONE_IF(condition)                  PLOG_IF(plog::none, condition)

#define LOG_VERBOSE_IF_(instanceId, condition)  PLOG_IF_(instanceId, plog::verbose, condition)
#define LOG_DEBUG_IF_(instanceId, condition)    PLOG_IF_(instanceId, plog::debug, condition)
#define LOG_INFO_IF_(instanceId, condition)     PLOG_IF_(instanceId, plog::info, condition)
#define LOG_WARNING_IF_(instanceId, condition)  PLOG_IF_(instanceId, plog::warning, condition)
#define LOG_ERROR_IF_(instanceId, condition)    PLOG_IF_(instanceId, plog::error, condition)
#define LOG_FATAL_IF_(instanceId, condition)    PLOG_IF_(instanceId, plog::fatal, condition)
#define LOG_NONE_IF_(instanceId, condition)     PLOG_IF_(instanceId, plog::none, condition)

#define LOGV_IF(condition)                      PLOG_VERBOSE_IF(condition)
#define LOGD_IF(condition)                      PLOG_DEBUG_IF(condition)
#define LOGI_IF(condition)                      PLOG_INFO_IF(condition)
#define LOGW_IF(condition)                      PLOG_WARNING_IF(condition)
#define LOGE_IF(condition)                      PLOG_ERROR_IF(condition)
#define LOGF_IF(condition)                      PLOG_FATAL_IF(condition)
#define LOGN_IF(condition)                      PLOG_NONE_IF(condition)

#define LOGV_IF_(instanceId, condition)         PLOG_VERBOSE_IF_(instanceId, condition)
#define LOGD_IF_(instanceId, condition)         PLOG_DEBUG_IF_(instanceId, condition)
#define LOGI_IF_(instanceId, condition)         PLOG_INFO_IF_(instanceId, condition)
#define LOGW_IF_(instanceId, condition)         PLOG_WARNING_IF_(instanceId, condition)
#define LOGE_IF_(instanceId, condition)         PLOG_ERROR_IF_(instanceId, condition)
#define LOGF_IF_(instanceId, condition)         PLOG_FATAL_IF_(instanceId, condition)
#define LOGN_IF_(instanceId, condition)         PLOG_NONE_IF_(instanceId, condition)

#endif
