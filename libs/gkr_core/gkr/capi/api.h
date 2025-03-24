#pragma once

#ifdef GKR_STATIC_LIBS
#define GKR_API_EXPORT
#define GKR_API_IMPORT
#else
#ifdef _WIN32
#define GKR_API_EXPORT  __declspec(dllexport)
#define GKR_API_IMPORT  __declspec(dllimport)
#else
#define GKR_API_EXPORT  __attribute__((__visibility__("default")))
#define GKR_API_IMPORT
#endif
#endif

#ifdef __cplusplus
#define GKR_NO_RETURN_API   [[noreturn]]
#else
#if (__STDC_VERSION__ >= 202311L)
#define GKR_NO_RETURN_API   [[noreturn]]
#else
#define GKR_NO_RETURN_API   _Noreturn
#endif
#endif

#ifdef BUILDING_GKR_CORE
#define GKR_CORE_API    GKR_API_EXPORT
#else
#define GKR_CORE_API    GKR_API_IMPORT
#endif
#ifdef BUILDING_GKR_LOG
#define GKR_LOG_API     GKR_API_EXPORT
#else
#define GKR_LOG_API     GKR_API_IMPORT
#endif

#define GKR_INNER_API   GKR_CORE_API
