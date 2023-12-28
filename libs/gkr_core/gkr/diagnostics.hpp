#ifndef DIAG_UNIT_TESTING
#pragma once
#endif

#ifndef __cplusplus
#error  For C you must include diagnostics.h instead of this header
#endif

#include <gkr/diagnostics.h>

#ifndef DIAG_ONE_TIME_CPP_DEFS
#define DIAG_ONE_TIME_CPP_DEFS
//
// no source arguments
//
[[noreturn]]
inline void diag_cpp_halt(int, const char*) noexcept
{
    diag_trap();
}
inline int diag_cpp_warn(int, const char*) noexcept
{
    return 1;
}
//
// preprocessor source arguments
//
[[noreturn]]
inline void diag_cpp_halt(int, const char*, const char*, int) noexcept
{
    diag_trap();
}
inline int diag_cpp_warn(int, const char*, const char*, int) noexcept
{
    return 1;
}
//
// preprocessor ex source arguments
//
[[noreturn]]
inline void diag_cpp_halt(int, const char*, const char*, const char*, int) noexcept
{
    diag_trap();
}
inline int diag_cpp_warn(int, const char*, const char*, const char*, int) noexcept
{
    return 1;
}
#ifdef __cpp_lib_source_location
#include <source_location>
//
// std::source_location source arguments
//
[[noreturn]]
inline void diag_cpp_halt(int, const char*, const std::source_location&) noexcept
{
    diag_trap();
}
inline int diag_cpp_warn(int, const char*, const std::source_location&) noexcept
{
    return 1;
}
#endif /* __cpp_lib_source_location */
#ifdef __cpp_lib_stacktrace
#include <stacktrace>
//
// std::stacktrace source arguments
//
[[noreturn]]
inline void diag_cpp_halt(int, const char*, const std::stacktrace&) noexcept
{
    diag_trap();
}
inline int diag_cpp_warn(int, const char*, const std::stacktrace&) noexcept
{
    return 1;
}
#endif /* __cpp_lib_stacktrace */
#endif /* ndef DIAG_ONE_CPP_TIME_DEFS */

//
// Set appropriate DIAG_SRC_LOCATION and DIAG_SRC_PROTOTYPE used for DIAG_xxx functions
//
#if (DIAG_SRC_INFO == DIAG_SRC_INFO_SOURCE_LOCATION)
#if !defined(__cpp_lib_source_location)
#error You must use at least C++20 with source location support
#else
#include <source_location>
#define DIAG_SRC_LOCATION  ,       std::source_location::current()
#define DIAG_SRC_PROTOTYPE , const std::source_location& location
#endif
#elif (DIAG_SRC_INFO == DIAG_SRC_INFO_STACKTRACE)
#if !defined(__cpp_lib_stacktrace)
#error You must use at least C++23 with stack trace support
#else
#include <stacktrace>
#define DIAG_SRC_LOCATION  ,       std::stacktrace::current()
#define DIAG_SRC_PROTOTYPE , const std::stacktrace& stacktrace
#endif
#else
//
// DIAG_SRC_LOCATION and DIAG_SRC_PROTOTYPE are defined elsewhere
//
#endif

//
// Define DIAG_NOEXCEPT if your own implementation of diagnostics APIs can throws exceptions
//
#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif
//
// define DIAG_EXTERNAL_API and/or DIAG_HALT/DIAG_WARN if you want to implement your own versions of halt/warn functions
//
#ifndef DIAG_EXTERNAL_API
#define DIAG_HALT diag_cpp_halt
#define DIAG_WARN diag_cpp_warn
#endif
