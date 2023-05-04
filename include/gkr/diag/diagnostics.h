#ifndef DIAG_UNIT_TESTING
#pragma once
#endif

#ifndef DIAG_ONE_TIME_DEFS
#define DIAG_ONE_TIME_DEFS

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
#define DIAG_ID_CHECK_RECOVERY  9
#define DIAG_ID_ARG_NOT_VALID   10
#define DIAG_ID_ARG_IS_NULL     11
#define DIAG_ID_ARG_FAILURE     12
#define DIAG_ID_ARG_BAD_ARRAY   13
#define DIAG_ID_COUNT           14

//
// Diagnostic source location information type
//
#define DIAG_SRC_INFO_DISABLED        0 /* Disabled information for source location - C and C++ */
#define DIAG_SRC_INFO_PREPROCESSOR    1 /* Preprocessor information for source location - only file and line - C and C++ */
#define DIAG_SRC_INFO_SOURCE_LOCATION 2 /* std::source_location information for source location - C++ 20 and later */
#define DIAG_SRC_INFO_STACKTRACE      3 /* std::stacktrace information for source location - C++ 23 and later */

//
// Diagnostic mode
//
#define DIAG_MODE_DISABLED  0 /* Disables all diagnostics */
#define DIAG_MODE_SILENT    1 /* Asserts are disabled and checks are silient - default for RELASE builds */
#define DIAG_MODE_STEADY    2 /* Asserts are disabled and checks warns on fail */
#define DIAG_MODE_NOISY     3 /* Asserts are enabled and checks warns on fail - default for DEBUG builds */
#define DIAG_MODE_INTRUSIVE 4 /* All diagnostics halts/stops execution */

#ifdef __cplusplus
extern "C"
{
#endif

//
// diag_trap - immediately breaks program execution
//
inline void diag_trap()
{
#if defined(__clang__) || defined(__GNUC__)
    __builtin_trap();
#elif defined(_MSC_VER)
    __debugbreak();
#else
    *(int*)0 = 0;
#endif
}

//
// diag_c_halt - used from all Assert_xxx macros when they are enabled
//
inline void diag_c_halt(int, const char*, ...)
{
    diag_trap();
}
//
// diag_c_stop - used from all Check_xxx macros in intrusive diagnostic mode
//
inline int diag_c_stop(int, const char*, ...)
{
    diag_trap();
    return 1;
}
//
// diag_cpp_warn - used from all Check_xxx macros for warns fails
//
inline int diag_c_warn(int, const char*, ...)
{
    return 1;
}

#ifdef __cplusplus
}
#endif

#ifdef __cplusplus

//
// diag_cpp_halt - used from all Assert_xxx macros when they are enabled
//
inline void diag_cpp_halt(int, const char*, ...) noexcept
{
    diag_trap();
}
//
// diag_cpp_stop - used from all Check_xxx macros in intrusive diagnostic mode
//
inline int diag_cpp_stop(int, const char*, ...) noexcept
{
    diag_trap();
    return 1;
}
//
// diag_cpp_warn - used from all Check_xxx macros for warns fails
//
inline int diag_cpp_warn(int, const char*, ...) noexcept
{
    return 1;
}

#endif

#endif

//
// If DIAG_MODE is not defined in advance tries to determine the right value
//
#ifndef DIAG_MODE
#if defined(_DEBUG)
#define DIAG_MODE DIAG_MODE_NOISY
#elif defined(NDEBUG)
#define DIAG_MODE DIAG_MODE_SILENT
#else
#define DIAG_MODE DIAG_MODE_STEADY
#endif
#endif

//
// If DIAG_SRC_INFO is not defined in advance tries to determine the right value
//
#ifndef DIAG_SRC_INFO
#if defined(__cpp_lib_stacktrace)
#define DIAG_SRC_INFO DIAG_SRC_INFO_STACKTRACE
#elif defined(__cpp_lib_source_location)
#define DIAG_SRC_INFO DIAG_SRC_INFO_SOURCE_LOCATION
#else
#define DIAG_SRC_INFO DIAG_SRC_INFO_PREPROCESSOR
#endif
#endif

