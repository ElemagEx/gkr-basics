#pragma once

#ifndef GKR_LOG_GENERIC
#define GKR_LOG_GENERIC

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
    {(const char*)0 , 0}}

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

#ifdef GKR_LOG_ALL_KEEP
#ifndef GKR_LOG_BASIC_KEEP
#define GKR_LOG_BASIC_KEEP
#endif
#ifndef GKR_LOG_FACILITY_KEEP
#define GKR_LOG_FACILITY_KEEP
#endif
#ifndef GKR_LOG_CHANNEL_KEEP
#define GKR_LOG_CHANNEL_KEEP
#endif
#ifndef GKR_LOG_GENERAL_KEEP
#define GKR_LOG_GENERAL_KEEP
#endif
#endif

#ifndef GKR_LOG_ALIAS
#if     defined(GKR_LOG_BASIC_KEEP)
#define GKR_LOG_ALIAS 0
#elif   defined(GKR_LOG_FACILITY_KEEP)
#define GKR_LOG_ALIAS 1
#elif   defined(GKR_LOG_CHANNEL_KEEP)
#define GKR_LOG_ALIAS 2
#elif   defined(GKR_LOG_GENERAL_KEEP)
#define GKR_LOG_ALIAS 3
#else
#define GKR_LOG_ALIAS 0
#endif
#endif

#if (GKR_LOG_ALIAS == 0)

#ifndef GKR_LOG_BASIC_KEEP
#define GKR_LOG_BASIC_KEEP
#endif
//
// LOGxxx Alias is basic LOG - bLOGxxx
//
#define LOG             bLOG
#define LOG_IF          bLOG_IF

#define LOGF            bLOGF
#define LOGE            bLOGE
#define LOGW            bLOGW
#define LOGI            bLOGI
#define LOGV            bLOGV
#define LOGD            bLOGD
#define LOGT            bLOGT

#define LOG_FATAL       bLOG_FATAL
#define LOG_ERROR       bLOG_ERROR
#define LOG_WARNING     bLOG_WARNING
#define LOG_INFO        bLOG_INFO
#define LOG_VERBOSE     bLOG_VERBOSE
#define LOG_DEBUG       bLOG_DEBUG
#define LOG_TRACE       bLOG_TRACE

#define LOGF_IF         bLOGF_IF
#define LOGE_IF         bLOGE_IF
#define LOGW_IF         bLOGW_IF
#define LOGI_IF         bLOGI_IF
#define LOGV_IF         bLOGV_IF
#define LOGD_IF         bLOGD_IF
#define LOGT_IF         bLOGT_IF

#define LOG_FATAL_IF    bLOG_FATAL_IF
#define LOG_ERROR_IF    bLOG_ERROR_IF
#define LOG_WARNING_IF  bLOG_WARNING_IF
#define LOG_INFO_IF     bLOG_INFO_IF
#define LOG_VERBOSE_IF  bLOG_VERBOSE_IF
#define LOG_DEBUG_IF    bLOG_DEBUG_IF
#define LOG_TRACE_IF    bLOG_TRACE_IF

#ifndef GKR_NO_PRINTF_LOGGING
#define LOG_            bLOG_
#define LOG_IF_         bLOG_IF_

#define LOGF_           bLOGF_
#define LOGE_           bLOGE_
#define LOGW_           bLOGW_
#define LOGI_           bLOGI_
#define LOGV_           bLOGV_
#define LOGD_           bLOGD_
#define LOGT_           bLOGT_

#define LOG_FATAL_      bLOG_FATAL_
#define LOG_ERROR_      bLOG_ERROR_
#define LOG_WARNING_    bLOG_WARNING_
#define LOG_INFO_       bLOG_INFO_
#define LOG_VERBOSE_    bLOG_VERBOSE_
#define LOG_DEBUG_      bLOG_DEBUG_
#define LOG_TRACE_      bLOG_TRACE_

