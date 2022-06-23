#pragma once

//
// Indicates whether DIAG_HALT and DIAG_STOP functions are noexcept - DIAG_WARN always must be noexcept
//
#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif

//
// Diagnostic mode
//
#define DIAG_MODE_DISABLED  0 /* Disables all diagnostics */
#define DIAG_MODE_DEFAULT   1 /* Asserts are disabled and checks are silient - default for RELASE builds */
#define DIAG_MODE_STEADY    2 /* Asserts are disabled and checks warns on fail */
#define DIAG_MODE_NOISY     3 /* Asserts are enabled and checks warns on fail - default for DEBUG builds */
#define DIAG_MODE_INTRUSIVE 4 /* All diagnostics stops execution */

//
// If DIAG_MODE is not defined in advance tries to determine the right value
//
#ifndef DIAG_MODE
#if defined(_DEBUG)
#define DIAG_MODE DIAG_MODE_NOISY
#elif defined(NDEBUG)
#define DIAG_MODE DIAG_MODE_DEFAULT
#else
#define DIAG_MODE DIAG_MODE_STEADY
#endif
#endif

//
// Diagnostic source location information type
//
#define DIAG_SRC_INFO_DISABLED        0 /* Disabled information for source location - C and C++ */
#define DIAG_SRC_INFO_DEFAULT         1 /* Default information for source location - only file and line - C and C++ */
#define DIAG_SRC_INFO_SOURCE_LOCATION 2 /* std::source_location information for source location - C++ 20 only */
#define DIAG_SRC_INFO_STACKTRACE      3 /* std::stacktrace information for source location - C++ 23 only */

//
// If DIAG_SRC_INFO is not defined in advance tries to determine the right value
//
#ifndef DIAG_SRC_INFO
#if defined(__has_include)
#if __has_include(<stacktrace>)
#include <stacktrace>
#endif
#if __has_include(<source_location>) && defined(__cpp_consteval) && !defined(__cpp_lib_stacktrace)
#include <source_location>
#endif
#endif
#if   defined(__cpp_lib_stacktrace)
#define DIAG_SRC_INFO DIAG_SRC_INFO_STACKTRACE
#elif defined(__cpp_lib_source_location)
#define DIAG_SRC_INFO DIAG_SRC_INFO_SOURCE_LOCATION
#else
#define DIAG_SRC_INFO DIAG_SRC_INFO_DEFAULT
#endif
#endif

//
// Set appropriate DIAG_SRC_LOCATION and DIAG_SRC_PROTOTYPE used for DIAG_xxx functions
//
#if   (DIAG_SRC_INFO == DIAG_SRC_INFO_DISABLED)
#define DIAG_SRC_LOCATION
#define DIAG_SRC_PROTOTYPE
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_DEFAULT)
#define DIAG_SRC_LOCATION  , __FILE__, __LINE__
#define DIAG_SRC_PROTOTYPE , const char* file, int line
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_SOURCE_LOCATION)
#include <source_location>
#define  DIAG_SRC_LOCATION  ,       std::source_location::current()
#define  DIAG_SRC_PROTOTYPE , const std::source_location& location
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_STACKTRACE)
#include <stacktrace>
#define  DIAG_SRC_LOCATION  ,       std::stacktrace::current()
#define  DIAG_SRC_PROTOTYPE , const std::stacktrace& stacktrace
#else
#error Unknown source location information type
#endif

//
// Diagnostics IDs for Assert_xxx macros
//
#define DIAG_ID_ASSERT_NULL_PTR 0
#define DIAG_ID_ASSERT_CHECK    1
#define DIAG_ID_ASSERT_COND_MSG 2
#define DIAG_ID_ASSERT_FAILURE  3
#define DIAG_ID_ASSERT_FAIL_MSG 4
//
// Diagnostics IDs for Check_xxx macros
//
#define DIAG_ID_CHECK_NULL_PTR  5
#define DIAG_ID_CHECK_STATE     6
#define DIAG_ID_CHECK_FAIL_MSG  7
#define DIAG_ID_CHECK_FAILURE   8
#define DIAG_ID_ARG_NOT_VALID   9
#define DIAG_ID_ARG_NOT_NULL    10
#define DIAG_ID_ARG_INVALID     11
#define DIAG_ID_ARG_BAD_ARRAY   12
#define DIAG_ID_COUNT           13

//
// Assert_xxx
//
#if (DIAG_MODE <= DIAG_MODE_STEADY)
//
// Asserts are disabled
//
#define Assert_NotNullPtr(ptr)
#define Assert_Check(check)
#define Assert_CheckMsg(check, msg)
#define Assert_Failure()
#define Assert_FailureMsg()
#else
//
// Asserts are enabled
//
#define Assert_NotNullPtr(ptr)      if((ptr)==NULL) DIAG_HALT(DIAG_ID_ASSERT_NULL_PTR, #ptr   DIAG_SRC_LOCATION)
#define Assert_Check(check)         if(!(check)   ) DIAG_HALT(DIAG_ID_ASSERT_CHECK   , #check DIAG_SRC_LOCATION)
#define Assert_CheckMsg(check, msg) if(!(check)   ) DIAG_HALT(DIAG_ID_ASSERT_COND_MSG, msg    DIAG_SRC_LOCATION)
#define Assert_Failure()                            DIAG_HALT(DIAG_ID_ASSERT_FAILURE , NULL   DIAG_SRC_LOCATION)
#define Assert_FailureMsg(msg)                      DIAG_HALT(DIAG_ID_ASSERT_FAIL_MSG, msg    DIAG_SRC_LOCATION)
#endif