//
// Set appropriate DIAG_SRC_LOCATION and DIAG_SRC_PROTOTYPE used for DIAG_xxx functions
//
#if   (DIAG_SRC_INFO == DIAG_SRC_INFO_DISABLED)
#define DIAG_SRC_LOCATION
#define DIAG_SRC_PROTOTYPE
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_PREPROCESSOR)
#define DIAG_SRC_LOCATION  , __FILE__, __LINE__
#define DIAG_SRC_PROTOTYPE , const char* file, int line
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_SOURCE_LOCATION)
#if !defined(__cplusplus)
#error Not available for C language
#elif !defined(__cpp_lib_source_location)
#error You must use at least C++20
#else
#include <source_location>
#define  DIAG_SRC_LOCATION  ,       std::source_location::current()
#define  DIAG_SRC_PROTOTYPE , const std::source_location& location
#endif
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_STACKTRACE)
#if !defined(__cplusplus)
#error Not available for C language
#elif !defined(__cpp_lib_source_location)
#error You must use at least C++23
#else
#include <stacktrace>
#define  DIAG_SRC_LOCATION  ,       std::stacktrace::current()
#define  DIAG_SRC_PROTOTYPE , const std::stacktrace& stacktrace
#endif
#else
//
// DIAG_SRC_LOCATION and DIAG_SRC_PROTOTYPE are user-defined
//
#endif

//
// DIAG_xxx functions - define DIAG_EXTERNAL_API if you want to implement your own versions of them
//
#ifdef DIAG_EXTERNAL_API

#ifdef __cplusplus
//
// You must define DIAG_NOEXCEPT to true or false when you implement own versions of them
//
#ifndef DIAG_NOEXCEPT
#error  DIAG_NOEXCEPT must be defined
#endif
#endif

#else

#ifdef __cplusplus
//
// Indicates whether DIAG_HALT and DIAG_STOP functions are noexcept - DIAG_WARN always must be noexcept
//
#define DIAG_NOEXCEPT true
//
// APIs
//
#define DIAG_HALT diag_cpp_halt
#define DIAG_STOP diag_cpp_stop
#define DIAG_WARN diag_cpp_warn
#else
//
// Must be defined anyway
//
#define DIAG_NOEXCEPT
//
// APIs
//
#define DIAG_HALT diag_c_halt
#define DIAG_STOP diag_c_stop
#define DIAG_WARN diag_c_warn
#endif

#endif

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
#define Assert_FailureMsg(msg)
#elif (DIAG_MODE <= DIAG_MODE_INTRUSIVE)
//
// Asserts are enabled
//
#define Assert_NotNullPtr(ptr)      if((ptr)==NULL) DIAG_HALT(DIAG_ID_ASSERT_NULL_PTR, #ptr   DIAG_SRC_LOCATION)
#define Assert_Check(check)         if(!(check)   ) DIAG_HALT(DIAG_ID_ASSERT_CHECK   , #check DIAG_SRC_LOCATION)
#define Assert_CheckMsg(check, msg) if(!(check)   ) DIAG_HALT(DIAG_ID_ASSERT_COND_MSG, msg    DIAG_SRC_LOCATION)
#define Assert_Failure()                            DIAG_HALT(DIAG_ID_ASSERT_FAILURE , NULL   DIAG_SRC_LOCATION)
#define Assert_FailureMsg(msg)                      DIAG_HALT(DIAG_ID_ASSERT_FAIL_MSG, msg    DIAG_SRC_LOCATION)
#else
//
// Asserts are user-defined
//
#endif

//
// Check_xxx
//
#if (DIAG_MODE == DIAG_MODE_DISABLED)
//
// Checks are disabled
//
#define Check_NotNullPtr(ptr,    ...)
#define Check_ValidState(check,  ...)
#define Check_FailureMsg(msg,    ...)   return __VA_ARGS__
#define Check_Failure(           ...)   return __VA_ARGS__
#define Check_Recovery(msg          )

#define Check_Arg_IsValid(check, ...)
#define Check_Arg_NotNull(ptr,   ...)
#define Check_Arg_Invalid(arg,   ...)   return __VA_ARGS__

#define Check_Arg_Array(ndx, cnt, check, ...)

