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
#ifndef GKR_LOG_MODULE_KEEP
#define GKR_LOG_MODULE_KEEP
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
#elif   defined(GKR_LOG_MODULE_KEEP)
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

#ifndef GKR_LOG_MODULE_KEEP
#define GKR_LOG_MODULE_KEEP
#endif
//
// LOGxxx Alias is module LOG - mLOGxxx
//
#define LOG             mLOG
#define LOG_IF          mLOG_IF

#define LOGF            mLOGF
#define LOGE            mLOGE
#define LOGW            mLOGW
#define LOGI            mLOGI
#define LOGV            mLOGV
#define LOGD            mLOGD
#define LOGT            mLOGT

#define LOG_FATAL       mLOG_FATAL
#define LOG_ERROR       mLOG_ERROR
#define LOG_WARNING     mLOG_WARNING
#define LOG_INFO        mLOG_INFO
#define LOG_VERBOSE     mLOG_VERBOSE
#define LOG_DEBUG       mLOG_DEBUG
#define LOG_TRACE       mLOG_TRACE

#define LOGF_IF         mLOGF_IF
#define LOGE_IF         mLOGE_IF
#define LOGW_IF         mLOGW_IF
#define LOGI_IF         mLOGI_IF
#define LOGV_IF         mLOGV_IF
#define LOGD_IF         mLOGD_IF
#define LOGT_IF         mLOGT_IF

#define LOG_FATAL_IF    mLOG_FATAL_IF
#define LOG_ERROR_IF    mLOG_ERROR_IF
#define LOG_WARNING_IF  mLOG_WARNING_IF
#define LOG_INFO_IF     mLOG_INFO_IF
#define LOG_VERBOSE_IF  mLOG_VERBOSE_IF
#define LOG_DEBUG_IF    mLOG_DEBUG_IF
#define LOG_TRACE_IF    mLOG_TRACE_IF

#ifndef GKR_NO_PRINTF_LOGGING
#define LOG_            mLOG_
#define LOG_IF_         mLOG_IF_

#define LOGF_           mLOGF_
#define LOGE_           mLOGE_
#define LOGW_           mLOGW_
#define LOGI_           mLOGI_
#define LOGV_           mLOGV_
#define LOGD_           mLOGD_
#define LOGT_           mLOGT_

#define LOG_FATAL_      mLOG_FATAL_
#define LOG_ERROR_      mLOG_ERROR_
#define LOG_WARNING_    mLOG_WARNING_
#define LOG_INFO_       mLOG_INFO_
#define LOG_VERBOSE_    mLOG_VERBOSE_
#define LOG_DEBUG_      mLOG_DEBUG_
#define LOG_TRACE_      mLOG_TRACE_

#define LOGF_IF_        mLOGF_IF_
#define LOGE_IF_        mLOGE_IF_
#define LOGW_IF_        mLOGW_IF_
#define LOGI_IF_        mLOGI_IF_
#define LOGV_IF_        mLOGV_IF_
#define LOGD_IF_        mLOGD_IF_
#define LOGT_IF_        mLOGT_IF_

#define LOG_FATAL_IF_   mLOG_FATAL_IF_
#define LOG_ERROR_IF_   mLOG_ERROR_IF_
#define LOG_WARNING_IF_ mLOG_WARNING_IF_
#define LOG_INFO_IF_    mLOG_INFO_IF_
#define LOG_VERBOSE_IF_ mLOG_VERBOSE_IF_
#define LOG_DEBUG_IF_   mLOG_DEBUG_IF_
#define LOG_TRACE_IF_   mLOG_TRACE_IF_
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
