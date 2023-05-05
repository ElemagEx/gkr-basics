#include <catch2/catch_test_macros.hpp>

#define DIAG_UNIT_TESTING
#include <gkr/diag/diagnostics.h>

#include <exception>

#include <setjmp.h>

#define TEST_ASSERT_CHECK_MESSAGE   "Test Assert Check Message"
#define TEST_ASSERT_FAILURE_MESSAGE "Test Assert Failure Message"
#define TEST_CHECK_FAILURE_MESSAGE  "Test Check Failure Message"
#define TEST_CHECK_RECOVERY_MESSAGE "Test Check Recovery Message"

//
// C++ procs - no args
//
static void my_cpp_halt_proc_no_args(int id, const char* msg) noexcept(false)
{
    throw std::exception();
}
static int my_cpp_stop_proc_no_args(int id, const char* msg) noexcept(false)
{
    throw std::exception();
}
static int my_cpp_warn_proc_no_args(int id, const char* msg) noexcept
{
    return 1;
}
//
// C++ procs - preprocessor args
//
static void my_cpp_halt_proc_pp_args(int id, const char* msg, const char* file, int line) noexcept(false)
{
    throw std::exception();
}
static int my_cpp_stop_proc_pp_args(int id, const char* msg, const char* file, int line) noexcept(false)
{
    throw std::exception();
}
static int my_cpp_warn_proc_pp_args(int id, const char* msg, const char* file, int line) noexcept
{
    return 1;
}
//
// C++ procs - std::source_location args
//
#ifdef __cpp_lib_source_location
#include <source_location>
static void my_cpp_halt_proc_sl_args(int id, const char* msg, const std::source_location& location) noexcept(false)
{
    throw std::exception();
}
static int my_cpp_stop_proc_sl_args(int id, const char* msg, const std::source_location& location) noexcept(false)
{
    throw std::exception();
}
static int my_cpp_warn_proc_sl_args(int id, const char* msg, const std::source_location& location) noexcept
{
    return 1;
}
#endif
//
// C++ procs - std::stacktrace args
//
#ifdef __cpp_lib_stacktrace
#include <stacktrace>
static void my_cpp_halt_proc_st_args(int id, const char* msg, const std::stacktrace& stacktrace) noexcept(false)
{
    throw std::exception();
}
static int my_cpp_stop_proc_st_args(int id, const char* msg, const std::stacktrace& stacktrace) noexcept(false)
{
    throw std::exception();
}
static int my_cpp_warn_proc_st_args(int id, const char* msg, const std::stacktrace& stacktrace) noexcept
{
    return 1;
}
#endif

extern "C" {

thread_local jmp_buf my_jmp_buffer;

//
// C procs - no args
//
static void my_c_halt_proc_no_args(int id, const char* msg)
{
    longjmp(my_jmp_buffer, -1);
}
static int my_c_stop_proc_no_args(int id, const char* msg)
{
    longjmp(my_jmp_buffer, -1);
}
static int my_c_warn_proc_no_args(int id, const char* msg)
{
    return 1;
}
//
// C procs - no args
//
static void my_c_halt_proc_pp_args(int id, const char* msg, const char* file, int line)
{
    longjmp(my_jmp_buffer, -1);
}
static int my_c_stop_proc_pp_args(int id, const char* msg, const char* file, int line)
{
    longjmp(my_jmp_buffer, -1);
}
static int my_c_warn_proc_pp_args(int id, const char* msg, const char* file, int line)
{
    return 1;
}

}

//
// call C function
//
#ifdef _MSC_VER
#pragma warning(disable:4611)
#endif
template<typename Ret>
Ret call(Ret (*proc)(unsigned, const char*), unsigned len, const char* ptr) noexcept(false)
{
    if(setjmp(my_jmp_buffer) != -1)
    {
        return (*proc)(len, ptr);
    }
    throw std::exception();
}
template<typename Ret>
Ret call(Ret (*proc)(const char*), const char* ptr) noexcept(false)
{
    if(setjmp(my_jmp_buffer) != -1)
    {
        return (*proc)(ptr);
    }
    throw std::exception();
}
template<typename Ret>
Ret call(Ret (*proc)(int), int arg) noexcept(false)
{
    if(setjmp(my_jmp_buffer) != -1)
    {
        return (*proc)(arg);
    }
    throw std::exception();
}
template<typename Ret>
Ret call(Ret (*proc)()) noexcept(false)
{
    if(setjmp(my_jmp_buffer) != -1)
    {
        return (*proc)();
    }
    throw std::exception();
}
#ifdef _MSC_VER
#pragma warning(default:4611)
#endif

