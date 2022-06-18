#pragma once

#define DIAG_MODE_DISABLED  0 /* Disables all diagnostics */
#define DIAG_MODE_DEFAULT   1 /* Asserts are disabled and other diagnostics are silient - default for RELASE builds */
#define DIAG_MODE_STEADY    2 /* Asserts are disabled and other diagnostics makes notes*/
#define DIAG_MODE_NOISY     3 /* Asserts are enabled and other diagnostics makes notes - default for DEBUG builds */
#define DIAG_MODE_INTRUSIVE 4 /* All diagnostics stops execution */

#ifndef DIAG_MODE
#if defined(_DEBUG)
#define DIAG_MODE DIAG_MODE_NOISY
#elif defined(NDEBUG)
#define DIAG_MODE DIAG_MODE_DEFAULT
#else
#define DIAG_MODE DIAG_MODE_DISABLED
#endif
#endif

#ifndef DIAG_SRC_LOCATION
#undef  DIAG_SRC_PROTOTYPE
#if   defined(__cpp_lib_stacktrace)
#define  DIAG_SRC_LOCATION  ,       std::basic_stacktrace::current()
#define  DIAG_SRC_PROTOTYPE , const std::basic_stacktrace&
#elif defined(__cpp_lib_source_location)
#define  DIAG_SRC_LOCATION  ,       std::source_location::current()
#define  DIAG_SRC_PROTOTYPE , const std::source_location&
#else
#define  DIAG_SRC_LOCATION  , __FILE__, __LINE__
#define  DIAG_SRC_PROTOTYPE , const char*, int
#endif
#endif

#define DIAG_ID_ASSERT_MSG      0
#define DIAG_ID_ASSERT_CHECK    1
#define DIAG_ID_ASSERT_NULL_PTR 2
#define DIAG_ID_FATAL_FAILURE   3
#define DIAG_ID_NOT_IMPLEMENTED 4
#define DIAG_ID_VERIFY_BOOL     5
#define DIAG_ID_VALID_STATE     6
#define DIAG_ID_NOT_NULL_PTR    7
#define DIAG_ID_STATE_FAILURE   8
#define DIAG_ID_NOT_VALID_ARG   9
#define DIAG_ID_NOT_NULL_ARG    10
#define DIAG_ID_INVALID_ARG     11
#define DIAG_ID_BAD_ARRAY_ARG   12

#if (DIAG_MODE <= DIAG_MODE_STEADY)
//
// Asserts are disabled
//
#define Assert_CheckMsg(check, msg)
#define Assert_Check(check)
#define Assert_NotNullPtr(ptr)
#define Assert_Failure()
#define Assert_NotImplemented()

#else
//
// Asserts are enabled
//
#define Assert_CheckMsg(check, msg) if(!(check)      )  HALT(DIAG_ID_ASSERT_MSG     , msg     DIAG_SRC_LOCATION)
#define Assert_Check(check)         if(!(check)      )  HALT(DIAG_ID_ASSERT_CHECK   , #check  DIAG_SRC_LOCATION)
#define Assert_NotNullPtr(ptr)      if((ptr)==nullptr)  HALT(DIAG_ID_ASSERT_NULL_PTR, #ptr    DIAG_SRC_LOCATION)
#define Assert_Failure()                                HALT(DIAG_ID_FATAL_FAILURE  , nullptr DIAG_SRC_LOCATION)
#define Assert_NotImplemented()                         HALT(DIAG_ID_NOT_IMPLEMENTED, nullptr DIAG_SRC_LOCATION)

#endif

#if (DIAG_MODE == DIAG_MODE_DISABLED)
//
// Checks are disabled
//
#define Verify_BoolRes(expr)            expr

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
#define Verify_BoolRes(expr)            expr

#define Check_ValidState(check, ...)    if(!(check)      ) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if((ptr)==nullptr) return __VA_ARGS__
#define Check_Failure(          ...)                       return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)      ) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if((ptr)==nullptr) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                         return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check)) return __VA_ARGS__

#elif (DIAG_MODE == DIAG_MODE_STEADY) || (DIAG_MODE == DIAG_MODE_NOISY)
//
// Checks makes notes
//
#define Verify_BoolRes(expr)            ((expr) || !NOTE(DIAG_ID_VERIFY_BOOL, #expr DIAG_SRC_LOCATION))

#define Check_ValidState(check, ...)    if(!(check)       && NOTE(DIAG_ID_VALID_STATE  , #check  DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if((ptr)==nullptr && NOTE(DIAG_ID_NOT_NULL_PTR , #ptr    DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Failure(          ...)                         NOTE(DIAG_ID_STATE_FAILURE, nullptr DIAG_SRC_LOCATION); return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)       && NOTE(DIAG_ID_NOT_VALID_ARG, #check  DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if((ptr)==nullptr && NOTE(DIAG_ID_NOT_NULL_ARG , #ptr    DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                           NOTE(DIAG_ID_INVALID_ARG  , #arg    DIAG_SRC_LOCATION); return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && NOTE(DIAG_ID_BAD_ARRAY_ARG, #check DIAG_SRC_LOCATION)) return __VA_ARGS__

#elif (DIAG_MODE == DIAG_MODE_INTRUSIVE)
//
// Checks stops execution
//
#define Verify_BoolRes(expr, ...)       ((expr) || !STOP(DIAG_ID_VERIFY_BOOL, #expr DIAG_SRC_LOCATION))

#define Check_ValidState(check, ...)    if(!(check)       && STOP(DIAG_ID_VALID_STATE  , #check  DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if((ptr)==nullptr && STOP(DIAG_ID_NOT_NULL_PTR , #ptr    DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_Failure(          ...)                         STOP(DIAG_ID_STATE_FAILURE, nullptr DIAG_SRC_LOCATION); return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)       && STOP(DIAG_ID_NOT_VALID_ARG, #check  DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if((ptr)==nullptr && STOP(DIAG_ID_NOT_NULL_ARG , #ptr    DIAG_SRC_LOCATION)) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                           STOP(DIAG_ID_INVALID_ARG  , #arg    DIAG_SRC_LOCATION); return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && STOP(DIAG_ID_BAD_ARRAY_ARG, #check DIAG_SRC_LOCATION)) return __VA_ARGS__

#else
#error Unknown diag mode
#endif

#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif

#ifndef DIAGS_EXTERNAL_API
[[noreturn]]
inline void HALT(int, const char* DIAG_SRC_PROTOTYPE)
{
#if defined(__clang__) || defined(__GNUC__)
    __builtin_trap();
#elif defined(_MSC_VER)
    __debugbreak();
#else
    *(int*)nullptr = 0;
#endif
}
#if defined(__clang__)
[[noreturn]]
#endif
inline bool STOP(int, const char* DIAG_SRC_PROTOTYPE)
{
#if defined(__clang__) || defined(__GNUC__)
    __builtin_trap();
#elif defined(_MSC_VER)
    __debugbreak();
#else
    *(int*)nullptr = 0;
#endif
}
inline bool NOTE(int, const char* DIAG_SRC_PROTOTYPE)
{
    return true;
}
#endif