#elif (DIAG_MODE == DIAG_MODE_SILENT)
//
// Checks are silint
//
#define Check_NotNullPtr(ptr,    ...)   if(((ptr)==NULL)) return __VA_ARGS__
#define Check_ValidState(check,  ...)   if(!(check)     ) return __VA_ARGS__
#define Check_FailureMsg(msg,    ...)                     return __VA_ARGS__
#define Check_Failure(           ...)                     return __VA_ARGS__
#define Check_Recovery(msg          )

#define Check_Arg_IsValid(check, ...)   if(!(check)     ) return __VA_ARGS__
#define Check_Arg_NotNull(ptr,   ...)   if(((ptr)==NULL)) return __VA_ARGS__
#define Check_Arg_Invalid(arg,   ...)                     return __VA_ARGS__

#ifdef __cplusplus
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check)) return __VA_ARGS__
#else
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(int ndx = 0; ndx < (int)(cnt); ++ndx)      if(!(check)) return __VA_ARGS__
#endif

#elif (DIAG_MODE == DIAG_MODE_STEADY) || (DIAG_MODE == DIAG_MODE_NOISY)
//
// Checks makes warns fails
//
#define Check_NotNullPtr(ptr,    ...)   if(((ptr)==NULL) && DIAG_WARN(DIAG_ID_CHECK_NULL_PTR , #ptr   DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_ValidState(check,  ...)   if(!(check)      && DIAG_WARN(DIAG_ID_CHECK_STATE    , #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_FailureMsg(msg,    ...)                       DIAG_WARN(DIAG_ID_CHECK_FAIL_MSG , msg    DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Failure(           ...)                       DIAG_WARN(DIAG_ID_CHECK_FAILURE  , NULL   DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Recovery(msg          )                       DIAG_WARN(DIAG_ID_CHECK_RECOVERY , msg    DIAG_SRC_LOCATION)

#define Check_Arg_IsValid(check, ...)   if(!(check)      && DIAG_WARN(DIAG_ID_ARG_NOT_VALID  , #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Arg_NotNull(ptr,   ...)   if(((ptr)==NULL) && DIAG_WARN(DIAG_ID_ARG_IS_NULL    , #ptr   DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Arg_Invalid(arg,   ...)                       DIAG_WARN(DIAG_ID_ARG_FAILURE    , #arg   DIAG_SRC_LOCATION); return __VA_ARGS__

#ifdef __cplusplus
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && DIAG_WARN(DIAG_ID_ARG_BAD_ARRAY, #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#else
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(int ndx = 0; ndx < (int)(cnt); ++ndx)      if(!(check) && DIAG_WARN(DIAG_ID_ARG_BAD_ARRAY, #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#endif

#elif (DIAG_MODE == DIAG_MODE_INTRUSIVE)
//
// Checks stops execution
//
#define Check_NotNullPtr(ptr,    ...)   if(((ptr)==NULL) && DIAG_STOP(DIAG_ID_CHECK_NULL_PTR , #ptr   DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_ValidState(check,  ...)   if(!(check)      && DIAG_STOP(DIAG_ID_CHECK_STATE    , #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_FailureMsg(msg,    ...)                       DIAG_STOP(DIAG_ID_CHECK_FAIL_MSG , msg    DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Failure(           ...)                       DIAG_STOP(DIAG_ID_CHECK_FAILURE  , NULL   DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Recovery(msg          )                    if(DIAG_STOP(DIAG_ID_CHECK_RECOVERY , msg    DIAG_SRC_LOCATION)) (void)0

#define Check_Arg_IsValid(check, ...)   if(!(check)      && DIAG_STOP(DIAG_ID_ARG_NOT_VALID  , #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Arg_NotNull(ptr,   ...)   if(((ptr)==NULL) && DIAG_STOP(DIAG_ID_ARG_IS_NULL    , #ptr   DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Arg_Invalid(arg,   ...)                       DIAG_STOP(DIAG_ID_ARG_FAILURE    , #arg   DIAG_SRC_LOCATION); return __VA_ARGS__

#ifdef __cplusplus
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && DIAG_WARN(DIAG_ID_ARG_BAD_ARRAY, #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#else
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(int ndx = 0; ndx < (int)(cnt); ++ndx)      if(!(check) && DIAG_WARN(DIAG_ID_ARG_BAD_ARRAY, #check DIAG_SRC_LOCATION)) return __VA_ARGS__
#endif

#else
//
// Checks are user-defined
//
#endif