//
// SECTION: lang=C, mode=disabled, src=undefined
//
extern "C" {
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN
#define DIAG_NOEXCEPT
#define DIAG_SRC_INFO       -1
#define DIAG_MODE           DIAG_MODE_DISABLED
#include <gkr/diag/diagnostics.h>
//
// Disabled Asserts - src=undefined
//
static void c_test_disabled_assert_not_null_ptr(const char* ptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_disabled_assert_check(int arg)
{
    Assert_Check(arg == 1);
}
static void c_test_disabled_assert_check_msg(int arg)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_disabled_assert_failure()
{
    Assert_Failure();
}
static void c_test_disabled_assert_failure_msg()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Disabled Checks - src=undefined
//
static bool c_test_disabled_check_not_null_ptr(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_disabled_check_valid_state(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_disabled_check_failure_msg()
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool c_test_disabled_check_failure()
{
    Check_Failure(false);
}
static bool c_test_disabled_check_recovery()
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Disabled Arguments Checks - src=undefined
//
static bool c_test_disabled_check_arg_not_null_ptr(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_disabled_check_arg_is_valid(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_disabled_check_arg_array(unsigned len, const char* str)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_disabled_check_arg_invalid(int arg)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=disabled, src=undefined
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=disabled, src=undefined")
{
    CHECK_NOTHROW(call(c_test_disabled_assert_not_null_ptr, "abcde"));
    CHECK_NOTHROW(call(c_test_disabled_assert_check, 1));
    CHECK_NOTHROW(call(c_test_disabled_assert_check_msg, 1));

    CHECK_NOTHROW(call(c_test_disabled_assert_not_null_ptr, NULL));
    CHECK_NOTHROW(call(c_test_disabled_assert_check, 0));
    CHECK_NOTHROW(call(c_test_disabled_assert_check_msg, 0));

    CHECK_NOTHROW(call(c_test_disabled_assert_failure));
    CHECK_NOTHROW(call(c_test_disabled_assert_failure_msg));
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=disabled, src=undefined")
{
    CHECK(call(c_test_disabled_check_not_null_ptr, "abcde"));
    CHECK(call(c_test_disabled_check_valid_state, 1));

    CHECK(call(c_test_disabled_check_not_null_ptr, NULL));
    CHECK(call(c_test_disabled_check_valid_state, 0));

    CHECK_FALSE(call(c_test_disabled_check_failure_msg));
    CHECK_FALSE(call(c_test_disabled_check_failure));
    CHECK_FALSE(call(c_test_disabled_check_recovery));
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=disabled, src=undefined")
{
    CHECK(call(c_test_disabled_check_arg_not_null_ptr, "abcde"));
    CHECK(call(c_test_disabled_check_arg_is_valid, 1));
    CHECK(call(c_test_disabled_check_arg_array, 7U, "abc-def"));

    CHECK(call(c_test_disabled_check_arg_not_null_ptr, NULL));
    CHECK(call(c_test_disabled_check_arg_is_valid, 0));
    CHECK(call(c_test_disabled_check_arg_array, 7U, "abc def"));

    CHECK_FALSE(call(c_test_disabled_check_arg_invalid, 0));
}

//
// SECTION: lang=C++, mode=disabled, src=undefined
//
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN
#define DIAG_NOEXCEPT       true
#define DIAG_SRC_INFO       -1
#define DIAG_MODE           DIAG_MODE_DISABLED
#include <gkr/diag/diagnostics.h>
//
// Disabled Asserts - src=undefined
//
static void cpp_test_disabled_assert_not_null_ptr(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_disabled_assert_check(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_disabled_assert_check_msg(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_disabled_assert_failure() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_disabled_assert_failure_msg() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Disabled Checks - src=undefined
//
static bool cpp_test_disabled_check_not_null_ptr(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_disabled_check_valid_state(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_disabled_check_failure_msg() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_disabled_check_failure() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_disabled_check_recovery() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Disabled Arguments Checks - src=undefined
//
static bool cpp_test_disabled_check_arg_not_null_ptr(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_disabled_check_arg_is_valid(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_disabled_check_arg_array(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_disabled_check_arg_invalid(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=disabled, src=undefined
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=disabled, src=undefined")
{
    CHECK_NOTHROW(cpp_test_disabled_assert_not_null_ptr("abcde"));
    CHECK_NOTHROW(cpp_test_disabled_assert_check(1));
    CHECK_NOTHROW(cpp_test_disabled_assert_check_msg(1));

    CHECK_NOTHROW(cpp_test_disabled_assert_not_null_ptr(nullptr));
    CHECK_NOTHROW(cpp_test_disabled_assert_check(0));
    CHECK_NOTHROW(cpp_test_disabled_assert_check_msg(0));

    CHECK_NOTHROW(cpp_test_disabled_assert_failure());
    CHECK_NOTHROW(cpp_test_disabled_assert_failure_msg());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=disabled, src=undefined")
{
    CHECK(cpp_test_disabled_check_not_null_ptr("abcde"));
    CHECK(cpp_test_disabled_check_valid_state(1));

    CHECK(cpp_test_disabled_check_not_null_ptr(nullptr));
    CHECK(cpp_test_disabled_check_valid_state(0));

    CHECK_FALSE(cpp_test_disabled_check_failure_msg());
    CHECK_FALSE(cpp_test_disabled_check_failure());
    CHECK_FALSE(cpp_test_disabled_check_recovery());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=disabled, src=undefined")
{
    CHECK(cpp_test_disabled_check_arg_not_null_ptr("abcde"));
    CHECK(cpp_test_disabled_check_arg_is_valid(1));
    CHECK(cpp_test_disabled_check_arg_array(7U, "abc-def"));

    CHECK(cpp_test_disabled_check_arg_not_null_ptr(nullptr));
    CHECK(cpp_test_disabled_check_arg_is_valid(0));
    CHECK(cpp_test_disabled_check_arg_array(7U, "abc def"));

    CHECK_FALSE(cpp_test_disabled_check_arg_invalid(0));
}

//
// SECTION: lang=C, mode=silent, src=undefined
//
extern "C" {
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN
#define DIAG_NOEXCEPT
#define DIAG_SRC_INFO       -1
#define DIAG_MODE           DIAG_MODE_SILENT
#include <gkr/diag/diagnostics.h>
//
// Silent Asserts - src=undefined
//
static void c_test_silent_assert_not_null_ptr(const char* ptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_silent_assert_check(int arg)
{
    Assert_Check(arg == 1);
}
static void c_test_silent_assert_check_msg(int arg)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_silent_assert_failure()
{
    Assert_Failure();
}
static void c_test_silent_assert_failure_msg()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Silent Checks - src=undefined
//
static bool c_test_silent_check_not_null_ptr(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_silent_check_valid_state(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_silent_check_failure_msg()
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool c_test_silent_check_failure()
{
    Check_Failure(false);
}
static bool c_test_silent_check_recovery()
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Silent Arguments Checks - src=undefined
//
static bool c_test_silent_check_arg_not_null_ptr(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_silent_check_arg_is_valid(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_silent_check_arg_array(unsigned len, const char* str)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_silent_check_arg_invalid(int arg)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=silent, src=undefined
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=silent, src=undefined")
{
    CHECK_NOTHROW(call(c_test_silent_assert_not_null_ptr, "abcde"));
    CHECK_NOTHROW(call(c_test_silent_assert_check, 1));
    CHECK_NOTHROW(call(c_test_silent_assert_check_msg, 1));

    CHECK_NOTHROW(call(c_test_silent_assert_not_null_ptr, NULL));
    CHECK_NOTHROW(call(c_test_silent_assert_check, 0));
    CHECK_NOTHROW(call(c_test_silent_assert_check_msg, 0));

    CHECK_NOTHROW(call(c_test_silent_assert_failure));
    CHECK_NOTHROW(call(c_test_silent_assert_failure_msg));
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=silent, src=undefined")
{
    CHECK(call(c_test_silent_check_not_null_ptr, "abcde"));
    CHECK(call(c_test_silent_check_valid_state, 1));

    CHECK_FALSE(call(c_test_silent_check_not_null_ptr, NULL));
    CHECK_FALSE(call(c_test_silent_check_valid_state, 0));

    CHECK_FALSE(call(c_test_silent_check_failure_msg));
    CHECK_FALSE(call(c_test_silent_check_failure));
    CHECK_FALSE(call(c_test_silent_check_recovery));
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=silent, src=undefined")
{
    CHECK(call(c_test_silent_check_arg_not_null_ptr, "abcde"));
    CHECK(call(c_test_silent_check_arg_is_valid, 1));
    CHECK(call(c_test_silent_check_arg_array, 7U, "abc-def"));

    CHECK_FALSE(call(c_test_silent_check_arg_not_null_ptr, NULL));
    CHECK_FALSE(call(c_test_silent_check_arg_is_valid, 0));
    CHECK_FALSE(call(c_test_silent_check_arg_array, 7U, "abc def"));

    CHECK_FALSE(call(c_test_silent_check_arg_invalid, 0));
}

//
// SECTION: lang=C++, mode=silent, src=undefined
//
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN
#define DIAG_NOEXCEPT       true
#define DIAG_SRC_INFO       -1
#define DIAG_MODE           DIAG_MODE_SILENT
#include <gkr/diag/diagnostics.h>
//
// Silent Asserts - src=undefined
//
static void cpp_test_silent_assert_not_null_ptr(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_silent_assert_check(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_silent_assert_check_msg(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_silent_assert_failure() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_silent_assert_failure_msg() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Silent Checks - src=undefined
//
static bool cpp_test_silent_check_not_null_ptr(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_silent_check_valid_state(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_silent_check_failure_msg() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_silent_check_failure() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_silent_check_recovery() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Silent Arguments Checks - src=undefined
//
static bool cpp_test_silent_check_arg_not_null_ptr(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_silent_check_arg_is_valid(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_silent_check_arg_array(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_silent_check_arg_invalid(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=silent, src=undefined
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=silent, src=undefined")
{
    CHECK_NOTHROW(cpp_test_silent_assert_not_null_ptr("abcde"));
    CHECK_NOTHROW(cpp_test_silent_assert_check(1));
    CHECK_NOTHROW(cpp_test_silent_assert_check_msg(1));

    CHECK_NOTHROW(cpp_test_silent_assert_not_null_ptr(nullptr));
    CHECK_NOTHROW(cpp_test_silent_assert_check(0));
    CHECK_NOTHROW(cpp_test_silent_assert_check_msg(0));

    CHECK_NOTHROW(cpp_test_silent_assert_failure());
    CHECK_NOTHROW(cpp_test_silent_assert_failure_msg());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=silent, src=undefined")
{
    CHECK(cpp_test_silent_check_not_null_ptr("abcde"));
    CHECK(cpp_test_silent_check_valid_state(1));

    CHECK_FALSE(cpp_test_silent_check_not_null_ptr(nullptr));
    CHECK_FALSE(cpp_test_silent_check_valid_state(0));

    CHECK_FALSE(cpp_test_silent_check_failure_msg());
    CHECK_FALSE(cpp_test_silent_check_failure());
    CHECK_FALSE(cpp_test_silent_check_recovery());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=silent, src=undefined")
{
    CHECK(cpp_test_silent_check_arg_not_null_ptr("abcde"));
    CHECK(cpp_test_silent_check_arg_is_valid(1));
    CHECK(cpp_test_silent_check_arg_array(7U, "abc-def"));

    CHECK_FALSE(cpp_test_silent_check_arg_not_null_ptr(nullptr));
    CHECK_FALSE(cpp_test_silent_check_arg_is_valid(0));
    CHECK_FALSE(cpp_test_silent_check_arg_array(7U, "abc def"));

    CHECK_FALSE(cpp_test_silent_check_arg_invalid(0));
}

//
// SECTION: lang=C, mode=steady, src=none
//
extern "C" {
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN           my_c_warn_proc_no_args
#define DIAG_NOEXCEPT
#define DIAG_SRC_INFO       DIAG_SRC_INFO_NONE
#define DIAG_MODE           DIAG_MODE_STEADY
#include <gkr/diag/diagnostics.h>
//
// Steady Asserts - src=none
//
static void c_test_steady_assert_not_null_ptr_no_args(const char* ptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_steady_assert_check_no_args(int arg)
{
    Assert_Check(arg == 1);
}
static void c_test_steady_assert_check_msg_no_args(int arg)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_steady_assert_failure_no_args()
{
    Assert_Failure();
}
static void c_test_steady_assert_failure_msg_no_args()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Steady Checks - src=none
//
static bool c_test_steady_check_not_null_ptr_no_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_steady_check_valid_state_no_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_steady_check_failure_msg_no_args()
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool c_test_steady_check_failure_no_args()
{
    Check_Failure(false);
}
static bool c_test_steady_check_recovery_no_args()
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Steady Arguments Checks - src=none
//
static bool c_test_steady_check_arg_not_null_ptr_no_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_steady_check_arg_is_valid_no_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_steady_check_arg_array_no_args(unsigned len, const char* str)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_steady_check_arg_invalid_no_args(int arg)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=steady, src=none
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=steady, src=none")
{
    CHECK_NOTHROW(call(c_test_steady_assert_not_null_ptr_no_args, "abcde"));
    CHECK_NOTHROW(call(c_test_steady_assert_check_no_args, 1));
    CHECK_NOTHROW(call(c_test_steady_assert_check_msg_no_args, 1));

    CHECK_NOTHROW(call(c_test_steady_assert_not_null_ptr_no_args, NULL));
    CHECK_NOTHROW(call(c_test_steady_assert_check_no_args, 0));
    CHECK_NOTHROW(call(c_test_steady_assert_check_msg_no_args, 0));

    CHECK_NOTHROW(call(c_test_steady_assert_failure_no_args));
    CHECK_NOTHROW(call(c_test_steady_assert_failure_msg_no_args));
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=steady, src=none")
{
    CHECK(call(c_test_steady_check_not_null_ptr_no_args, "abcde"));
    CHECK(call(c_test_steady_check_valid_state_no_args, 1));

    CHECK_FALSE(call(c_test_steady_check_not_null_ptr_no_args, NULL));
    CHECK_FALSE(call(c_test_steady_check_valid_state_no_args, 0));

    CHECK_FALSE(call(c_test_steady_check_failure_msg_no_args));
    CHECK_FALSE(call(c_test_steady_check_failure_no_args));
    CHECK_FALSE(call(c_test_steady_check_recovery_no_args));
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=steady, src=none")
{
    CHECK(call(c_test_steady_check_arg_not_null_ptr_no_args, "abcde"));
    CHECK(call(c_test_steady_check_arg_is_valid_no_args, 1));
    CHECK(call(c_test_steady_check_arg_array_no_args, 7U, "abc-def"));

    CHECK_FALSE(call(c_test_steady_check_arg_not_null_ptr_no_args, NULL));
    CHECK_FALSE(call(c_test_steady_check_arg_is_valid_no_args, 0));
    CHECK_FALSE(call(c_test_steady_check_arg_array_no_args, 7U, "abc def"));

    CHECK_FALSE(call(c_test_steady_check_arg_invalid_no_args, 0));
}

//
// SECTION: lang=C, mode=steady, src=preprocessor
//
extern "C" {
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN           my_c_warn_proc_pp_args
#define DIAG_NOEXCEPT
#define DIAG_SRC_INFO       DIAG_SRC_INFO_PREPROCESSOR
#define DIAG_MODE           DIAG_MODE_STEADY
#include <gkr/diag/diagnostics.h>
//
// Steady Asserts - src=preprocessor
//
static void c_test_steady_assert_not_null_ptr_pp_args(const char* ptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_steady_assert_check_pp_args(int arg)
{
    Assert_Check(arg == 1);
}
static void c_test_steady_assert_check_msg_pp_args(int arg)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_steady_assert_failure_pp_args()
{
    Assert_Failure();
}
static void c_test_steady_assert_failure_msg_pp_args()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Steady Checks - src=preprocessor
//
static bool c_test_steady_check_not_null_ptr_pp_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_steady_check_valid_state_pp_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_steady_check_failure_msg_pp_args()
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool c_test_steady_check_failure_pp_args()
{
    Check_Failure(false);
}
static bool c_test_steady_check_recovery_pp_args()
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Steady Arguments Checks - src=preprocessor
//
static bool c_test_steady_check_arg_not_null_ptr_pp_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_steady_check_arg_is_valid_pp_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_steady_check_arg_array_pp_args(unsigned len, const char* str)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_steady_check_arg_invalid_pp_args(int arg)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=steady, src=preprocessor
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=steady, src=preprocessor")
{
    CHECK_NOTHROW(call(c_test_steady_assert_not_null_ptr_pp_args, "abcde"));
    CHECK_NOTHROW(call(c_test_steady_assert_check_pp_args, 1));
    CHECK_NOTHROW(call(c_test_steady_assert_check_msg_pp_args, 1));

    CHECK_NOTHROW(call(c_test_steady_assert_not_null_ptr_pp_args, NULL));
    CHECK_NOTHROW(call(c_test_steady_assert_check_pp_args, 0));
    CHECK_NOTHROW(call(c_test_steady_assert_check_msg_pp_args, 0));

    CHECK_NOTHROW(call(c_test_steady_assert_failure_pp_args));
    CHECK_NOTHROW(call(c_test_steady_assert_failure_msg_pp_args));
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=steady, src=preprocessor")
{
    CHECK(call(c_test_steady_check_not_null_ptr_pp_args, "abcde"));
    CHECK(call(c_test_steady_check_valid_state_pp_args, 1));

    CHECK_FALSE(call(c_test_steady_check_not_null_ptr_pp_args, NULL));
    CHECK_FALSE(call(c_test_steady_check_valid_state_pp_args, 0));

    CHECK_FALSE(call(c_test_steady_check_failure_msg_pp_args));
    CHECK_FALSE(call(c_test_steady_check_failure_pp_args));
    CHECK_FALSE(call(c_test_steady_check_recovery_pp_args));
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=steady, src=preprocessor")
{
    CHECK(call(c_test_steady_check_arg_not_null_ptr_pp_args, "abcde"));
    CHECK(call(c_test_steady_check_arg_is_valid_pp_args, 1));
    CHECK(call(c_test_steady_check_arg_array_pp_args, 7U, "abc-def"));

    CHECK_FALSE(call(c_test_steady_check_arg_not_null_ptr_pp_args, NULL));
    CHECK_FALSE(call(c_test_steady_check_arg_is_valid_pp_args, 0));
    CHECK_FALSE(call(c_test_steady_check_arg_array_pp_args, 7U, "abc def"));

    CHECK_FALSE(call(c_test_steady_check_arg_invalid_pp_args, 0));
}

//
// SECTION: lang=C++, mode=steady, src=none
//
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN           my_cpp_warn_proc_no_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_NONE
#define DIAG_MODE           DIAG_MODE_STEADY
#include <gkr/diag/diagnostics.h>
//
// Steady Asserts - src=none
//
static void cpp_test_steady_assert_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_steady_assert_check_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_steady_assert_check_msg_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_steady_assert_failure_no_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_steady_assert_failure_msg_no_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Steady Checks - src=none
//
static bool cpp_test_steady_check_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_steady_check_valid_state_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_steady_check_failure_msg_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_steady_check_failure_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_steady_check_recovery_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Steady Arguments Checks - src=none
//
static bool cpp_test_steady_check_arg_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_steady_check_arg_is_valid_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_steady_check_arg_array_no_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_steady_check_arg_invalid_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=steady, src=none
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=steady, src=none")
{
    CHECK_NOTHROW(cpp_test_steady_assert_not_null_ptr_no_args("abcde"));
    CHECK_NOTHROW(cpp_test_steady_assert_check_no_args(1));
    CHECK_NOTHROW(cpp_test_steady_assert_check_msg_no_args(1));

    CHECK_NOTHROW(cpp_test_steady_assert_not_null_ptr_no_args(nullptr));
    CHECK_NOTHROW(cpp_test_steady_assert_check_no_args(1));
    CHECK_NOTHROW(cpp_test_steady_assert_check_msg_no_args(1));

    CHECK_NOTHROW(cpp_test_steady_assert_failure_no_args());
    CHECK_NOTHROW(cpp_test_steady_assert_failure_msg_no_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=steady, src=none")
{
    CHECK(cpp_test_steady_check_not_null_ptr_no_args("abcde"));
    CHECK(cpp_test_steady_check_valid_state_no_args(1));

    CHECK_FALSE(cpp_test_steady_check_not_null_ptr_no_args(nullptr));
    CHECK_FALSE(cpp_test_steady_check_valid_state_no_args(0));

    CHECK_FALSE(cpp_test_steady_check_failure_msg_no_args());
    CHECK_FALSE(cpp_test_steady_check_failure_no_args());
    CHECK_FALSE(cpp_test_steady_check_recovery_no_args());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=steady, src=none")
{
    CHECK(cpp_test_steady_check_arg_not_null_ptr_no_args("abcde"));
    CHECK(cpp_test_steady_check_arg_is_valid_no_args(1));
    CHECK(cpp_test_steady_check_arg_array_no_args(7U, "abc-def"));

    CHECK_FALSE(cpp_test_steady_check_arg_not_null_ptr_no_args(nullptr));
    CHECK_FALSE(cpp_test_steady_check_arg_is_valid_no_args(0));
    CHECK_FALSE(cpp_test_steady_check_arg_array_no_args(7U, "abc def"));

    CHECK_FALSE(cpp_test_steady_check_arg_invalid_no_args(0));
}

//
// SECTION: lang=C++, mode=steady, src=preprocessor
//
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN           my_cpp_warn_proc_pp_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_PREPROCESSOR
#define DIAG_MODE           DIAG_MODE_STEADY
#include <gkr/diag/diagnostics.h>
//
// Steady Asserts - src=preprocessor
//
static void cpp_test_steady_assert_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_steady_assert_check_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_steady_assert_check_msg_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_steady_assert_failure_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_steady_assert_failure_msg_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Steady Checks - src=preprocessor
//
static bool cpp_test_steady_check_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_steady_check_valid_state_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_steady_check_failure_msg_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_steady_check_failure_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_steady_check_recovery_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Steady Arguments Checks - src=preprocessor
//
static bool cpp_test_steady_check_arg_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_steady_check_arg_is_valid_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_steady_check_arg_array_pp_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_steady_check_arg_invalid_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=steady, src=preprocessor
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=steady, src=preprocessor")
{
    CHECK_NOTHROW(cpp_test_steady_assert_not_null_ptr_pp_args("abcde"));
    CHECK_NOTHROW(cpp_test_steady_assert_check_pp_args(1));
    CHECK_NOTHROW(cpp_test_steady_assert_check_msg_pp_args(1));

    CHECK_NOTHROW(cpp_test_steady_assert_not_null_ptr_pp_args(nullptr));
    CHECK_NOTHROW(cpp_test_steady_assert_check_pp_args(0));
    CHECK_NOTHROW(cpp_test_steady_assert_check_msg_pp_args(0));

    CHECK_NOTHROW(cpp_test_steady_assert_failure_pp_args());
    CHECK_NOTHROW(cpp_test_steady_assert_failure_msg_pp_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=steady, src=preprocessor")
{
    CHECK(cpp_test_steady_check_not_null_ptr_pp_args("abcde"));
    CHECK(cpp_test_steady_check_valid_state_pp_args(1));

    CHECK_FALSE(cpp_test_steady_check_not_null_ptr_pp_args(nullptr));
    CHECK_FALSE(cpp_test_steady_check_valid_state_pp_args(0));

    CHECK_FALSE(cpp_test_steady_check_failure_msg_pp_args());
    CHECK_FALSE(cpp_test_steady_check_failure_pp_args());
    CHECK_FALSE(cpp_test_steady_check_recovery_pp_args());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=steady, src=preprocessor")
{
    CHECK(cpp_test_steady_check_arg_not_null_ptr_pp_args("abcde"));
    CHECK(cpp_test_steady_check_arg_is_valid_pp_args(1));
    CHECK(cpp_test_steady_check_arg_array_pp_args(7U, "abc-def"));

    CHECK_FALSE(cpp_test_steady_check_arg_not_null_ptr_pp_args(nullptr));
    CHECK_FALSE(cpp_test_steady_check_arg_is_valid_pp_args(0));
    CHECK_FALSE(cpp_test_steady_check_arg_array_pp_args(7U, "abc def"));

    CHECK_FALSE(cpp_test_steady_check_arg_invalid_pp_args(0));
}

//
// SECTION: lang=C++, mode=steady, src=std_source_location
//
#ifdef __cpp_lib_source_location
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN           my_cpp_warn_proc_sl_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_SOURCE_LOCATION
#define DIAG_MODE           DIAG_MODE_STEADY
#include <gkr/diag/diagnostics.h>
//
// Steady Asserts - src=std_source_location
//
static void cpp_test_steady_assert_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_steady_assert_check_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_steady_assert_check_msg_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_steady_assert_failure_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_steady_assert_failure_msg_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Steady Checks - src=std_source_location
//
static bool cpp_test_steady_check_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_steady_check_valid_state_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_steady_check_failure_msg_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_steady_check_failure_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_steady_check_recovery_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Steady Arguments Checks - src=std_source_location
//
static bool cpp_test_steady_check_arg_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_steady_check_arg_is_valid_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_steady_check_arg_array_sl_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_steady_check_arg_invalid_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
#endif

//
// Test cases: lang=C++, mode=steady, src=std_source_location
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=steady, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK_NOTHROW(cpp_test_steady_assert_not_null_ptr_sl_args("abcde"));
    CHECK_NOTHROW(cpp_test_steady_assert_check_sl_args(1));
    CHECK_NOTHROW(cpp_test_steady_assert_check_msg_sl_args(1));

    CHECK_NOTHROW(cpp_test_steady_assert_not_null_ptr_sl_args(nullptr));
    CHECK_NOTHROW(cpp_test_steady_assert_check_sl_args(0));
    CHECK_NOTHROW(cpp_test_steady_assert_check_msg_sl_args(0));

    CHECK_NOTHROW(cpp_test_steady_assert_failure_sl_args());
    CHECK_NOTHROW(cpp_test_steady_assert_failure_msg_sl_args());
#endif
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=steady, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK(cpp_test_steady_check_not_null_ptr_sl_args("abcde"));
    CHECK(cpp_test_steady_check_valid_state_sl_args(1));

    CHECK_FALSE(cpp_test_steady_check_not_null_ptr_sl_args(nullptr));
    CHECK_FALSE(cpp_test_steady_check_valid_state_sl_args(0));

    CHECK_FALSE(cpp_test_steady_check_failure_msg_sl_args());
    CHECK_FALSE(cpp_test_steady_check_failure_sl_args());
    CHECK_FALSE(cpp_test_steady_check_recovery_sl_args());
#endif
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=steady, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK(cpp_test_steady_check_arg_not_null_ptr_sl_args("abcde"));
    CHECK(cpp_test_steady_check_arg_is_valid_sl_args(1));
    CHECK(cpp_test_steady_check_arg_array_sl_args(7U, "abc-def"));

    CHECK_FALSE(cpp_test_steady_check_arg_not_null_ptr_sl_args(nullptr));
    CHECK_FALSE(cpp_test_steady_check_arg_is_valid_sl_args(0));
    CHECK_FALSE(cpp_test_steady_check_arg_array_sl_args(7U, "abc def"));

    CHECK_FALSE(cpp_test_steady_check_arg_invalid_sl_args(0));
#endif
}

//
// SECTION: lang=C++, mode=steady, src=std_stacktrace
//
#ifdef __cpp_lib_stacktrace
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT
#define DIAG_STOP
#define DIAG_WARN           my_cpp_warn_proc_st_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_STACKTRACE
#define DIAG_MODE           DIAG_MODE_STEADY
#include <gkr/diag/diagnostics.h>
//
// Steady Asserts - src=std_stacktrace
//
static void cpp_test_steady_assert_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_steady_assert_check_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_steady_assert_check_msg_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_steady_assert_failure_st_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_steady_assert_failure_msg_st_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Steady Checks - src=std_stacktrace
//
static bool cpp_test_steady_check_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_steady_check_valid_state_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_steady_check_failure_msg_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_steady_check_failure_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_steady_check_recovery_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Steady Arguments Checks - src=std_stacktrace
//
static bool cpp_test_steady_check_arg_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_steady_check_arg_is_valid_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_steady_check_arg_array_st_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_steady_check_arg_invalid_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
#endif
//
// Test cases: lang=C++, mode=steady, src=std_stacktrace
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=steady, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK_NOTHROW(cpp_test_steady_assert_not_null_ptr_st_args("abcde"));
    CHECK_NOTHROW(cpp_test_steady_assert_check_st_args(1));
    CHECK_NOTHROW(cpp_test_steady_assert_check_msg_st_args(1));

    CHECK_NOTHROW(cpp_test_steady_assert_not_null_ptr_st_args(nullptr));
    CHECK_NOTHROW(cpp_test_steady_assert_check_st_args(0));
    CHECK_NOTHROW(cpp_test_steady_assert_check_msg_st_args(0));

    CHECK_NOTHROW(cpp_test_steady_assert_failure_st_args());
    CHECK_NOTHROW(cpp_test_steady_assert_failure_msg_st_args());
#endif
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=steady, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK(cpp_test_steady_check_not_null_ptr_st_args("abcde"));
    CHECK(cpp_test_steady_check_valid_state_st_args(1));

    CHECK_FALSE(cpp_test_steady_check_not_null_ptr_st_args(nullptr));
    CHECK_FALSE(cpp_test_steady_check_valid_state_st_args(0));

    CHECK_FALSE(cpp_test_steady_check_failure_msg_st_args());
    CHECK_FALSE(cpp_test_steady_check_failure_st_args());
    CHECK_FALSE(cpp_test_steady_check_recovery_st_args());
#endif
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=steady, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK(cpp_test_steady_check_arg_not_null_ptr_st_args("abcde"));
    CHECK(cpp_test_steady_check_arg_is_valid_st_args(1));
    CHECK(cpp_test_steady_check_arg_array_st_args(7U, "abc-def"));

    CHECK_FALSE(cpp_test_steady_check_arg_not_null_ptr_st_args(nullptr));
    CHECK_FALSE(cpp_test_steady_check_arg_is_valid_st_args(0));
    CHECK_FALSE(cpp_test_steady_check_arg_array_st_args(7U, "abc def"));

    CHECK_FALSE(cpp_test_steady_check_arg_invalid_st_args(0));
#endif
}

//
// SECTION: lang=C, mode=noisy, src=none
//
extern "C" {
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_c_halt_proc_no_args
#define DIAG_STOP
#define DIAG_WARN           my_c_warn_proc_no_args
#define DIAG_NOEXCEPT
#define DIAG_SRC_INFO       DIAG_SRC_INFO_NONE
#define DIAG_MODE           DIAG_MODE_NOISY
#include <gkr/diag/diagnostics.h>
//
// Noisy Asserts - src=none
//
static void c_test_noisy_assert_not_null_ptr_no_args(const char* ptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_noisy_assert_check_no_args(int arg)
{
    Assert_Check(arg == 1);
}
static void c_test_noisy_assert_check_msg_no_args(int arg)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_noisy_assert_failure_no_args()
{
    Assert_Failure();
}
static void c_test_noisy_assert_failure_msg_no_args()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Noisy Checks - src=none
//
static bool c_test_noisy_check_not_null_ptr_no_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_noisy_check_valid_state_no_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_noisy_check_failure_msg_no_args()
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool c_test_noisy_check_failure_no_args()
{
    Check_Failure(false);
}
static bool c_test_noisy_check_recovery_no_args()
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Noisy Arguments Checks - src=none
//
static bool c_test_noisy_check_arg_not_null_ptr_no_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_noisy_check_arg_is_valid_no_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_noisy_check_arg_array_no_args(unsigned len, const char* str)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_noisy_check_arg_invalid_no_args(int arg)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=noisy, src=none
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=noisy, src=none")
{
    CHECK_NOTHROW(call(c_test_noisy_assert_not_null_ptr_no_args, "abcde"));
    CHECK_NOTHROW(call(c_test_noisy_assert_check_no_args, 1));
    CHECK_NOTHROW(call(c_test_noisy_assert_check_msg_no_args, 1));

    CHECK_THROWS(call(c_test_noisy_assert_not_null_ptr_no_args, NULL));
    CHECK_THROWS(call(c_test_noisy_assert_check_no_args, 0));
    CHECK_THROWS(call(c_test_noisy_assert_check_msg_no_args, 0));

    CHECK_THROWS(call(c_test_noisy_assert_failure_no_args));
    CHECK_THROWS(call(c_test_noisy_assert_failure_msg_no_args));
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=noisy, src=none")
{
    CHECK(call(c_test_noisy_check_not_null_ptr_no_args, "abcde"));
    CHECK(call(c_test_noisy_check_valid_state_no_args, 1));

    CHECK_FALSE(call(c_test_noisy_check_not_null_ptr_no_args, NULL));
    CHECK_FALSE(call(c_test_noisy_check_valid_state_no_args, 0));

    CHECK_FALSE(call(c_test_noisy_check_failure_msg_no_args));
    CHECK_FALSE(call(c_test_noisy_check_failure_no_args));
    CHECK_FALSE(call(c_test_noisy_check_recovery_no_args));
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=noisy, src=none")
{
    CHECK(call(c_test_noisy_check_arg_not_null_ptr_no_args, "abcde"));
    CHECK(call(c_test_noisy_check_arg_is_valid_no_args, 1));
    CHECK(call(c_test_noisy_check_arg_array_no_args, 7U, "abc-def"));

    CHECK_FALSE(call(c_test_noisy_check_arg_not_null_ptr_no_args, NULL));
    CHECK_FALSE(call(c_test_noisy_check_arg_is_valid_no_args, 0));
    CHECK_FALSE(call(c_test_noisy_check_arg_array_no_args, 7U, "abc def"));

    CHECK_FALSE(call(c_test_noisy_check_arg_invalid_no_args, 0));
}

//
// SECTION: lang=C, mode=noisy, src=preprocessor
//
extern "C" {
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_c_halt_proc_pp_args
#define DIAG_STOP
#define DIAG_WARN           my_c_warn_proc_pp_args
#define DIAG_NOEXCEPT
#define DIAG_SRC_INFO       DIAG_SRC_INFO_PREPROCESSOR
#define DIAG_MODE           DIAG_MODE_NOISY
#include <gkr/diag/diagnostics.h>
//
// Noisy Asserts - src=preprocessor
//
static void c_test_noisy_assert_not_null_ptr_pp_args(const char* ptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_noisy_assert_check_pp_args(int arg)
{
    Assert_Check(arg == 1);
}
static void c_test_noisy_assert_check_msg_pp_args(int arg)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_noisy_assert_failure_pp_args()
{
    Assert_Failure();
}
static void c_test_noisy_assert_failure_msg_pp_args()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Noisy Checks - src=preprocessor
//
static bool c_test_noisy_check_not_null_ptr_pp_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_noisy_check_valid_state_pp_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_noisy_check_failure_msg_pp_args()
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool c_test_noisy_check_failure_pp_args()
{
    Check_Failure(false);
}
static bool c_test_noisy_check_recovery_pp_args()
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Noisy Arguments Checks - src=preprocessor
//
static bool c_test_noisy_check_arg_not_null_ptr_pp_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_noisy_check_arg_is_valid_pp_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_noisy_check_arg_array_pp_args(unsigned len, const char* str)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_noisy_check_arg_invalid_pp_args(int arg)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=noisy, src=preprocessor
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=noisy, src=preprocessor")
{
    CHECK_NOTHROW(call(c_test_noisy_assert_not_null_ptr_pp_args, "abcde"));
    CHECK_NOTHROW(call(c_test_noisy_assert_check_pp_args, 1));
    CHECK_NOTHROW(call(c_test_noisy_assert_check_msg_pp_args, 1));

    CHECK_THROWS(call(c_test_noisy_assert_not_null_ptr_pp_args, NULL));
    CHECK_THROWS(call(c_test_noisy_assert_check_pp_args, 0));
    CHECK_THROWS(call(c_test_noisy_assert_check_msg_pp_args, 0));

    CHECK_THROWS(call(c_test_noisy_assert_failure_pp_args));
    CHECK_THROWS(call(c_test_noisy_assert_failure_msg_pp_args));
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=noisy, src=preprocessor")
{
    CHECK(call(c_test_noisy_check_not_null_ptr_pp_args, "abcde"));
    CHECK(call(c_test_noisy_check_valid_state_pp_args, 1));

    CHECK_FALSE(call(c_test_noisy_check_not_null_ptr_pp_args, NULL));
    CHECK_FALSE(call(c_test_noisy_check_valid_state_pp_args, 0));

    CHECK_FALSE(call(c_test_noisy_check_failure_msg_pp_args));
    CHECK_FALSE(call(c_test_noisy_check_failure_pp_args));
    CHECK_FALSE(call(c_test_noisy_check_recovery_pp_args));
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=noisy, src=preprocessor")
{
    CHECK(call(c_test_noisy_check_arg_not_null_ptr_pp_args, "abcde"));
    CHECK(call(c_test_noisy_check_arg_is_valid_pp_args, 1));
    CHECK(call(c_test_noisy_check_arg_array_pp_args, 7U, "abc-def"));

    CHECK_FALSE(call(c_test_noisy_check_arg_not_null_ptr_pp_args, NULL));
    CHECK_FALSE(call(c_test_noisy_check_arg_is_valid_pp_args, 0));
    CHECK_FALSE(call(c_test_noisy_check_arg_array_pp_args, 7U, "abc def"));

    CHECK_FALSE(call(c_test_noisy_check_arg_invalid_pp_args, 0));
}

//
// SECTION: lang=C++, mode=noisy, src=none
//
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_no_args
#define DIAG_STOP
#define DIAG_WARN           my_cpp_warn_proc_no_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_NONE
#define DIAG_MODE           DIAG_MODE_NOISY
#include <gkr/diag/diagnostics.h>
//
// Noisy Asserts - src=none
//
static void cpp_test_noisy_assert_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_noisy_assert_check_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_noisy_assert_check_msg_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_noisy_assert_failure_no_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_noisy_assert_failure_msg_no_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Noisy Checks - src=none
//
static bool cpp_test_noisy_check_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_noisy_check_valid_state_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_noisy_check_failure_msg_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_noisy_check_failure_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_noisy_check_recovery_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// noisy Arguments Checks - src=none
//
static bool cpp_test_noisy_check_arg_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_noisy_check_arg_is_valid_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_noisy_check_arg_array_no_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_noisy_check_arg_invalid_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=noisy, src=none
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=noisy, src=none")
{
    CHECK_NOTHROW(cpp_test_noisy_assert_not_null_ptr_no_args("abcde"));
    CHECK_NOTHROW(cpp_test_noisy_assert_check_no_args(1));
    CHECK_NOTHROW(cpp_test_noisy_assert_check_msg_no_args(1));

    CHECK_THROWS(cpp_test_noisy_assert_not_null_ptr_no_args(nullptr));
    CHECK_THROWS(cpp_test_noisy_assert_check_no_args(0));
    CHECK_THROWS(cpp_test_noisy_assert_check_msg_no_args(0));

    CHECK_THROWS(cpp_test_noisy_assert_failure_no_args());
    CHECK_THROWS(cpp_test_noisy_assert_failure_msg_no_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=noisy, src=none")
{
    CHECK(cpp_test_noisy_check_not_null_ptr_no_args("abcde"));
    CHECK(cpp_test_noisy_check_valid_state_no_args(1));

    CHECK_FALSE(cpp_test_noisy_check_not_null_ptr_no_args(nullptr));
    CHECK_FALSE(cpp_test_noisy_check_valid_state_no_args(0));

    CHECK_FALSE(cpp_test_noisy_check_failure_msg_no_args());
    CHECK_FALSE(cpp_test_noisy_check_failure_no_args());
    CHECK_FALSE(cpp_test_noisy_check_recovery_no_args());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=noisy, src=none")
{
    CHECK(cpp_test_noisy_check_arg_not_null_ptr_no_args("abcde"));
    CHECK(cpp_test_noisy_check_arg_is_valid_no_args(1));
    CHECK(cpp_test_noisy_check_arg_array_no_args(7U, "abc-def"));

    CHECK_FALSE(cpp_test_noisy_check_arg_not_null_ptr_no_args(nullptr));
    CHECK_FALSE(cpp_test_noisy_check_arg_is_valid_no_args(0));
    CHECK_FALSE(cpp_test_noisy_check_arg_array_no_args(7U, "abc def"));

    CHECK_FALSE(cpp_test_noisy_check_arg_invalid_no_args(0));
}

//
// SECTION: lang=C++, mode=noisy, src=preprocessor
//
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_pp_args
#define DIAG_STOP
#define DIAG_WARN           my_cpp_warn_proc_pp_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_PREPROCESSOR
#define DIAG_MODE           DIAG_MODE_NOISY
#include <gkr/diag/diagnostics.h>
//
// Noisy Asserts - src=preprocessor
//
static void cpp_test_noisy_assert_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_noisy_assert_check_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_noisy_assert_check_msg_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_noisy_assert_failure_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_noisy_assert_failure_msg_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Noisy Checks - src=preprocessor
//
static bool cpp_test_noisy_check_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_noisy_check_valid_state_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_noisy_check_failure_msg_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_noisy_check_failure_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_noisy_check_recovery_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// noisy Arguments Checks - src=preprocessor
//
static bool cpp_test_noisy_check_arg_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_noisy_check_arg_is_valid_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_noisy_check_arg_array_pp_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_noisy_check_arg_invalid_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=noisy, src=preprocessor
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=noisy, src=preprocessor")
{
    CHECK_NOTHROW(cpp_test_noisy_assert_not_null_ptr_pp_args("abcde"));
    CHECK_NOTHROW(cpp_test_noisy_assert_check_pp_args(1));
    CHECK_NOTHROW(cpp_test_noisy_assert_check_msg_pp_args(1));

    CHECK_THROWS(cpp_test_noisy_assert_not_null_ptr_pp_args(nullptr));
    CHECK_THROWS(cpp_test_noisy_assert_check_pp_args(0));
    CHECK_THROWS(cpp_test_noisy_assert_check_msg_pp_args(0));

    CHECK_THROWS(cpp_test_noisy_assert_failure_pp_args());
    CHECK_THROWS(cpp_test_noisy_assert_failure_msg_pp_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=noisy, src=preprocessor")
{
    CHECK(cpp_test_noisy_check_not_null_ptr_pp_args("abcde"));
    CHECK(cpp_test_noisy_check_valid_state_pp_args(1));

    CHECK_FALSE(cpp_test_noisy_check_not_null_ptr_pp_args(nullptr));
    CHECK_FALSE(cpp_test_noisy_check_valid_state_pp_args(0));

    CHECK_FALSE(cpp_test_noisy_check_failure_msg_pp_args());
    CHECK_FALSE(cpp_test_noisy_check_failure_pp_args());
    CHECK_FALSE(cpp_test_noisy_check_recovery_pp_args());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=noisy, src=preprocessor")
{
    CHECK(cpp_test_noisy_check_arg_not_null_ptr_pp_args("abcde"));
    CHECK(cpp_test_noisy_check_arg_is_valid_pp_args(1));
    CHECK(cpp_test_noisy_check_arg_array_pp_args(7U, "abc-def"));

    CHECK_FALSE(cpp_test_noisy_check_arg_not_null_ptr_pp_args(nullptr));
    CHECK_FALSE(cpp_test_noisy_check_arg_is_valid_pp_args(0));
    CHECK_FALSE(cpp_test_noisy_check_arg_array_pp_args(7U, "abc def"));

    CHECK_FALSE(cpp_test_noisy_check_arg_invalid_pp_args(0));
}

//
// SECTION: lang=C++, mode=noisy, src=std_source_location
//
#ifdef __cpp_lib_source_location
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_sl_args
#define DIAG_STOP
#define DIAG_WARN           my_cpp_warn_proc_sl_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_SOURCE_LOCATION
#define DIAG_MODE           DIAG_MODE_NOISY
#include <gkr/diag/diagnostics.h>
//
// Noisy Asserts - src=std_source_location
//
static void cpp_test_noisy_assert_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_noisy_assert_check_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_noisy_assert_check_msg_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_noisy_assert_failure_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_noisy_assert_failure_msg_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// noisy Checks - src=std_source_location
//
static bool cpp_test_noisy_check_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_noisy_check_valid_state_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_noisy_check_failure_msg_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_noisy_check_failure_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_noisy_check_recovery_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// noisy Arguments Checks - src=std_source_location
//
static bool cpp_test_noisy_check_arg_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_noisy_check_arg_is_valid_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_noisy_check_arg_array_sl_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_noisy_check_arg_invalid_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
#endif

//
// Test cases: lang=C++, mode=noisy, src=std_source_location
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=noisy, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK_NOTHROW(cpp_test_noisy_assert_not_null_ptr_sl_args("abcde"));
    CHECK_NOTHROW(cpp_test_noisy_assert_check_sl_args(1));
    CHECK_NOTHROW(cpp_test_noisy_assert_check_msg_sl_args(1));

    CHECK_THROWS(cpp_test_noisy_assert_not_null_ptr_sl_args(nullptr));
    CHECK_THROWS(cpp_test_noisy_assert_check_sl_args(0));
    CHECK_THROWS(cpp_test_noisy_assert_check_msg_sl_args(0));

    CHECK_THROWS(cpp_test_noisy_assert_failure_sl_args());
    CHECK_THROWS(cpp_test_noisy_assert_failure_msg_sl_args());
#endif
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=noisy, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK(cpp_test_noisy_check_not_null_ptr_sl_args("abcde"));
    CHECK(cpp_test_noisy_check_valid_state_sl_args(1));

    CHECK_FALSE(cpp_test_noisy_check_not_null_ptr_sl_args(nullptr));
    CHECK_FALSE(cpp_test_noisy_check_valid_state_sl_args(0));

    CHECK_FALSE(cpp_test_noisy_check_failure_msg_sl_args());
    CHECK_FALSE(cpp_test_noisy_check_failure_sl_args());
    CHECK_FALSE(cpp_test_noisy_check_recovery_sl_args());
#endif
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=noisy, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK(cpp_test_noisy_check_arg_not_null_ptr_sl_args("abcde"));
    CHECK(cpp_test_noisy_check_arg_is_valid_sl_args(1));
    CHECK(cpp_test_noisy_check_arg_array_sl_args(7U, "abc-def"));

    CHECK_FALSE(cpp_test_noisy_check_arg_not_null_ptr_sl_args(nullptr));
    CHECK_FALSE(cpp_test_noisy_check_arg_is_valid_sl_args(0));
    CHECK_FALSE(cpp_test_noisy_check_arg_array_sl_args(7U, "abc def"));

    CHECK_FALSE(cpp_test_noisy_check_arg_invalid_sl_args(0));
#endif
}

//
// SECTION: lang=C++, mode=noisy, src=std_stacktrace
//
#ifdef __cpp_lib_stacktrace
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_st_args
#define DIAG_STOP
#define DIAG_WARN           my_cpp_warn_proc_st_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_STACKTRACE
#define DIAG_MODE           DIAG_MODE_NOISY
#include <gkr/diag/diagnostics.h>
//
// Noisy Asserts - src=std_stacktrace
//
static void cpp_test_noisy_assert_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_noisy_assert_check_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_noisy_assert_check_msg_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_noisy_assert_failure_st_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_noisy_assert_failure_msg_st_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Noisy Checks - src=std_stacktrace
//
static bool cpp_test_noisy_check_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_noisy_check_valid_state_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_noisy_check_failure_msg_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_noisy_check_failure_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_noisy_check_recovery_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Noisy Arguments Checks - src=std_stacktrace
//
static bool cpp_test_noisy_check_arg_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_noisy_check_arg_is_valid_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_noisy_check_arg_array_st_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_noisy_check_arg_invalid_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
#endif
//
// Test cases: lang=C++, mode=noisy, src=std_stacktrace
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=noisy, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK_NOTHROW(cpp_test_noisy_assert_not_null_ptr_st_args("abcde"));
    CHECK_NOTHROW(cpp_test_noisy_assert_check_st_args(1));
    CHECK_NOTHROW(cpp_test_noisy_assert_check_msg_st_args(1));

    CHECK_THROWS(cpp_test_noisy_assert_not_null_ptr_st_args(nullptr));
    CHECK_THROWS(cpp_test_noisy_assert_check_st_args(0));
    CHECK_THROWS(cpp_test_noisy_assert_check_msg_st_args(0));

    CHECK_THROWS(cpp_test_noisy_assert_failure_st_args());
    CHECK_THROWS(cpp_test_noisy_assert_failure_msg_st_args());
#endif
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=noisy, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK(cpp_test_noisy_check_not_null_ptr_st_args("abcde"));
    CHECK(cpp_test_noisy_check_valid_state_st_args(1));

    CHECK_FALSE(cpp_test_noisy_check_not_null_ptr_st_args(nullptr));
    CHECK_FALSE(cpp_test_noisy_check_valid_state_st_args(0));

    CHECK_FALSE(cpp_test_noisy_check_failure_msg_st_args());
    CHECK_FALSE(cpp_test_noisy_check_failure_st_args());
    CHECK_FALSE(cpp_test_noisy_check_recovery_st_args());
#endif
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=noisy, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK(cpp_test_noisy_check_arg_not_null_ptr_st_args("abcde"));
    CHECK(cpp_test_noisy_check_arg_is_valid_st_args(1));
    CHECK(cpp_test_noisy_check_arg_array_st_args(7U, "abc-def"));

    CHECK_FALSE(cpp_test_noisy_check_arg_not_null_ptr_st_args(nullptr));
    CHECK_FALSE(cpp_test_noisy_check_arg_is_valid_st_args(0));
    CHECK_FALSE(cpp_test_noisy_check_arg_array_st_args(7U, "abc def"));

    CHECK_FALSE(cpp_test_noisy_check_arg_invalid_st_args(0));
#endif
}

//
// SECTION: lang=C, mode=intrusive, src=none
//
extern "C" {
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_c_halt_proc_no_args
#define DIAG_STOP           my_c_stop_proc_no_args
#define DIAG_WARN
#define DIAG_NOEXCEPT
#define DIAG_SRC_INFO       DIAG_SRC_INFO_NONE
#define DIAG_MODE           DIAG_MODE_INTRUSIVE
#include <gkr/diag/diagnostics.h>
//
// Intrusive Asserts - src=none
//
static void c_test_intrusive_assert_not_null_ptr_no_args(const char* ptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_intrusive_assert_check_no_args(int arg)
{
    Assert_Check(arg == 1);
}
static void c_test_intrusive_assert_check_msg_no_args(int arg)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_intrusive_assert_failure_no_args()
{
    Assert_Failure();
}
static void c_test_intrusive_assert_failure_msg_no_args()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Intrusive Checks - src=none
//
static bool c_test_intrusive_check_not_null_ptr_no_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_intrusive_check_valid_state_no_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_intrusive_check_failure_msg_no_args()
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool c_test_intrusive_check_failure_no_args()
{
    Check_Failure(false);
}
static bool c_test_intrusive_check_recovery_no_args()
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Intrusive Arguments Checks - src=none
//
static bool c_test_intrusive_check_arg_not_null_ptr_no_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_intrusive_check_arg_is_valid_no_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_intrusive_check_arg_array_no_args(unsigned len, const char* str)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_intrusive_check_arg_invalid_no_args(int arg)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=intrusive, src=none
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=intrusive, src=none")
{
    CHECK_NOTHROW(call(c_test_intrusive_assert_not_null_ptr_no_args, "abcde"));
    CHECK_NOTHROW(call(c_test_intrusive_assert_check_no_args, 1));
    CHECK_NOTHROW(call(c_test_intrusive_assert_check_msg_no_args, 1));

    CHECK_THROWS(call(c_test_intrusive_assert_not_null_ptr_no_args, NULL));
    CHECK_THROWS(call(c_test_intrusive_assert_check_no_args, 0));
    CHECK_THROWS(call(c_test_intrusive_assert_check_msg_no_args, 0));

    CHECK_THROWS(call(c_test_intrusive_assert_failure_no_args));
    CHECK_THROWS(call(c_test_intrusive_assert_failure_msg_no_args));
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=intrusive, src=none")
{
    CHECK(call(c_test_intrusive_check_not_null_ptr_no_args, "abcde"));
    CHECK(call(c_test_intrusive_check_valid_state_no_args, 1));

    CHECK_THROWS(call(c_test_intrusive_check_not_null_ptr_no_args, NULL));
    CHECK_THROWS(call(c_test_intrusive_check_valid_state_no_args, 0));

    CHECK_THROWS(call(c_test_intrusive_check_failure_msg_no_args));
    CHECK_THROWS(call(c_test_intrusive_check_failure_no_args));
    CHECK_THROWS(call(c_test_intrusive_check_recovery_no_args));
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=intrusive, src=none")
{
    CHECK(call(c_test_intrusive_check_arg_not_null_ptr_no_args, "abcde"));
    CHECK(call(c_test_intrusive_check_arg_is_valid_no_args, 1));
    CHECK(call(c_test_intrusive_check_arg_array_no_args, 7U, "abc-def"));

    CHECK_THROWS(call(c_test_intrusive_check_arg_not_null_ptr_no_args, NULL));
    CHECK_THROWS(call(c_test_intrusive_check_arg_is_valid_no_args, 0));
    CHECK_THROWS(call(c_test_intrusive_check_arg_array_no_args, 7U, "abc def"));

    CHECK_THROWS(call(c_test_intrusive_check_arg_invalid_no_args, 0));
}

//
// SECTION: lang=C, mode=intrusive, src=preprocessor
//
extern "C" {
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_c_halt_proc_pp_args
#define DIAG_STOP           my_c_stop_proc_pp_args
#define DIAG_WARN
#define DIAG_NOEXCEPT
#define DIAG_SRC_INFO       DIAG_SRC_INFO_PREPROCESSOR
#define DIAG_MODE           DIAG_MODE_INTRUSIVE
#include <gkr/diag/diagnostics.h>
//
// Intrusive Asserts - src=preprocessor
//
static void c_test_intrusive_assert_not_null_ptr_pp_args(const char* ptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_intrusive_assert_check_pp_args(int arg)
{
    Assert_Check(arg == 1);
}
static void c_test_intrusive_assert_check_msg_pp_args(int arg)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_intrusive_assert_failure_pp_args()
{
    Assert_Failure();
}
static void c_test_intrusive_assert_failure_msg_pp_args()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Intrusive Checks - src=preprocessor
//
static bool c_test_intrusive_check_not_null_ptr_pp_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_intrusive_check_valid_state_pp_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_intrusive_check_failure_msg_pp_args()
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool c_test_intrusive_check_failure_pp_args()
{
    Check_Failure(false);
}
static bool c_test_intrusive_check_recovery_pp_args()
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Intrusive Arguments Checks - src=preprocessor
//
static bool c_test_intrusive_check_arg_not_null_ptr_pp_args(const char* ptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_intrusive_check_arg_is_valid_pp_args(int arg)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_intrusive_check_arg_array_pp_args(unsigned len, const char* str)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_intrusive_check_arg_invalid_pp_args(int arg)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=intrusive, src=preprocessor
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=intrusive, src=preprocessor")
{
    CHECK_NOTHROW(call(c_test_intrusive_assert_not_null_ptr_pp_args, "abcde"));
    CHECK_NOTHROW(call(c_test_intrusive_assert_check_pp_args, 1));
    CHECK_NOTHROW(call(c_test_intrusive_assert_check_msg_pp_args, 1));

    CHECK_THROWS(call(c_test_intrusive_assert_not_null_ptr_pp_args, NULL));
    CHECK_THROWS(call(c_test_intrusive_assert_check_pp_args, 0));
    CHECK_THROWS(call(c_test_intrusive_assert_check_msg_pp_args, 0));

    CHECK_THROWS(call(c_test_intrusive_assert_failure_pp_args));
    CHECK_THROWS(call(c_test_intrusive_assert_failure_msg_pp_args));
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=intrusive, src=preprocessor")
{
    CHECK(call(c_test_intrusive_check_not_null_ptr_pp_args, "abcde"));
    CHECK(call(c_test_intrusive_check_valid_state_pp_args, 1));

    CHECK_THROWS(call(c_test_intrusive_check_not_null_ptr_pp_args, NULL));
    CHECK_THROWS(call(c_test_intrusive_check_valid_state_pp_args, 0));

    CHECK_THROWS(call(c_test_intrusive_check_failure_msg_pp_args));
    CHECK_THROWS(call(c_test_intrusive_check_failure_pp_args));
    CHECK_THROWS(call(c_test_intrusive_check_recovery_pp_args));
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=intrusive, src=preprocessor")
{
    CHECK(call(c_test_intrusive_check_arg_not_null_ptr_pp_args, "abcde"));
    CHECK(call(c_test_intrusive_check_arg_is_valid_pp_args, 1));
    CHECK(call(c_test_intrusive_check_arg_array_pp_args, 7U, "abc-def"));

    CHECK_THROWS(call(c_test_intrusive_check_arg_not_null_ptr_pp_args, NULL));
    CHECK_THROWS(call(c_test_intrusive_check_arg_is_valid_pp_args, 0));
    CHECK_THROWS(call(c_test_intrusive_check_arg_array_pp_args, 7U, "abc def"));

    CHECK_THROWS(call(c_test_intrusive_check_arg_invalid_pp_args, 0));
}

//
// SECTION: lang=C++, mode=intrusive, src=none
//
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_no_args
#define DIAG_STOP           my_cpp_stop_proc_no_args
#define DIAG_WARN
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_NONE
#define DIAG_MODE           DIAG_MODE_INTRUSIVE
#include <gkr/diag/diagnostics.h>
//
// Intrusive Asserts - src=none
//
static void cpp_test_intrusive_assert_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_intrusive_assert_check_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_intrusive_assert_check_msg_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_intrusive_assert_failure_no_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_intrusive_assert_failure_msg_no_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Intrusive Checks - src=none
//
static bool cpp_test_intrusive_check_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_intrusive_check_valid_state_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_intrusive_check_failure_msg_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_intrusive_check_failure_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_intrusive_check_recovery_no_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// intrusive Arguments Checks - src=none
//
static bool cpp_test_intrusive_check_arg_not_null_ptr_no_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_intrusive_check_arg_is_valid_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_intrusive_check_arg_array_no_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_intrusive_check_arg_invalid_no_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=intrusive, src=none
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=intrusive, src=none")
{
    CHECK_NOTHROW(cpp_test_intrusive_assert_not_null_ptr_no_args("abcde"));
    CHECK_NOTHROW(cpp_test_intrusive_assert_check_no_args(1));
    CHECK_NOTHROW(cpp_test_intrusive_assert_check_msg_no_args(1));

    CHECK_THROWS(cpp_test_intrusive_assert_not_null_ptr_no_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_assert_check_no_args(0));
    CHECK_THROWS(cpp_test_intrusive_assert_check_msg_no_args(0));

    CHECK_THROWS(cpp_test_intrusive_assert_failure_no_args());
    CHECK_THROWS(cpp_test_intrusive_assert_failure_msg_no_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=intrusive, src=none")
{
    CHECK(cpp_test_intrusive_check_not_null_ptr_no_args("abcde"));
    CHECK(cpp_test_intrusive_check_valid_state_no_args(1));

    CHECK_THROWS(cpp_test_intrusive_check_not_null_ptr_no_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_check_valid_state_no_args(0));

    CHECK_THROWS(cpp_test_intrusive_check_failure_msg_no_args());
    CHECK_THROWS(cpp_test_intrusive_check_failure_no_args());
    CHECK_THROWS(cpp_test_intrusive_check_recovery_no_args());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=intrusive, src=none")
{
    CHECK(cpp_test_intrusive_check_arg_not_null_ptr_no_args("abcde"));
    CHECK(cpp_test_intrusive_check_arg_is_valid_no_args(1));
    CHECK(cpp_test_intrusive_check_arg_array_no_args(7U, "abc-def"));

    CHECK_THROWS(cpp_test_intrusive_check_arg_not_null_ptr_no_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_check_arg_is_valid_no_args(0));
    CHECK_THROWS(cpp_test_intrusive_check_arg_array_no_args(7U, "abc def"));

    CHECK_THROWS(cpp_test_intrusive_check_arg_invalid_no_args(0));
}

//
// SECTION: lang=C++, mode=intrusive, src=preprocessor
//
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_pp_args
#define DIAG_STOP           my_cpp_stop_proc_pp_args
#define DIAG_WARN
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_PREPROCESSOR
#define DIAG_MODE           DIAG_MODE_INTRUSIVE
#include <gkr/diag/diagnostics.h>
//
// Intrusive Asserts - src=preprocessor
//
static void cpp_test_intrusive_assert_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_intrusive_assert_check_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_intrusive_assert_check_msg_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_intrusive_assert_failure_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_intrusive_assert_failure_msg_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Intrusive Checks - src=preprocessor
//
static bool cpp_test_intrusive_check_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_intrusive_check_valid_state_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_intrusive_check_failure_msg_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_intrusive_check_failure_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_intrusive_check_recovery_pp_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Intrusive Arguments Checks - src=preprocessor
//
static bool cpp_test_intrusive_check_arg_not_null_ptr_pp_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_intrusive_check_arg_is_valid_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_intrusive_check_arg_array_pp_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_intrusive_check_arg_invalid_pp_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=intrusive, src=preprocessor
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=intrusive, src=preprocessor")
{
    CHECK_NOTHROW(cpp_test_intrusive_assert_not_null_ptr_pp_args("abcde"));
    CHECK_NOTHROW(cpp_test_intrusive_assert_check_pp_args(1));
    CHECK_NOTHROW(cpp_test_intrusive_assert_check_msg_pp_args(1));

    CHECK_THROWS(cpp_test_intrusive_assert_not_null_ptr_pp_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_assert_check_pp_args(0));
    CHECK_THROWS(cpp_test_intrusive_assert_check_msg_pp_args(0));

    CHECK_THROWS(cpp_test_intrusive_assert_failure_pp_args());
    CHECK_THROWS(cpp_test_intrusive_assert_failure_msg_pp_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=intrusive, src=preprocessor")
{
    CHECK(cpp_test_intrusive_check_not_null_ptr_pp_args("abcde"));
    CHECK(cpp_test_intrusive_check_valid_state_pp_args(1));

    CHECK_THROWS(cpp_test_intrusive_check_not_null_ptr_pp_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_check_valid_state_pp_args(0));

    CHECK_THROWS(cpp_test_intrusive_check_failure_msg_pp_args());
    CHECK_THROWS(cpp_test_intrusive_check_failure_pp_args());
    CHECK_THROWS(cpp_test_intrusive_check_recovery_pp_args());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=intrusive, src=preprocessor")
{
    CHECK(cpp_test_intrusive_check_arg_not_null_ptr_pp_args("abcde"));
    CHECK(cpp_test_intrusive_check_arg_is_valid_pp_args(1));
    CHECK(cpp_test_intrusive_check_arg_array_pp_args(7U, "abc-def"));

    CHECK_THROWS(cpp_test_intrusive_check_arg_not_null_ptr_pp_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_check_arg_is_valid_pp_args(0));
    CHECK_THROWS(cpp_test_intrusive_check_arg_array_pp_args(7U, "abc def"));

    CHECK_THROWS(cpp_test_intrusive_check_arg_invalid_pp_args(0));
}

//
// SECTION: lang=C++, mode=intrusive, src=std_source_location
//
#ifdef __cpp_lib_source_location
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_sl_args
#define DIAG_STOP           my_cpp_stop_proc_sl_args
#define DIAG_WARN
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_SOURCE_LOCATION
#define DIAG_MODE           DIAG_MODE_INTRUSIVE
#include <gkr/diag/diagnostics.h>
//
// Intrusive Asserts - src=std_source_location
//
static void cpp_test_intrusive_assert_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_intrusive_assert_check_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_intrusive_assert_check_msg_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_intrusive_assert_failure_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_intrusive_assert_failure_msg_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Intrusive Checks - src=std_source_location
//
static bool cpp_test_intrusive_check_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_intrusive_check_valid_state_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_intrusive_check_failure_msg_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_intrusive_check_failure_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_intrusive_check_recovery_sl_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Intrusive Arguments Checks - src=std_source_location
//
static bool cpp_test_intrusive_check_arg_not_null_ptr_sl_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_intrusive_check_arg_is_valid_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_intrusive_check_arg_array_sl_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_intrusive_check_arg_invalid_sl_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
#endif

//
// Test cases: lang=C++, mode=intrusive, src=std_source_location
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=intrusive, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK_NOTHROW(cpp_test_intrusive_assert_not_null_ptr_sl_args("abcde"));
    CHECK_NOTHROW(cpp_test_intrusive_assert_check_sl_args(1));
    CHECK_NOTHROW(cpp_test_intrusive_assert_check_msg_sl_args(1));

    CHECK_THROWS(cpp_test_intrusive_assert_not_null_ptr_sl_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_assert_check_sl_args(0));
    CHECK_THROWS(cpp_test_intrusive_assert_check_msg_sl_args(0));

    CHECK_THROWS(cpp_test_intrusive_assert_failure_sl_args());
    CHECK_THROWS(cpp_test_intrusive_assert_failure_msg_sl_args());
#endif
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=intrusive, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK(cpp_test_intrusive_check_not_null_ptr_sl_args("abcde"));
    CHECK(cpp_test_intrusive_check_valid_state_sl_args(1));

    CHECK_THROWS(cpp_test_intrusive_check_not_null_ptr_sl_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_check_valid_state_sl_args(0));

    CHECK_THROWS(cpp_test_intrusive_check_failure_msg_sl_args());
    CHECK_THROWS(cpp_test_intrusive_check_failure_sl_args());
    CHECK_THROWS(cpp_test_intrusive_check_recovery_sl_args());
#endif
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=intrusive, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK(cpp_test_intrusive_check_arg_not_null_ptr_sl_args("abcde"));
    CHECK(cpp_test_intrusive_check_arg_is_valid_sl_args(1));
    CHECK(cpp_test_intrusive_check_arg_array_sl_args(7U, "abc-def"));

    CHECK_THROWS(cpp_test_intrusive_check_arg_not_null_ptr_sl_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_check_arg_is_valid_sl_args(0));
    CHECK_THROWS(cpp_test_intrusive_check_arg_array_sl_args(7U, "abc def"));

    CHECK_THROWS(cpp_test_intrusive_check_arg_invalid_sl_args(0));
#endif
}

//
// SECTION: lang=C++, mode=intrusive, src=std_stacktrace
//
#ifdef __cpp_lib_stacktrace
#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_st_args
#define DIAG_STOP           my_cpp_stop_proc_st_args
#define DIAG_WARN
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_STACKTRACE
#define DIAG_MODE           DIAG_MODE_INTRUSIVE
#include <gkr/diag/diagnostics.h>
//
// Intrusive Asserts - src=std_stacktrace
//
static void cpp_test_intrusive_assert_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_intrusive_assert_check_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_intrusive_assert_check_msg_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_intrusive_assert_failure_st_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
}
static void cpp_test_intrusive_assert_failure_msg_st_args() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
}
//
// Intrusive Checks - src=std_stacktrace
//
static bool cpp_test_intrusive_check_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_intrusive_check_valid_state_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_intrusive_check_failure_msg_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_FailureMsg(TEST_CHECK_FAILURE_MESSAGE, false);
}
static bool cpp_test_intrusive_check_failure_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Failure(false);
}
static bool cpp_test_intrusive_check_recovery_st_args() noexcept(DIAG_NOEXCEPT)
{
    Check_Recovery(TEST_CHECK_RECOVERY_MESSAGE);
    return false;
}
//
// Intrusive Arguments Checks - src=std_stacktrace
//
static bool cpp_test_intrusive_check_arg_not_null_ptr_st_args(const char* ptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_intrusive_check_arg_is_valid_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_intrusive_check_arg_array_st_args(unsigned len, const char* str) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_intrusive_check_arg_invalid_st_args(int arg) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
#endif
//
// Test cases: lang=C++, mode=intrusive, src=std_stacktrace
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=intrusive, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK_NOTHROW(cpp_test_intrusive_assert_not_null_ptr_st_args("abcde"));
    CHECK_NOTHROW(cpp_test_intrusive_assert_check_st_args(1));
    CHECK_NOTHROW(cpp_test_intrusive_assert_check_msg_st_args(1));

    CHECK_THROWS(cpp_test_intrusive_assert_not_null_ptr_st_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_assert_check_st_args(0));
    CHECK_THROWS(cpp_test_intrusive_assert_check_msg_st_args(0));

    CHECK_THROWS(cpp_test_intrusive_assert_failure_st_args());
    CHECK_THROWS(cpp_test_intrusive_assert_failure_msg_st_args());
#endif
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=intrusive, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK(cpp_test_intrusive_check_not_null_ptr_st_args("abcde"));
    CHECK(cpp_test_intrusive_check_valid_state_st_args(1));

    CHECK_THROWS(cpp_test_intrusive_check_not_null_ptr_st_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_check_valid_state_st_args(0));

    CHECK_THROWS(cpp_test_intrusive_check_failure_msg_st_args());
    CHECK_THROWS(cpp_test_intrusive_check_failure_st_args());
    CHECK_THROWS(cpp_test_intrusive_check_recovery_st_args());
#endif
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=intrusive, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK(cpp_test_intrusive_check_arg_not_null_ptr_st_args("abcde"));
    CHECK(cpp_test_intrusive_check_arg_is_valid_st_args(1));
    CHECK(cpp_test_intrusive_check_arg_array_st_args(7U, "abc-def"));

    CHECK_THROWS(cpp_test_intrusive_check_arg_not_null_ptr_st_args(nullptr));
    CHECK_THROWS(cpp_test_intrusive_check_arg_is_valid_st_args(0));
    CHECK_THROWS(cpp_test_intrusive_check_arg_array_st_args(7U, "abc def"));

    CHECK_THROWS(cpp_test_intrusive_check_arg_invalid_st_args(0));
#endif
}
