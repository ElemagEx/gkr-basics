#pragma once

#define DIAGS_MODE_DISABLED   0 /* Disables all diagnostics */
#define DIAGS_MODE_DEFAULT    1 /* Asserts are enabled and other diagnostics are silient */
#define DIAGS_MODE_STEADY     2 /* Asserts are disabled and other diagnostics only makes notes - default for RELASE builds */
#define DIAGS_MODE_INTRUSIVE  3 /* All diagnostics stops execution - default for DEBUG builds */

#define DIAG_NOEXCEPT true

#ifndef DIAGS_MODE
#if defined(_DEBUG)
#define DIAGS_MODE DIAGS_MODE_INTRUSIVE
#elif defined(NDEBUG)
#define DIAGS_MODE DIAGS_MODE_STEADY
#else
#define DIAGS_MODE DIAGS_MODE_DEFAULT
#endif
#endif

#ifndef DIAG_SRC_LOCATION
#define DIAG_SRC_LOCATION , __FILE__, __LINE__
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

#if   (DIAGS_MODE == DIAGS_MODE_DISABLED)

#define Assert_CheckMsg(check, msg)

#define Assert_Check(check)
#define Assert_NotNullPtr(ptr)
#define Assert_Failure()
#define Assert_NotImplemented()

#define Verify_BoolRes(expr, ...)       if(!(expr)) return __VA_ARGS__

#define Check_ValidState(check, ...)
#define Check_NotNullPtr(ptr,   ...)
#define Check_Failure(          ...)                return __VA_ARGS__

#define Check_ValidArg(check, ...)
#define Check_NotNullArg(ptr, ...)
#define Check_InvalidArg(arg, ...)

#define Check_ValidArrayArg(ndx, cnt, check, ...)

#elif (DIAGS_MODE == DIAGS_MODE_DEFAULT)

#define Assert_CheckMsg(check, msg)     if(!(check)      ) HALT(DIAG_ID_ASSERT_MSG     , msg     DIAG_SRC_LOCATION)

#define Assert_Check(check)             if(!(check)      ) HALT(DIAG_ID_ASSERT_CHECK   , #check  DIAG_SRC_LOCATION)
#define Assert_NotNullPtr(ptr)          if((ptr)==nullptr) HALT(DIAG_ID_ASSERT_NULL_PTR, #ptr    DIAG_SRC_LOCATION)
#define Assert_Failure()                                   HALT(DIAG_ID_FATAL_FAILURE  , nullptr DIAG_SRC_LOCATION)
#define Assert_NotImplemented()                            HALT(DIAG_ID_NOT_IMPLEMENTED, nullptr DIAG_SRC_LOCATION)

#define Verify_BoolRes(expr, ...)       if(!(expr)       ) return __VA_ARGS__

#define Check_ValidState(check, ...)    if(!(check)      ) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if((ptr)==nullptr) return __VA_ARGS__
#define Check_Failure(          ...)                       return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)      ) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if((ptr)==nullptr) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                         return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check)) return __VA_ARGS__

#elif (DIAGS_MODE == DIAGS_MODE_STEADY)

#define Assert_CheckMsg(check, msg)

#define Assert_Check(check)
#define Assert_NotNullPtr(ptr)
#define Assert_Failure()
#define Assert_NotImplemented()

#define Verify_BoolRes(expr, ...)       if(!(expr )       && (NOTE(DIAG_ID_VERIFY_BOOL  , #expr   DIAG_SRC_LOCATION), true)) return __VA_ARGS__

#define Check_ValidState(check, ...)    if(!(check)       && (NOTE(DIAG_ID_VALID_STATE  , #check  DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if((ptr)==nullptr && (NOTE(DIAG_ID_NOT_NULL_PTR , #ptr    DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_Failure(          ...)                          NOTE(DIAG_ID_STATE_FAILURE, nullptr DIAG_SRC_LOCATION);        return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)       && (NOTE(DIAG_ID_NOT_VALID_ARG, #check  DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if((ptr)==nullptr && (NOTE(DIAG_ID_NOT_NULL_ARG , #ptr    DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                            NOTE(DIAG_ID_INVALID_ARG  , #arg    DIAG_SRC_LOCATION);        return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && (NOTE(DIAG_ID_BAD_ARRAY_ARG, #check DIAG_SRC_LOCATION), true)) return __VA_ARGS__

#elif (DIAGS_MODE == DIAGS_MODE_INTRUSIVE)

#define Assert_CheckMsg(check, msg)     if(!(check))          HALT(DIAG_ID_ASSERT_MSG     , msg     DIAG_SRC_LOCATION)

#define Assert_Check(check)             if(!(check))          HALT(DIAG_ID_ASSERT_CHECK   , #check  DIAG_SRC_LOCATION)
#define Assert_NotNullPtr(ptr)          if((ptr)==nullptr)    HALT(DIAG_ID_ASSERT_NULL_PTR, #ptr    DIAG_SRC_LOCATION)
#define Assert_Failure()                                      HALT(DIAG_ID_FATAL_FAILURE  , nullptr DIAG_SRC_LOCATION)
#define Assert_NotImplemented()                               HALT(DIAG_ID_NOT_IMPLEMENTED, nullptr DIAG_SRC_LOCATION)

#define Verify_BoolRes(expr, ...)       if(!(expr )       && (STOP(DIAG_ID_VERIFY_BOOL  , #expr   DIAG_SRC_LOCATION), true)) return __VA_ARGS__

#define Check_ValidState(check, ...)    if(!(check)       && (STOP(DIAG_ID_VALID_STATE  , #check  DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if((ptr)==nullptr && (STOP(DIAG_ID_NOT_NULL_PTR , #ptr    DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_Failure(          ...)                          STOP(DIAG_ID_STATE_FAILURE, nullptr DIAG_SRC_LOCATION);        return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)       && (STOP(DIAG_ID_NOT_VALID_ARG, #check  DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if((ptr)==nullptr && (STOP(DIAG_ID_NOT_NULL_ARG , #ptr    DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                            STOP(DIAG_ID_INVALID_ARG  , #arg    DIAG_SRC_LOCATION);        return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && (STOP(DIAG_ID_BAD_ARRAY_ARG, #check DIAG_SRC_LOCATION), true)) return __VA_ARGS__

#else
// Custom DIAG mode - diagnostics macros are defined somewhere else
#endif

#ifndef DIAGS_EXTERNAL_API
#ifdef _WIN32
extern "C" __declspec(dllimport) void __stdcall DebugBreak();
#else
inline void DebugBreak() { __builtin_trap(); }
#endif
[[noreturn]]
inline void HALT(int id, const char* msg, const char* file, int line) { DebugBreak(); }
inline void STOP(int id, const char* msg, const char* file, int line) { DebugBreak(); }
inline void NOTE(int id, const char* msg, const char* file, int line) {}
#endif
