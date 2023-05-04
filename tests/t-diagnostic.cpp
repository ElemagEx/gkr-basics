#include <catch2/catch_test_macros.hpp>

#define DIAG_UNIT_TESTING
#include <gkr/diag/diagnostics.h>

#include <exception>

#define TEST_ASSERT_CHECK_MESSAGE   "Test Assert Check Message"
#define TEST_ASSERT_FAILURE_MESSAGE "Test Assert Failure Message"
#define TEST_CHECK_FAILURE_MESSAGE  "Test Check Failure Message"
#define TEST_CHECK_RECOVERY_MESSAGE "Test Check Recovery Message"

//
// C++ procs - no args
//
/*static*/ void my_cpp_halt_proc_no_args(int id, const char* msg) noexcept(false)
{
    throw std::exception();
}
/*static*/ int my_cpp_stop_proc_no_args(int id, const char* msg) noexcept(false)
{
    throw std::exception();
}
/*static*/ int my_cpp_warn_proc_no_args(int id, const char* msg) noexcept
{
    return 1;
}
//
// C++ procs - preprocessor args
//
/*static*/ void my_cpp_halt_proc_pp_args(int id, const char* msg, const char* file, int line) noexcept(false)
{
    throw std::exception();
}
/*static*/ int my_cpp_stop_proc_pp_args(int id, const char* msg, const char* file, int line) noexcept(false)
{
    throw std::exception();
}
/*static*/ int my_cpp_warn_proc_pp_args(int id, const char* msg, const char* file, int line) noexcept
{
    return 1;
}
//
// C++ procs - std::source_location args
//
#ifdef __cpp_lib_source_location
#include <source_location>
/*static*/ void my_cpp_halt_proc_sl_args(int id, const char* msg, const std::source_location& location) noexcept(false)
{
    throw std::exception();
}
/*static*/ int my_cpp_stop_proc_sl_args(int id, const char* msg, const std::source_location& location) noexcept(false)
{
    throw std::exception();
}
/*static*/ int my_cpp_warn_proc_sl_args(int id, const char* msg, const std::source_location& location) noexcept
{
    return 1;
}
#endif
//
// C++ procs - std::stacktrace args
//
#ifdef __cpp_lib_stacktrace
#include <stacktrace>
/*static*/ void my_cpp_halt_proc_st_args(int id, const char* msg, const std::stacktrace& stacktrace) noexcept(false)
{
    throw std::exception();
}
/*static*/ int my_cpp_stop_proc_st_args(int id, const char* msg, const std::stacktrace& stacktrace) noexcept(false)
{
    throw std::exception();
}
/*static*/ int my_cpp_warn_proc_st_args(int id, const char* msg, const std::stacktrace& stacktrace) noexcept
{
    return 1;
}
#endif

