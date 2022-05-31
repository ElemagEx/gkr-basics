#pragma once

#define DIAGS_MODE_DISABLED   0 /* Disables all diagnostics */
#define DIAGS_MODE_DEFAULT    1 /* Asserts are enabled and other diagnostics are silient */
#define DIAGS_MODE_STEADY     2 /* Asserts are disabled and other diagnostics only makes notes - default for RELASE builds */
#define DIAGS_MODE_INTRUSIVE  3 /* All diagnostics stops execution - default for DEBUG builds */

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

#if   (DIAGS_MODE == DIAGS_MODE_DISABLED)

#define Assert_CheckMsg(check, msg)

#define Assert_Check(check)
#define Assert_NotNullPtr(ptr)
#define Assert_Failure()
#define Assert_NotImplemented()

#define Verify_BoolRes(expr, ...)       if(!(expr )) return __VA_ARGS__

#define Check_ValidState(check, ...)
#define Check_NotNullPtr(ptr,   ...)
#define Check_Failure(          ...)                 return __VA_ARGS__

#define Check_ValidArg(check, ...)
#define Check_NotNullArg(ptr, ...)
#define Check_InvalidArg(arg, ...)

#define Check_ValidArrayArg(ndx, cnt, check, ...)

#elif (DIAGS_MODE == DIAGS_MODE_DEFAULT)

#define Assert_CheckMsg(check, msg)     if(!(check)) HALT(msg    DIAG_SRC_LOCATION)

#define Assert_Check(check)             if(!(check)) HALT(#check DIAG_SRC_LOCATION)
#define Assert_NotNullPtr(ptr)          if(!(ptr  )) HALT(#ptr   DIAG_SRC_LOCATION)
#define Assert_Failure()                             HALT("Fail" DIAG_SRC_LOCATION)
#define Assert_NotImplemented()                      HALT("Impl" DIAG_SRC_LOCATION)

#define Verify_BoolRes(expr, ...)       if(!(expr )) return __VA_ARGS__

#define Check_ValidState(check, ...)    if(!(check)) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if(!(ptr  )) return __VA_ARGS__
#define Check_Failure(          ...)                 return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if(!(ptr  )) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                   return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check)) return __VA_ARGS__

#elif (DIAGS_MODE == DIAGS_MODE_STEADY)

#define Assert_CheckMsg(check, msg)

#define Assert_Check(check)
#define Assert_NotNullPtr(ptr)
#define Assert_Failure()
#define Assert_NotImplemented()

#define Verify_BoolRes(expr, ...)       if(!(expr ) && (NOTE(#expr  DIAG_SRC_LOCATION), true)) return __VA_ARGS__

#define Check_ValidState(check, ...)    if(!(check) && (NOTE(#check DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if(!(ptr  ) && (NOTE(#ptr   DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_Failure(          ...)                    NOTE("Fail" DIAG_SRC_LOCATION);        return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check) && (NOTE(#check DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if(!(ptr  ) && (NOTE(#ptr   DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                      NOTE(#arg   DIAG_SRC_LOCATION);        return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && (NOTE(#check DIAG_SRC_LOCATION), true)) return __VA_ARGS__

#elif (DIAGS_MODE == DIAGS_MODE_INTRUSIVE)

#define Assert_CheckMsg(check, msg)     if(!(check)) HALT(msg    DIAG_SRC_LOCATION)

#define Assert_Check(check)             if(!(check)) HALT(#check DIAG_SRC_LOCATION)
#define Assert_NotNullPtr(ptr)          if(!(ptr  )) HALT(#ptr   DIAG_SRC_LOCATION)
#define Assert_Failure()                             HALT("Fail" DIAG_SRC_LOCATION)
#define Assert_NotImplemented()                      HALT("Impl" DIAG_SRC_LOCATION)

#define Verify_BoolRes(expr, ...)       if(!(expr ) && (STOP(#expr  DIAG_SRC_LOCATION), true)) return __VA_ARGS__

#define Check_ValidState(check, ...)    if(!(check) && (STOP(#check DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_NotNullPtr(ptr,   ...)    if(!(ptr  ) && (STOP(#ptr   DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_Failure(          ...)                    STOP("Fail" DIAG_SRC_LOCATION);        return __VA_ARGS__

#define Check_ValidArg(check, ...)      if(!(check) && (STOP(#check DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_NotNullArg(ptr, ...)      if(!(ptr  ) && (STOP(#ptr   DIAG_SRC_LOCATION), true)) return __VA_ARGS__
#define Check_InvalidArg(arg, ...)                      STOP(#arg   DIAG_SRC_LOCATION);        return __VA_ARGS__

#define Check_ValidArrayArg(ndx, cnt, check, ...) \
for(decltype(cnt) ndx = 0; ndx < (cnt); ++ndx) if(!(check) && (STOP(#check DIAG_SRC_LOCATION), true)) return __VA_ARGS__

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
inline void HALT(const char* msg, const char* file, int line) { DebugBreak(); }
inline void STOP(const char* msg, const char* file, int line) { DebugBreak(); }
inline void NOTE(const char* msg, const char* file, int line) {}
#endif