#if (DIAG_MODE == DIAG_MODE_DISABLED)
//
// Checks are disabled
//
#define Check_ValidState(check, ...)
#define Check_NotNullPtr(ptr,   ...)
#define Check_Failure(          ...)    return __VA_ARGS__

#define Check_ValidArg(check, ...)
#define Check_NotNullArg(ptr, ...)
#define Check_InvalidArg(arg, ...)      return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...)

#elif (DIAG_MODE == DIAG_MODE_DEFAULT)
//
// Checks are silint
//
#define Check_ValidState(check, ...)    if(!(check)   ) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if((ptr)==NULL) return __VA_ARGS__
#define Check_Failure(          ...)                    return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)   ) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if((ptr)==NULL) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                      return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check)) return __VA_ARGS__

#elif (DIAG_MODE == DIAG_MODE_STEADY) || (DIAG_MODE == DIAG_MODE_NOISY)
//
// Checks makes warns fails
//
#define Check_Arg_IsValid(check, ...)
#define Check_Arg_NotNull(ptr,   ...)
#define Check_Arg_Failure(arg,   ...)

#define Check_Arg_Array(ndx, cnt, check, ...)

#define Check_NotNullPtr(ptr,   ...)    if(((ptr)==NULL) && DIAG_WARN(DIAG_ID_CHECK_NULL_PTR , #ptr   DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_ValidState(check, ...)    if(!(check)      && DIAG_WARN(DIAG_ID_CHECK_STATE    , #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_FailureMsg(msg,   ...)                        DIAG_WARN(DIAG_ID_CHECK_FAIL_MSG , msg    DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Failure(          ...)                        DIAG_WARN(DIAG_ID_CHECK_FAILURE  , NULL   DIAG_SRC_LOCATION); return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)      && DIAG_WARN(DIAG_ID_ARG_NOT_VALID  , #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if(((ptr)==NULL) && DIAG_WARN(DIAG_ID_ARG_NOT_NULL   , #ptr   DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                          DIAG_WARN(DIAG_ID_ARG_INVALID    , #arg   DIAG_SRC_LOCATION); return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && DIAG_WARN(DIAG_ID_ARG_BAD_ARRAY, #check DIAG_SRC_LOCATION)) return __VA_ARGS__

#elif (DIAG_MODE == DIAG_MODE_INTRUSIVE)
//
// Checks stops execution
//
#define Check_ValidState(check, ...)    if(!(check)       && DIAG_STOP(DIAG_ID_VALID_STATE  , #check  DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if((ptr)==nullptr && DIAG_STOP(DIAG_ID_NOT_NULL_PTR , #ptr    DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Failure(          ...)                         DIAG_STOP(DIAG_ID_STATE_FAILURE, nullptr DIAG_SRC_LOCATION); return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)       && DIAG_STOP(DIAG_ID_NOT_VALID_ARG, #check  DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if((ptr)==nullptr && DIAG_STOP(DIAG_ID_NOT_NULL_ARG , #ptr    DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                           DIAG_STOP(DIAG_ID_INVALID_ARG  , #arg    DIAG_SRC_LOCATION); return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && DIAG_STOP(DIAG_ID_BAD_ARRAY_ARG, #check DIAG_SRC_LOCATION)) return __VA_ARGS__

#else
#error Unknown diagnostics mode
#endif

//
// DIAG_xxx functions - define DIAG_EXTERNAL_API if you want to implement your own versions of them
//
#ifndef DIAG_EXTERNAL_API

//
// DIAG_HALT - used from all Assert_xxx macros when they are enabled
//
[[noreturn]]
inline void DIAG_HALT(int, const char* DIAG_SRC_PROTOTYPE)
#ifdef __cplusplus
noexcept(DIAG_NOEXCEPT)
#endif
{
#if defined(__clang__) || defined(__GNUC__)
    __builtin_trap();
#elif defined(_MSC_VER)
    __debugbreak();
#else
    *(int*)nullptr = 0;
#endif
}

//
// DIAG_STOP - used from all Check_xxx macros in intrusive diagnostic mode
//
#if defined(__clang__)
[[noreturn]]
#endif
inline bool DIAG_STOP(int, const char* DIAG_SRC_PROTOTYPE)
#ifdef __cplusplus
noexcept(DIAG_NOEXCEPT)
#endif
{
#if defined(__clang__) || defined(__GNUC__)
    __builtin_trap();
#elif defined(_MSC_VER)
    __debugbreak();
#else
    *(int*)nullptr = 0;
#endif
}

//
// DIAG_WARN - used from all Check_xxx macros for warns fails
//
inline bool DIAG_WARN(int, const char* DIAG_SRC_PROTOTYPE)
#ifdef __cplusplus
noexcept
#endif
{
    return true;
}
#endif