extern "C" {
//
// C procs - no args
//
/*static*/ void my_c_halt_proc_no_args(int id, const char* msg)
{
}
/*static*/ int my_c_stop_proc_no_args(int id, const char* msg)
{
    return 0;
}
/*static*/ int my_c_warn_proc_no_args(int id, const char* msg)
{
    return 1;
}
//
// C procs - no args
//
/*static*/ void my_c_halt_proc_pp_args(int id, const char* msg, const char* file, int line)
{
}
/*static*/ int my_c_stop_proc_pp_args(int id, const char* msg, const char* file, int line)
{
    return 0;
}
/*static*/ int my_c_warn_proc_pp_args(int id, const char* msg, const char* file, int line)
{
    return 1;
}
}

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
static void c_test_trigger_disabled_assert_not_null_ptr(const char* ptr = nullptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_success_disabled_assert_not_null_ptr(const char* ptr = "abc")
{
    Assert_NotNullPtr(ptr);
}
static void c_test_trigger_disabled_assert_check(int arg = 0)
{
    Assert_Check(arg == 1);
}
static void c_test_success_disabled_assert_check(int arg = 0)
{
    Assert_Check(arg == 0);
}
static void c_test_trigger_disabled_assert_check_msg(int arg = 0)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_success_disabled_assert_check_msg(int arg = 0)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool c_test_trigger_disabled_check_not_null_ptr(const char* ptr = nullptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_success_disabled_check_not_null_ptr(const char* ptr = "abc")
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_trigger_disabled_check_valid_state(int arg = 0)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_success_disabled_check_valid_state(int arg = 1)
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
static bool c_test_trigger_disabled_check_arg_not_null_ptr(const char* ptr = nullptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_success_disabled_check_arg_not_null_ptr(const char* ptr = "abc")
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_trigger_disabled_check_arg_is_valid(int arg = 0)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_success_disabled_check_arg_is_valid(int arg = 1)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_trigger_disabled_check_arg_array(unsigned len = 7, const char* str = "abc def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_success_disabled_check_arg_array(unsigned len = 7, const char* str = "abc-def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_disabled_check_arg_invalid(int arg = 0)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=disabled, src=undefined
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=disabled, src=undefined")
{
    CHECK_NOTHROW(c_test_success_disabled_assert_not_null_ptr());
    CHECK_NOTHROW(c_test_success_disabled_assert_check());
    CHECK_NOTHROW(c_test_success_disabled_assert_check_msg());

    CHECK_NOTHROW(c_test_trigger_disabled_assert_not_null_ptr());
    CHECK_NOTHROW(c_test_trigger_disabled_assert_check());
    CHECK_NOTHROW(c_test_trigger_disabled_assert_check_msg());

    CHECK_NOTHROW(c_test_disabled_assert_failure());
    CHECK_NOTHROW(c_test_disabled_assert_failure_msg());
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=disabled, src=undefined")
{
    CHECK(c_test_success_disabled_check_not_null_ptr());
    CHECK(c_test_success_disabled_check_valid_state());

    CHECK(c_test_trigger_disabled_check_not_null_ptr());
    CHECK(c_test_trigger_disabled_check_valid_state());

    CHECK_FALSE(c_test_disabled_check_failure_msg());
    CHECK_FALSE(c_test_disabled_check_failure());
    CHECK_FALSE(c_test_disabled_check_recovery());
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=disabled, src=undefined")
{
    CHECK(c_test_success_disabled_check_arg_not_null_ptr());
    CHECK(c_test_success_disabled_check_arg_is_valid());
    CHECK(c_test_success_disabled_check_arg_array());

    CHECK(c_test_trigger_disabled_check_arg_not_null_ptr());
    CHECK(c_test_trigger_disabled_check_arg_is_valid());
    CHECK(c_test_trigger_disabled_check_arg_array());

    CHECK_FALSE(c_test_disabled_check_arg_invalid());
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
static void cpp_test_trigger_disabled_assert_not_null_ptr(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_success_disabled_assert_not_null_ptr(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_trigger_disabled_assert_check(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_success_disabled_assert_check(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 0);
}
static void cpp_test_trigger_disabled_assert_check_msg(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_success_disabled_assert_check_msg(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool cpp_test_trigger_disabled_check_not_null_ptr(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_disabled_check_not_null_ptr(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_disabled_check_valid_state(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_disabled_check_valid_state(int arg = 1) noexcept(DIAG_NOEXCEPT)
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
static bool cpp_test_trigger_disabled_check_arg_not_null_ptr(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_disabled_check_arg_not_null_ptr(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_disabled_check_arg_is_valid(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_disabled_check_arg_is_valid(int arg = 1) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_trigger_disabled_check_arg_array(unsigned len = 7, const char* str = "abc def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_success_disabled_check_arg_array(unsigned len = 7, const char* str = "abc-def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_disabled_check_arg_invalid(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=disabled, src=undefined
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=disabled, src=undefined")
{
    CHECK_NOTHROW(cpp_test_success_disabled_assert_not_null_ptr());
    CHECK_NOTHROW(cpp_test_success_disabled_assert_check());
    CHECK_NOTHROW(cpp_test_success_disabled_assert_check_msg());

    CHECK_NOTHROW(cpp_test_trigger_disabled_assert_not_null_ptr());
    CHECK_NOTHROW(cpp_test_trigger_disabled_assert_check());
    CHECK_NOTHROW(cpp_test_trigger_disabled_assert_check_msg());

    CHECK_NOTHROW(cpp_test_disabled_assert_failure());
    CHECK_NOTHROW(cpp_test_disabled_assert_failure_msg());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=disabled, src=undefined")
{
    CHECK(cpp_test_success_disabled_check_not_null_ptr());
    CHECK(cpp_test_success_disabled_check_valid_state());

    CHECK(cpp_test_trigger_disabled_check_not_null_ptr());
    CHECK(cpp_test_trigger_disabled_check_valid_state());

    CHECK_FALSE(cpp_test_disabled_check_failure_msg());
    CHECK_FALSE(cpp_test_disabled_check_failure());
    CHECK_FALSE(cpp_test_disabled_check_recovery());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=disabled, src=undefined")
{
    CHECK(cpp_test_success_disabled_check_arg_not_null_ptr());
    CHECK(cpp_test_success_disabled_check_arg_is_valid());
    CHECK(cpp_test_success_disabled_check_arg_array());

    CHECK(cpp_test_trigger_disabled_check_arg_not_null_ptr());
    CHECK(cpp_test_trigger_disabled_check_arg_is_valid());
    CHECK(cpp_test_trigger_disabled_check_arg_array());

    CHECK_FALSE(cpp_test_disabled_check_arg_invalid());
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
static void c_test_trigger_silent_assert_not_null_ptr(const char* ptr = nullptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_success_silent_assert_not_null_ptr(const char* ptr = "abc")
{
    Assert_NotNullPtr(ptr);
}
static void c_test_trigger_silent_assert_check(int arg = 0)
{
    Assert_Check(arg == 1);
}
static void c_test_success_silent_assert_check(int arg = 0)
{
    Assert_Check(arg == 0);
}
static void c_test_trigger_silent_assert_check_msg(int arg = 0)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_success_silent_assert_check_msg(int arg = 0)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool c_test_trigger_silent_check_not_null_ptr(const char* ptr = nullptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_success_silent_check_not_null_ptr(const char* ptr = "abc")
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_trigger_silent_check_valid_state(int arg = 0)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_success_silent_check_valid_state(int arg = 1)
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
static bool c_test_trigger_silent_check_arg_not_null_ptr(const char* ptr = nullptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_success_silent_check_arg_not_null_ptr(const char* ptr = "abc")
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_trigger_silent_check_arg_is_valid(int arg = 0)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_success_silent_check_arg_is_valid(int arg = 1)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_trigger_silent_check_arg_array(unsigned len = 7, const char* str = "abc def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_success_silent_check_arg_array(unsigned len = 7, const char* str = "abc-def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_silent_check_arg_invalid(int arg = 0)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=silent, src=undefined
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=silent, src=undefined")
{
    CHECK_NOTHROW(c_test_success_silent_assert_not_null_ptr());
    CHECK_NOTHROW(c_test_success_silent_assert_check());
    CHECK_NOTHROW(c_test_success_silent_assert_check_msg());

    CHECK_NOTHROW(c_test_trigger_silent_assert_not_null_ptr());
    CHECK_NOTHROW(c_test_trigger_silent_assert_check());
    CHECK_NOTHROW(c_test_trigger_silent_assert_check_msg());

    CHECK_NOTHROW(c_test_silent_assert_failure());
    CHECK_NOTHROW(c_test_silent_assert_failure_msg());
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=silent, src=undefined")
{
    CHECK(c_test_success_silent_check_not_null_ptr());
    CHECK(c_test_success_silent_check_valid_state());

    CHECK_FALSE(c_test_trigger_silent_check_not_null_ptr());
    CHECK_FALSE(c_test_trigger_silent_check_valid_state());

    CHECK_FALSE(c_test_silent_check_failure_msg());
    CHECK_FALSE(c_test_silent_check_failure());
    CHECK_FALSE(c_test_silent_check_recovery());
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=silent, src=undefined")
{
    CHECK(c_test_success_silent_check_arg_not_null_ptr());
    CHECK(c_test_success_silent_check_arg_is_valid());
    CHECK(c_test_success_silent_check_arg_array());

    CHECK_FALSE(c_test_trigger_silent_check_arg_not_null_ptr());
    CHECK_FALSE(c_test_trigger_silent_check_arg_is_valid());
    CHECK_FALSE(c_test_trigger_silent_check_arg_array());

    CHECK_FALSE(c_test_silent_check_arg_invalid());
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
static void cpp_test_trigger_silent_assert_not_null_ptr(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_success_silent_assert_not_null_ptr(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_trigger_silent_assert_check(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_success_silent_assert_check(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 0);
}
static void cpp_test_trigger_silent_assert_check_msg(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_success_silent_assert_check_msg(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool cpp_test_trigger_silent_check_not_null_ptr(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_silent_check_not_null_ptr(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_silent_check_valid_state(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_silent_check_valid_state(int arg = 1) noexcept(DIAG_NOEXCEPT)
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
static bool cpp_test_trigger_silent_check_arg_not_null_ptr(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_silent_check_arg_not_null_ptr(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_silent_check_arg_is_valid(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_silent_check_arg_is_valid(int arg = 1) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_trigger_silent_check_arg_array(unsigned len = 7, const char* str = "abc def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_success_silent_check_arg_array(unsigned len = 7, const char* str = "abc-def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_silent_check_arg_invalid(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=silent, src=undefined
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=silent, src=undefined")
{
    CHECK_NOTHROW(cpp_test_success_silent_assert_not_null_ptr());
    CHECK_NOTHROW(cpp_test_success_silent_assert_check());
    CHECK_NOTHROW(cpp_test_success_silent_assert_check_msg());

    CHECK_NOTHROW(cpp_test_trigger_silent_assert_not_null_ptr());
    CHECK_NOTHROW(cpp_test_trigger_silent_assert_check());
    CHECK_NOTHROW(cpp_test_trigger_silent_assert_check_msg());

    CHECK_NOTHROW(cpp_test_silent_assert_failure());
    CHECK_NOTHROW(cpp_test_silent_assert_failure_msg());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=silent, src=undefined")
{
    CHECK(cpp_test_success_silent_check_not_null_ptr());
    CHECK(cpp_test_success_silent_check_valid_state());

    CHECK_FALSE(cpp_test_trigger_silent_check_not_null_ptr());
    CHECK_FALSE(cpp_test_trigger_silent_check_valid_state());

    CHECK_FALSE(cpp_test_silent_check_failure_msg());
    CHECK_FALSE(cpp_test_silent_check_failure());
    CHECK_FALSE(cpp_test_silent_check_recovery());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=silent, src=undefined")
{
    CHECK(cpp_test_success_silent_check_arg_not_null_ptr());
    CHECK(cpp_test_success_silent_check_arg_is_valid());
    CHECK(cpp_test_success_silent_check_arg_array());

    CHECK_FALSE(cpp_test_trigger_silent_check_arg_not_null_ptr());
    CHECK_FALSE(cpp_test_trigger_silent_check_arg_is_valid());
    CHECK_FALSE(cpp_test_trigger_silent_check_arg_array());

    CHECK_FALSE(cpp_test_silent_check_arg_invalid());
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
static void c_test_trigger_steady_assert_not_null_ptr_no_args(const char* ptr = nullptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_success_steady_assert_not_null_ptr_no_args(const char* ptr = "abc")
{
    Assert_NotNullPtr(ptr);
}
static void c_test_trigger_steady_assert_check_no_args(int arg = 0)
{
    Assert_Check(arg == 1);
}
static void c_test_success_steady_assert_check_no_args(int arg = 0)
{
    Assert_Check(arg == 0);
}
static void c_test_trigger_steady_assert_check_msg_no_args(int arg = 0)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_success_steady_assert_check_msg_no_args(int arg = 0)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool c_test_trigger_steady_check_not_null_ptr_no_args(const char* ptr = nullptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_success_steady_check_not_null_ptr_no_args(const char* ptr = "abc")
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_trigger_steady_check_valid_state_no_args(int arg = 0)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_success_steady_check_valid_state_no_args(int arg = 1)
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
static bool c_test_trigger_steady_check_arg_not_null_ptr_no_args(const char* ptr = nullptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_success_steady_check_arg_not_null_ptr_no_args(const char* ptr = "abc")
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_trigger_steady_check_arg_is_valid_no_args(int arg = 0)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_success_steady_check_arg_is_valid_no_args(int arg = 1)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_trigger_steady_check_arg_array_no_args(unsigned len = 7, const char* str = "abc def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_success_steady_check_arg_array_no_args(unsigned len = 7, const char* str = "abc-def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_steady_check_arg_invalid_no_args(int arg = 0)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=steady, src=none
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=steady, src=none")
{
    CHECK_NOTHROW(c_test_success_steady_assert_not_null_ptr_no_args());
    CHECK_NOTHROW(c_test_success_steady_assert_check_no_args());
    CHECK_NOTHROW(c_test_success_steady_assert_check_msg_no_args());

    CHECK_NOTHROW(c_test_trigger_steady_assert_not_null_ptr_no_args());
    CHECK_NOTHROW(c_test_trigger_steady_assert_check_no_args());
    CHECK_NOTHROW(c_test_trigger_steady_assert_check_msg_no_args());

    CHECK_NOTHROW(c_test_steady_assert_failure_no_args());
    CHECK_NOTHROW(c_test_steady_assert_failure_msg_no_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=steady, src=none")
{
    CHECK(c_test_success_steady_check_not_null_ptr_no_args());
    CHECK(c_test_success_steady_check_valid_state_no_args());

    CHECK_FALSE(c_test_trigger_steady_check_not_null_ptr_no_args());
    CHECK_FALSE(c_test_trigger_steady_check_valid_state_no_args());

    CHECK_FALSE(c_test_steady_check_failure_msg_no_args());
    CHECK_FALSE(c_test_steady_check_failure_no_args());
    CHECK_FALSE(c_test_steady_check_recovery_no_args());
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=steady, src=none")
{
    CHECK(c_test_success_steady_check_arg_not_null_ptr_no_args());
    CHECK(c_test_success_steady_check_arg_is_valid_no_args());
    CHECK(c_test_success_steady_check_arg_array_no_args());

    CHECK_FALSE(c_test_trigger_steady_check_arg_not_null_ptr_no_args());
    CHECK_FALSE(c_test_trigger_steady_check_arg_is_valid_no_args());
    CHECK_FALSE(c_test_trigger_steady_check_arg_array_no_args());

    CHECK_FALSE(c_test_steady_check_arg_invalid_no_args());
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
static void c_test_trigger_steady_assert_not_null_ptr_pp_args(const char* ptr = nullptr)
{
    Assert_NotNullPtr(ptr);
}
static void c_test_success_steady_assert_not_null_ptr_pp_args(const char* ptr = "abc")
{
    Assert_NotNullPtr(ptr);
}
static void c_test_trigger_steady_assert_check_pp_args(int arg = 0)
{
    Assert_Check(arg == 1);
}
static void c_test_success_steady_assert_check_pp_args(int arg = 0)
{
    Assert_Check(arg == 0);
}
static void c_test_trigger_steady_assert_check_msg_pp_args(int arg = 0)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void c_test_success_steady_assert_check_msg_pp_args(int arg = 0)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool c_test_trigger_steady_check_not_null_ptr_pp_args(const char* ptr = nullptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_success_steady_check_not_null_ptr_pp_args(const char* ptr = "abc")
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_trigger_steady_check_valid_state_pp_args(int arg = 0)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_success_steady_check_valid_state_pp_args(int arg = 1)
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
static bool c_test_trigger_steady_check_arg_not_null_ptr_pp_args(const char* ptr = nullptr)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_success_steady_check_arg_not_null_ptr_pp_args(const char* ptr = "abc")
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool c_test_trigger_steady_check_arg_is_valid_pp_args(int arg = 0)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_success_steady_check_arg_is_valid_pp_args(int arg = 1)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool c_test_trigger_steady_check_arg_array_pp_args(unsigned len = 7, const char* str = "abc def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_success_steady_check_arg_array_pp_args(unsigned len = 7, const char* str = "abc-def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_steady_check_arg_invalid_pp_args(int arg = 0)
{
    Check_Arg_Invalid(arg, false);
}
}
//
// Test cases: lang=C, mode=steady, src=preprocessor
//
TEST_CASE("general.diagnostics. Asserts, lang=C, mode=steady, src=preprocessor")
{
    CHECK_NOTHROW(c_test_success_steady_assert_not_null_ptr_pp_args());
    CHECK_NOTHROW(c_test_success_steady_assert_check_pp_args());
    CHECK_NOTHROW(c_test_success_steady_assert_check_msg_pp_args());

    CHECK_NOTHROW(c_test_trigger_steady_assert_not_null_ptr_pp_args());
    CHECK_NOTHROW(c_test_trigger_steady_assert_check_pp_args());
    CHECK_NOTHROW(c_test_trigger_steady_assert_check_msg_pp_args());

    CHECK_NOTHROW(c_test_steady_assert_failure_pp_args());
    CHECK_NOTHROW(c_test_steady_assert_failure_msg_pp_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C, mode=steady, src=preprocessor")
{
    CHECK(c_test_success_steady_check_not_null_ptr_pp_args());
    CHECK(c_test_success_steady_check_valid_state_pp_args());

    CHECK_FALSE(c_test_trigger_steady_check_not_null_ptr_pp_args());
    CHECK_FALSE(c_test_trigger_steady_check_valid_state_pp_args());

    CHECK_FALSE(c_test_steady_check_failure_msg_pp_args());
    CHECK_FALSE(c_test_steady_check_failure_pp_args());
    CHECK_FALSE(c_test_steady_check_recovery_pp_args());
}
TEST_CASE("general.diagnostics. Args, lang=C, mode=steady, src=preprocessor")
{
    CHECK(c_test_success_steady_check_arg_not_null_ptr_pp_args());
    CHECK(c_test_success_steady_check_arg_is_valid_pp_args());
    CHECK(c_test_success_steady_check_arg_array_pp_args());

    CHECK_FALSE(c_test_trigger_steady_check_arg_not_null_ptr_pp_args());
    CHECK_FALSE(c_test_trigger_steady_check_arg_is_valid_pp_args());
    CHECK_FALSE(c_test_trigger_steady_check_arg_array_pp_args());

    CHECK_FALSE(c_test_steady_check_arg_invalid_pp_args());
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
static void cpp_test_trigger_steady_assert_not_null_ptr_no_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_success_steady_assert_not_null_ptr_no_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_trigger_steady_assert_check_no_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_success_steady_assert_check_no_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 0);
}
static void cpp_test_trigger_steady_assert_check_msg_no_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_success_steady_assert_check_msg_no_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool cpp_test_trigger_steady_check_not_null_ptr_no_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_steady_check_not_null_ptr_no_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_steady_check_valid_state_no_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_steady_check_valid_state_no_args(int arg = 1) noexcept(DIAG_NOEXCEPT)
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
static bool cpp_test_trigger_steady_check_arg_not_null_ptr_no_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_steady_check_arg_not_null_ptr_no_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_steady_check_arg_is_valid_no_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_steady_check_arg_is_valid_no_args(int arg = 1) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_trigger_steady_check_arg_array_no_args(unsigned len = 7, const char* str = "abc def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_success_steady_check_arg_array_no_args(unsigned len = 7, const char* str = "abc-def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_steady_check_arg_invalid_no_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=steady, src=none
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=steady, src=none")
{
    CHECK_NOTHROW(cpp_test_success_steady_assert_not_null_ptr_no_args());
    CHECK_NOTHROW(cpp_test_success_steady_assert_check_no_args());
    CHECK_NOTHROW(cpp_test_success_steady_assert_check_msg_no_args());

    CHECK_NOTHROW(cpp_test_trigger_steady_assert_not_null_ptr_no_args());
    CHECK_NOTHROW(cpp_test_trigger_steady_assert_check_no_args());
    CHECK_NOTHROW(cpp_test_trigger_steady_assert_check_msg_no_args());

    CHECK_NOTHROW(cpp_test_steady_assert_failure_no_args());
    CHECK_NOTHROW(cpp_test_steady_assert_failure_msg_no_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=steady, src=none")
{
    CHECK(cpp_test_success_steady_check_not_null_ptr_no_args());
    CHECK(cpp_test_success_steady_check_valid_state_no_args());

    CHECK_FALSE(cpp_test_trigger_steady_check_not_null_ptr_no_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_valid_state_no_args());

    CHECK_FALSE(cpp_test_steady_check_failure_msg_no_args());
    CHECK_FALSE(cpp_test_steady_check_failure_no_args());
    CHECK_FALSE(cpp_test_steady_check_recovery_no_args());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=steady, src=none")
{
    CHECK(cpp_test_success_steady_check_arg_not_null_ptr_no_args());
    CHECK(cpp_test_success_steady_check_arg_is_valid_no_args());
    CHECK(cpp_test_success_steady_check_arg_array_no_args());

    CHECK_FALSE(cpp_test_trigger_steady_check_arg_not_null_ptr_no_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_arg_is_valid_no_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_arg_array_no_args());

    CHECK_FALSE(cpp_test_steady_check_arg_invalid_no_args());
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
static void cpp_test_trigger_steady_assert_not_null_ptr_pp_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_success_steady_assert_not_null_ptr_pp_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_trigger_steady_assert_check_pp_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_success_steady_assert_check_pp_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 0);
}
static void cpp_test_trigger_steady_assert_check_msg_pp_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_success_steady_assert_check_msg_pp_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool cpp_test_trigger_steady_check_not_null_ptr_pp_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_steady_check_not_null_ptr_pp_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_steady_check_valid_state_pp_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_steady_check_valid_state_pp_args(int arg = 1) noexcept(DIAG_NOEXCEPT)
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
static bool cpp_test_trigger_steady_check_arg_not_null_ptr_pp_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_steady_check_arg_not_null_ptr_pp_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_steady_check_arg_is_valid_pp_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_steady_check_arg_is_valid_pp_args(int arg = 1) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_trigger_steady_check_arg_array_pp_args(unsigned len = 7, const char* str = "abc def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_success_steady_check_arg_array_pp_args(unsigned len = 7, const char* str = "abc-def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_steady_check_arg_invalid_pp_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Invalid(arg, false);
}
//
// Test cases: lang=C++, mode=steady, src=preprocessor
//
TEST_CASE("general.diagnostics. Asserts, lang=C++, mode=steady, src=preprocessor")
{
    CHECK_NOTHROW(cpp_test_success_steady_assert_not_null_ptr_pp_args());
    CHECK_NOTHROW(cpp_test_success_steady_assert_check_pp_args());
    CHECK_NOTHROW(cpp_test_success_steady_assert_check_msg_pp_args());

    CHECK_NOTHROW(cpp_test_trigger_steady_assert_not_null_ptr_pp_args());
    CHECK_NOTHROW(cpp_test_trigger_steady_assert_check_pp_args());
    CHECK_NOTHROW(cpp_test_trigger_steady_assert_check_msg_pp_args());

    CHECK_NOTHROW(cpp_test_steady_assert_failure_pp_args());
    CHECK_NOTHROW(cpp_test_steady_assert_failure_msg_pp_args());
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=steady, src=preprocessor")
{
    CHECK(cpp_test_success_steady_check_not_null_ptr_pp_args());
    CHECK(cpp_test_success_steady_check_valid_state_pp_args());

    CHECK_FALSE(cpp_test_trigger_steady_check_not_null_ptr_pp_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_valid_state_pp_args());

    CHECK_FALSE(cpp_test_steady_check_failure_msg_pp_args());
    CHECK_FALSE(cpp_test_steady_check_failure_pp_args());
    CHECK_FALSE(cpp_test_steady_check_recovery_pp_args());
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=steady, src=preprocessor")
{
    CHECK(cpp_test_success_steady_check_arg_not_null_ptr_pp_args());
    CHECK(cpp_test_success_steady_check_arg_is_valid_pp_args());
    CHECK(cpp_test_success_steady_check_arg_array_pp_args());

    CHECK_FALSE(cpp_test_trigger_steady_check_arg_not_null_ptr_pp_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_arg_is_valid_pp_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_arg_array_pp_args());

    CHECK_FALSE(cpp_test_steady_check_arg_invalid_pp_args());
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
static void cpp_test_trigger_steady_assert_not_null_ptr_sl_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_success_steady_assert_not_null_ptr_sl_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_trigger_steady_assert_check_sl_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_success_steady_assert_check_sl_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 0);
}
static void cpp_test_trigger_steady_assert_check_msg_sl_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_success_steady_assert_check_msg_sl_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool cpp_test_trigger_steady_check_not_null_ptr_sl_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_steady_check_not_null_ptr_sl_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_steady_check_valid_state_sl_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_steady_check_valid_state_sl_args(int arg = 1) noexcept(DIAG_NOEXCEPT)
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
static bool cpp_test_trigger_steady_check_arg_not_null_ptr_sl_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_steady_check_arg_not_null_ptr_sl_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_steady_check_arg_is_valid_sl_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_steady_check_arg_is_valid_sl_args(int arg = 1) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_trigger_steady_check_arg_array_sl_args(unsigned len = 7, const char* str = "abc def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_success_steady_check_arg_array_sl_args(unsigned len = 7, const char* str = "abc-def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_steady_check_arg_invalid_sl_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
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
    CHECK_NOTHROW(cpp_test_success_steady_assert_not_null_ptr_sl_args());
    CHECK_NOTHROW(cpp_test_success_steady_assert_check_sl_args());
    CHECK_NOTHROW(cpp_test_success_steady_assert_check_msg_sl_args());

    CHECK_NOTHROW(cpp_test_trigger_steady_assert_not_null_ptr_sl_args());
    CHECK_NOTHROW(cpp_test_trigger_steady_assert_check_sl_args());
    CHECK_NOTHROW(cpp_test_trigger_steady_assert_check_msg_sl_args());

    CHECK_NOTHROW(cpp_test_steady_assert_failure_sl_args());
    CHECK_NOTHROW(cpp_test_steady_assert_failure_msg_sl_args());
#endif
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=steady, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK(cpp_test_success_steady_check_not_null_ptr_sl_args());
    CHECK(cpp_test_success_steady_check_valid_state_sl_args());

    CHECK_FALSE(cpp_test_trigger_steady_check_not_null_ptr_sl_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_valid_state_sl_args());

    CHECK_FALSE(cpp_test_steady_check_failure_msg_sl_args());
    CHECK_FALSE(cpp_test_steady_check_failure_sl_args());
    CHECK_FALSE(cpp_test_steady_check_recovery_sl_args());
#endif
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=steady, src=std_source_location")
{
#ifdef __cpp_lib_source_location
    CHECK(cpp_test_success_steady_check_arg_not_null_ptr_sl_args());
    CHECK(cpp_test_success_steady_check_arg_is_valid_sl_args());
    CHECK(cpp_test_success_steady_check_arg_array_sl_args());

    CHECK_FALSE(cpp_test_trigger_steady_check_arg_not_null_ptr_sl_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_arg_is_valid_sl_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_arg_array_sl_args());

    CHECK_FALSE(cpp_test_steady_check_arg_invalid_sl_args());
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
static void cpp_test_trigger_steady_assert_not_null_ptr_st_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_success_steady_assert_not_null_ptr_st_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
}
static void cpp_test_trigger_steady_assert_check_st_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
}
static void cpp_test_success_steady_assert_check_st_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 0);
}
static void cpp_test_trigger_steady_assert_check_msg_st_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
}
static void cpp_test_success_steady_assert_check_msg_st_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
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
static bool cpp_test_trigger_steady_check_not_null_ptr_st_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_steady_check_not_null_ptr_st_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_steady_check_valid_state_st_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_steady_check_valid_state_st_args(int arg = 1) noexcept(DIAG_NOEXCEPT)
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
static bool cpp_test_trigger_steady_check_arg_not_null_ptr_st_args(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_success_steady_check_arg_not_null_ptr_st_args(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr, false);
    return true;
}
static bool cpp_test_trigger_steady_check_arg_is_valid_st_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_success_steady_check_arg_is_valid_st_args(int arg = 1) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(arg == 1, false);
    return true;
}
static bool cpp_test_trigger_steady_check_arg_array_st_args(unsigned len = 7, const char* str = "abc def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_success_steady_check_arg_array_st_args(unsigned len = 7, const char* str = "abc-def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_steady_check_arg_invalid_st_args(int arg = 0) noexcept(DIAG_NOEXCEPT)
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
    CHECK_NOTHROW(cpp_test_success_steady_assert_not_null_ptr_st_args());
    CHECK_NOTHROW(cpp_test_success_steady_assert_check_st_args());
    CHECK_NOTHROW(cpp_test_success_steady_assert_check_msg_st_args());

    CHECK_NOTHROW(cpp_test_trigger_steady_assert_not_null_ptr_st_args());
    CHECK_NOTHROW(cpp_test_trigger_steady_assert_check_st_args());
    CHECK_NOTHROW(cpp_test_trigger_steady_assert_check_msg_st_args());

    CHECK_NOTHROW(cpp_test_steady_assert_failure_st_args());
    CHECK_NOTHROW(cpp_test_steady_assert_failure_msg_st_args());
#endif
}
TEST_CASE("general.diagnostics. Checks, lang=C++, mode=steady, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK(cpp_test_success_steady_check_not_null_ptr_st_args());
    CHECK(cpp_test_success_steady_check_valid_state_st_args());

    CHECK_FALSE(cpp_test_trigger_steady_check_not_null_ptr_st_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_valid_state_st_args());

    CHECK_FALSE(cpp_test_steady_check_failure_msg_st_args());
    CHECK_FALSE(cpp_test_steady_check_failure_st_args());
    CHECK_FALSE(cpp_test_steady_check_recovery_st_args());
#endif
}
TEST_CASE("general.diagnostics. Args, lang=C++, mode=steady, src=std_stacktrace")
{
#ifdef __cpp_lib_stacktrace
    CHECK(cpp_test_success_steady_check_arg_not_null_ptr_st_args());
    CHECK(cpp_test_success_steady_check_arg_is_valid_st_args());
    CHECK(cpp_test_success_steady_check_arg_array_st_args());

    CHECK_FALSE(cpp_test_trigger_steady_check_arg_not_null_ptr_st_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_arg_is_valid_st_args());
    CHECK_FALSE(cpp_test_trigger_steady_check_arg_array_st_args());

    CHECK_FALSE(cpp_test_steady_check_arg_invalid_st_args());
#endif
}