#define LOGF_IF_        bLOGF_IF_
#define LOGE_IF_        bLOGE_IF_
#define LOGW_IF_        bLOGW_IF_
#define LOGI_IF_        bLOGI_IF_
#define LOGV_IF_        bLOGV_IF_
#define LOGD_IF_        bLOGD_IF_
#define LOGT_IF_        bLOGT_IF_

#define LOG_FATAL_IF_   bLOG_FATAL_IF_
#define LOG_ERROR_IF_   bLOG_ERROR_IF_
#define LOG_WARNING_IF_ bLOG_WARNING_IF_
#define LOG_INFO_IF_    bLOG_INFO_IF_
#define LOG_VERBOSE_IF_ bLOG_VERBOSE_IF_
#define LOG_DEBUG_IF_   bLOG_DEBUG_IF_
#define LOG_TRACE_IF_   bLOG_TRACE_IF_
#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#elif (GKR_LOG_ALIAS == 1)

#ifndef GKR_LOG_FACILITY_KEEP
#define GKR_LOG_FACILITY_KEEP
#endif
//
// LOGxxx Alias is facility LOG - fLOGxxx
//
#define LOG             fLOG
#define LOG_IF          fLOG_IF

#define LOGF            fLOGF
#define LOGE            fLOGE
#define LOGW            fLOGW
#define LOGI            fLOGI
#define LOGV            fLOGV
#define LOGD            fLOGD
#define LOGT            fLOGT

#define LOG_FATAL       fLOG_FATAL
#define LOG_ERROR       fLOG_ERROR
#define LOG_WARNING     fLOG_WARNING
#define LOG_INFO        fLOG_INFO
#define LOG_VERBOSE     fLOG_VERBOSE
#define LOG_DEBUG       fLOG_DEBUG
#define LOG_TRACE       fLOG_TRACE

#define LOGF_IF         fLOGF_IF
#define LOGE_IF         fLOGE_IF
#define LOGW_IF         fLOGW_IF
#define LOGI_IF         fLOGI_IF
#define LOGV_IF         fLOGV_IF
#define LOGD_IF         fLOGD_IF
#define LOGT_IF         fLOGT_IF

#define LOG_FATAL_IF    fLOG_FATAL_IF
#define LOG_ERROR_IF    fLOG_ERROR_IF
#define LOG_WARNING_IF  fLOG_WARNING_IF
#define LOG_INFO_IF     fLOG_INFO_IF
#define LOG_VERBOSE_IF  fLOG_VERBOSE_IF
#define LOG_DEBUG_IF    fLOG_DEBUG_IF
#define LOG_TRACE_IF    fLOG_TRACE_IF

#ifndef GKR_NO_PRINTF_LOGGING
#define LOG_            fLOG_
#define LOG_IF_         fLOG_IF_

#define LOGF_           fLOGF_
#define LOGE_           fLOGE_
#define LOGW_           fLOGW_
#define LOGI_           fLOGI_
#define LOGV_           fLOGV_
#define LOGD_           fLOGD_
#define LOGT_           fLOGT_

#define LOG_FATAL_      fLOG_FATAL_
#define LOG_ERROR_      fLOG_ERROR_
#define LOG_WARNING_    fLOG_WARNING_
#define LOG_INFO_       fLOG_INFO_
#define LOG_VERBOSE_    fLOG_VERBOSE_
#define LOG_DEBUG_      fLOG_DEBUG_
#define LOG_TRACE_      fLOG_TRACE_

#define LOGF_IF_        fLOGF_IF_
#define LOGE_IF_        fLOGE_IF_
#define LOGW_IF_        fLOGW_IF_
#define LOGI_IF_        fLOGI_IF_
#define LOGV_IF_        fLOGV_IF_
#define LOGD_IF_        fLOGD_IF_
#define LOGT_IF_        fLOGT_IF_

