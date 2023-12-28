#ifndef DIAG_UNIT_TESTING
#pragma once
#endif

#ifndef DIAG_ONE_TIME_C_DEFS
#define DIAG_ONE_TIME_C_DEFS

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
#define DIAG_SRC_INFO_NONE            0 /* None information for source location - C and C++ */
#define DIAG_SRC_INFO_PREPROCESSOR    1 /* Preprocessor information for source location - file and line - C and C++ */
#define DIAG_SRC_INFO_PREPROCESSOR_EX 2 /* Preprocessor information for source location - function, file, and line - C and C++ */
#define DIAG_SRC_INFO_SOURCE_LOCATION 3 /* std::source_location information for source location - C++ 20 and later */
#define DIAG_SRC_INFO_STACKTRACE      4 /* std::stacktrace information for source location - C++ 23 and later */

//
// Diagnostic mode
//
#define DIAG_MODE_DISABLED  0 /* Disables all diagnostics */
#define DIAG_MODE_SILENT    1 /* Asserts are disabled and checks are silent - default for Release builds */
#define DIAG_MODE_STEADY    2 /* Asserts are disabled and checks warns on fail */
#define DIAG_MODE_NOISY     3 /* Asserts halts execution and checks warns on fail - default for Debug builds */
#define DIAG_MODE_INTRUSIVE 4 /* All diagnostics halts execution */

//
// Null pointer
//
#ifdef __cplusplus
#define DIAG_NULL   nullptr
#else
#define DIAG_NULL   ((void*)0)
#endif
//
// No operation and function
//
#ifdef _MSC_VER
#define DIAG_NOOP   __noop
#define DIAG_FUNC   __FUNCSIG__
#else
#define DIAG_NOOP   ((void)0)
#define DIAG_FUNC   __PRETTY_FUNCTION__
#endif

#ifdef __cplusplus
extern "C"
{
#endif

//
// diag_trap - immediately breaks program execution
//
[[noreturn]]
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
// diag_true - always returns true
//
inline int diag_true()
{
    return 1;
}

//
// diag_halt - used from all Assert_xxx macros when they are enabled
//
[[noreturn]]
inline void diag_c_halt(int, const char*, ...)
{
    diag_trap();
}
//
// diag_warn - used from all Check_xxx macros for warns fails
//
inline int diag_c_warn(int, const char*, ...)
{
    return 1;
}

#ifdef __cplusplus
} // extern "C"
#endif
#endif /* ndef DIAG_ONE_C_TIME_DEFS */

//
// If DIAG_MODE is not defined in advance tries to determine the right value
//
#ifndef DIAG_MODE
#if defined(_DEBUG) || defined(DEBUG)
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
#if   (DIAG_SRC_INFO == DIAG_SRC_INFO_NONE)
#define DIAG_SRC_LOCATION
#define DIAG_SRC_PROTOTYPE
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_PREPROCESSOR)
#define DIAG_SRC_LOCATION  , __FILE__, __LINE__
#define DIAG_SRC_PROTOTYPE , const char* file, int line
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_PREPROCESSOR_EX)
#define DIAG_SRC_LOCATION  , DIAG_FUNC, __FILE__, __LINE__
#define DIAG_SRC_PROTOTYPE , const char* func, const char* file, int line
#else
//
// DIAG_SRC_LOCATION and DIAG_SRC_PROTOTYPE are defined elsewhere
//
#endif

//
// define DIAG_EXTERNAL_API and/or DIAG_HALT/DIAG_WARN if you want to implement your own versions of halt/warn functions
//
#ifndef __cplusplus
#ifndef DIAG_EXTERNAL_API
#define DIAG_HALT diag_c_halt
#define DIAG_WARN diag_c_warn
#endif
#endif