#define LOG_FATAL_IF_   fLOG_FATAL_IF_
#define LOG_ERROR_IF_   fLOG_ERROR_IF_
#define LOG_WARNING_IF_ fLOG_WARNING_IF_
#define LOG_INFO_IF_    fLOG_INFO_IF_
#define LOG_VERBOSE_IF_ fLOG_VERBOSE_IF_
#define LOG_DEBUG_IF_   fLOG_DEBUG_IF_
#define LOG_TRACE_IF_   fLOG_TRACE_IF_
#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#elif (GKR_LOG_ALIAS == 2)

#ifndef GKR_LOG_CHANNEL_KEEP
#define GKR_LOG_CHANNEL_KEEP
#endif
//
// LOGxxx Alias is channel LOG - cLOGxxx
//
#define LOG             cLOG
#define LOG_IF          cLOG_IF

#define LOGF            cLOGF
#define LOGE            cLOGE
#define LOGW            cLOGW
#define LOGI            cLOGI
#define LOGV            cLOGV
#define LOGD            cLOGD
#define LOGT            cLOGT

#define LOG_FATAL       cLOG_FATAL
#define LOG_ERROR       cLOG_ERROR
#define LOG_WARNING     cLOG_WARNING
#define LOG_INFO        cLOG_INFO
#define LOG_VERBOSE     cLOG_VERBOSE
#define LOG_DEBUG       cLOG_DEBUG
#define LOG_TRACE       cLOG_TRACE

#define LOGF_IF         cLOGF_IF
#define LOGE_IF         cLOGE_IF
#define LOGW_IF         cLOGW_IF
#define LOGI_IF         cLOGI_IF
#define LOGV_IF         cLOGV_IF
#define LOGD_IF         cLOGD_IF
#define LOGT_IF         cLOGT_IF

#define LOG_FATAL_IF    cLOG_FATAL_IF
#define LOG_ERROR_IF    cLOG_ERROR_IF
#define LOG_WARNING_IF  cLOG_WARNING_IF
#define LOG_INFO_IF     cLOG_INFO_IF
#define LOG_VERBOSE_IF  cLOG_VERBOSE_IF
#define LOG_DEBUG_IF    cLOG_DEBUG_IF
#define LOG_TRACE_IF    cLOG_TRACE_IF

#ifndef GKR_NO_PRINTF_LOGGING
#define LOG_            cLOG_
#define LOG_IF_         cLOG_IF_

#define LOGF_           cLOGF_
#define LOGE_           cLOGE_
#define LOGW_           cLOGW_
#define LOGI_           cLOGI_
#define LOGV_           cLOGV_
#define LOGD_           cLOGD_
#define LOGT_           cLOGT_

#define LOG_FATAL_      cLOG_FATAL_
#define LOG_ERROR_      cLOG_ERROR_
#define LOG_WARNING_    cLOG_WARNING_
#define LOG_INFO_       cLOG_INFO_
#define LOG_VERBOSE_    cLOG_VERBOSE_
#define LOG_DEBUG_      cLOG_DEBUG_
#define LOG_TRACE_      cLOG_TRACE_

#define LOGF_IF_        cLOGF_IF_
#define LOGE_IF_        cLOGE_IF_
#define LOGW_IF_        cLOGW_IF_
#define LOGI_IF_        cLOGI_IF_
#define LOGV_IF_        cLOGV_IF_
#define LOGD_IF_        cLOGD_IF_
#define LOGT_IF_        cLOGT_IF_

#define LOG_FATAL_IF_   cLOG_FATAL_IF_
#define LOG_ERROR_IF_   cLOG_ERROR_IF_
#define LOG_WARNING_IF_ cLOG_WARNING_IF_
#define LOG_INFO_IF_    cLOG_INFO_IF_
#define LOG_VERBOSE_IF_ cLOG_VERBOSE_IF_
#define LOG_DEBUG_IF_   cLOG_DEBUG_IF_
#define LOG_TRACE_IF_   cLOG_TRACE_IF_
#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#elif (GKR_LOG_ALIAS == 3)