//
// Assert_xxx
//
#if (DIAG_MODE == DIAG_MODE_DISABLED) || (DIAG_MODE == DIAG_MODE_SILENT) || (DIAG_MODE == DIAG_MODE_STEADY)
//
// Asserts are disabled
//
#define Assert_NotNullPtr(ptr)      DIAG_NOOP
#define Assert_Check(check)         DIAG_NOOP
#define Assert_CheckMsg(check, msg) DIAG_NOOP
#define Assert_Failure()            DIAG_NOOP
#define Assert_FailureMsg(msg)      DIAG_NOOP
#elif (DIAG_MODE == DIAG_MODE_NOISY) || (DIAG_MODE == DIAG_MODE_INTRUSIVE)
//
// Asserts are enabled
//
#define Assert_NotNullPtr(ptr)      if((ptr)==DIAG_NULL) DIAG_HALT(DIAG_ID_ASSERT_NULL_PTR, #ptr      DIAG_SRC_LOCATION)
#define Assert_Check(check)         if(!(check))         DIAG_HALT(DIAG_ID_ASSERT_CHECK   , #check    DIAG_SRC_LOCATION)
#define Assert_CheckMsg(check, msg) if(!(check))         DIAG_HALT(DIAG_ID_ASSERT_COND_MSG, msg       DIAG_SRC_LOCATION)
#define Assert_Failure()            if(diag_true())      DIAG_HALT(DIAG_ID_ASSERT_FAILURE , DIAG_NULL DIAG_SRC_LOCATION)
#define Assert_FailureMsg(msg)      if(diag_true())      DIAG_HALT(DIAG_ID_ASSERT_FAIL_MSG, msg       DIAG_SRC_LOCATION)
#else
//
// Asserts are user-defined
//
#endif

#define DIAG_ARG_STMT errno = EINVAL
#define DIAG_ARG_COND ((DIAG_ARG_STMT) != 0)
#define DIAG_ARG_WARN DIAG_ARG_COND && DIAG_WARN

#define DIAG_ERR_STMT errno = EFAULT
#define DIAG_ERR_COND ((DIAG_ERR_STMT) != 0)
#define DIAG_ERR_WARN DIAG_ERR_COND && DIAG_WARN

//
// Check_xxx
//
#if (DIAG_MODE == DIAG_MODE_DISABLED)
//
// Checks are disabled
//
#define Check_NotNullPtr(ptr,    ...)   DIAG_NOOP
#define Check_ValidState(check,  ...)   DIAG_NOOP
#define Check_FailureMsg(msg,    ...)   return __VA_ARGS__
#define Check_Failure(           ...)   return __VA_ARGS__
#define Check_Recovery(msg          )   DIAG_NOOP

#define Check_Arg_IsValid(check, ...)   DIAG_NOOP
#define Check_Arg_NotNull(ptr,   ...)   DIAG_NOOP
#define Check_Arg_Invalid(arg,   ...)   return __VA_ARGS__

#define Check_Arg_Array(ndx, cnt, check, ...)

#elif (DIAG_MODE == DIAG_MODE_SILENT)
//
// Checks are silent
//
#define Check_NotNullPtr(ptr,    ...)   if(((ptr)==DIAG_NULL) && DIAG_ERR_COND) return __VA_ARGS__
#define Check_ValidState(check,  ...)   if((!(check))         && DIAG_ERR_COND) return __VA_ARGS__
#define Check_FailureMsg(msg,    ...)                            DIAG_ERR_STMT; return __VA_ARGS__
#define Check_Failure(           ...)                            DIAG_ERR_STMT; return __VA_ARGS__
#define Check_Recovery(msg          )   DIAG_NOOP

#define Check_Arg_IsValid(check, ...)   if(!(check)           && DIAG_ARG_COND) return __VA_ARGS__
#define Check_Arg_NotNull(ptr,   ...)   if(((ptr)==DIAG_NULL) && DIAG_ARG_COND) return __VA_ARGS__
#define Check_Arg_Invalid(arg,   ...)                            DIAG_ARG_STMT; return __VA_ARGS__

#ifdef __cplusplus
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check)    && DIAG_ARG_COND) return __VA_ARGS__
#else
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(int ndx = 0; ndx < (int)(cnt); ++ndx)      if(!(check)    && DIAG_ARG_COND) return __VA_ARGS__
#endif