#ifndef GKR_LOG_GENERAL_KEEP
#define GKR_LOG_GENERAL_KEEP
#endif
//
// LOGxxx Alias is general LOG - gLOGxxx
//
#define LOG             gLOG
#define LOG_IF          gLOG_IF

#define LOGF            gLOGF
#define LOGE            gLOGE
#define LOGW            gLOGW
#define LOGI            gLOGI
#define LOGV            gLOGV
#define LOGD            gLOGD
#define LOGT            gLOGT

#define LOG_FATAL       gLOG_FATAL
#define LOG_ERROR       gLOG_ERROR
#define LOG_WARNING     gLOG_WARNING
#define LOG_INFO        gLOG_INFO
#define LOG_VERBOSE     gLOG_VERBOSE
#define LOG_DEBUG       gLOG_DEBUG
#define LOG_TRACE       gLOG_TRACE

#define LOGF_IF         gLOGF_IF
#define LOGE_IF         gLOGE_IF
#define LOGW_IF         gLOGW_IF
#define LOGI_IF         gLOGI_IF
#define LOGV_IF         gLOGV_IF
#define LOGD_IF         gLOGD_IF
#define LOGT_IF         gLOGT_IF

#define LOG_FATAL_IF    gLOG_FATAL_IF
#define LOG_ERROR_IF    gLOG_ERROR_IF
#define LOG_WARNING_IF  gLOG_WARNING_IF
#define LOG_INFO_IF     gLOG_INFO_IF
#define LOG_VERBOSE_IF  gLOG_VERBOSE_IF
#define LOG_DEBUG_IF    gLOG_DEBUG_IF
#define LOG_TRACE_IF    gLOG_TRACE_IF

#ifndef GKR_NO_PRINTF_LOGGING
#define LOG_            gLOG_
#define LOG_IF_         gLOG_IF_

#define LOGF_           gLOGF_
#define LOGE_           gLOGE_
#define LOGW_           gLOGW_
#define LOGI_           gLOGI_
#define LOGV_           gLOGV_
#define LOGD_           gLOGD_
#define LOGT_           gLOGT_

#define LOG_FATAL_      gLOG_FATAL_
#define LOG_ERROR_      gLOG_ERROR_
#define LOG_WARNING_    gLOG_WARNING_
#define LOG_INFO_       gLOG_INFO_
#define LOG_VERBOSE_    gLOG_VERBOSE_
#define LOG_DEBUG_      gLOG_DEBUG_
#define LOG_TRACE_      gLOG_TRACE_

#define LOGF_IF_        gLOGF_IF_
#define LOGE_IF_        gLOGE_IF_
#define LOGW_IF_        gLOGW_IF_
#define LOGI_IF_        gLOGI_IF_
#define LOGV_IF_        gLOGV_IF_
#define LOGD_IF_        gLOGD_IF_
#define LOGT_IF_        gLOGT_IF_

#define LOG_FATAL_IF_   gLOG_FATAL_IF_
#define LOG_ERROR_IF_   gLOG_ERROR_IF_
#define LOG_WARNING_IF_ gLOG_WARNING_IF_
#define LOG_INFO_IF_    gLOG_INFO_IF_
#define LOG_VERBOSE_IF_ gLOG_VERBOSE_IF_
#define LOG_DEBUG_IF_   gLOG_DEBUG_IF_
#define LOG_TRACE_IF_   gLOG_TRACE_IF_
#endif /*ndef GKR_NO_PRINTF_LOGGING*/

#endif /*GKR_LOG_ALIAS != 0, 1, 2, 3*/

#if !defined(__cplusplus) || defined(LOG_USE_C_DEFS)
#include <gkr/capi/log/defs/generic_cdefs.h>
#endif

#endif /*ndef GKR_LOG_GENERIC*/