#elif (DIAG_MODE == DIAG_MODE_STEADY) || (DIAG_MODE == DIAG_MODE_NOISY)
//
// Checks makes warns fails
//
#define Check_NotNullPtr(ptr,    ...)   if(((ptr)==DIAG_NULL) && DIAG_ERR_WARN(DIAG_ID_CHECK_NULL_PTR , #ptr      DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_ValidState(check,  ...)   if(!(check)           && DIAG_ERR_WARN(DIAG_ID_CHECK_STATE    , #check    DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_FailureMsg(msg,    ...)                            DIAG_ERR_WARN(DIAG_ID_CHECK_FAIL_MSG , msg       DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Failure(           ...)                            DIAG_ERR_WARN(DIAG_ID_CHECK_FAILURE  , DIAG_NULL DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Recovery(msg          )                         if(DIAG_ERR_WARN(DIAG_ID_CHECK_RECOVERY , msg       DIAG_SRC_LOCATION)) DIAG_NOOP

#define Check_Arg_IsValid(check, ...)   if(!(check)           && DIAG_ARG_WARN(DIAG_ID_ARG_NOT_VALID  , #check    DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Arg_NotNull(ptr,   ...)   if(((ptr)==DIAG_NULL) && DIAG_ARG_WARN(DIAG_ID_ARG_IS_NULL    , #ptr      DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Arg_Invalid(arg,   ...)                            DIAG_ARG_WARN(DIAG_ID_ARG_FAILURE    , #arg      DIAG_SRC_LOCATION); return __VA_ARGS__

#ifdef __cplusplus
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check)    && DIAG_ARG_WARN(DIAG_ID_ARG_BAD_ARRAY, #check      DIAG_SRC_LOCATION)) return __VA_ARGS__
#else
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(int ndx = 0; ndx < (int)(cnt); ++ndx)      if(!(check)    && DIAG_ARG_WARN(DIAG_ID_ARG_BAD_ARRAY, #check      DIAG_SRC_LOCATION)) return __VA_ARGS__
#endif

#elif (DIAG_MODE == DIAG_MODE_INTRUSIVE)
//
// Checks halts execution
//
#define Check_NotNullPtr(ptr,    ...)   if((ptr)==DIAG_NULL) DIAG_HALT(DIAG_ID_CHECK_NULL_PTR , #ptr      DIAG_SRC_LOCATION)
#define Check_ValidState(check,  ...)   if(!(check))         DIAG_HALT(DIAG_ID_CHECK_STATE    , #check    DIAG_SRC_LOCATION)
#define Check_FailureMsg(msg,    ...)   if(diag_true())      DIAG_HALT(DIAG_ID_CHECK_FAIL_MSG , msg       DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Failure(           ...)   if(diag_true())      DIAG_HALT(DIAG_ID_CHECK_FAILURE  , DIAG_NULL DIAG_SRC_LOCATION); return __VA_ARGS__
#define Check_Recovery(msg          )   if(diag_true())      DIAG_HALT(DIAG_ID_CHECK_RECOVERY , msg       DIAG_SRC_LOCATION)

#define Check_Arg_IsValid(check, ...)   if(!(check))         DIAG_HALT(DIAG_ID_ARG_NOT_VALID  , #check    DIAG_SRC_LOCATION)
#define Check_Arg_NotNull(ptr,   ...)   if((ptr)==DIAG_NULL) DIAG_HALT(DIAG_ID_ARG_IS_NULL    , #ptr      DIAG_SRC_LOCATION)
#define Check_Arg_Invalid(arg,   ...)   if(diag_true())      DIAG_HALT(DIAG_ID_ARG_FAILURE    , #arg      DIAG_SRC_LOCATION); return __VA_ARGS__

#ifdef __cplusplus
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check))  DIAG_HALT(DIAG_ID_ARG_BAD_ARRAY, #check      DIAG_SRC_LOCATION)
#else
#define Check_Arg_Array(ndx, cnt, check, ...) \
for(int ndx = 0; ndx < (int)(cnt); ++ndx)      if(!(check))  DIAG_HALT(DIAG_ID_ARG_BAD_ARRAY, #check      DIAG_SRC_LOCATION)
#endif

#else
//
// Checks are user-defined
//
#endif
