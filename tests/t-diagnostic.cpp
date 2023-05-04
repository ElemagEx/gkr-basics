#include <catch2/catch_test_macros.hpp>

#define DIAG_UNIT_TESTING
#include <gkr/diag/diagnostics.h>

#include <exception>

#define TEST_ASSERT_CHECK_MESSAGE   "Test Assert Check Message"
#define TEST_ASSERT_FAILURE_MESSAGE "Test Assert Failure Message"
#define TEST_CHECK_FAILURE_MESSAGE  "Test Check Failure Message"
#define TEST_CHECK_RECOVERY_MESSAGE "Test Check Recovery Message"

/*static*/ void my_cpp_halt_proc_no_args(int id, const char* msg)
{
    throw std::exception();
}
/*static*/ int my_cpp_stop_proc_no_args(int id, const char* msg)
{
    throw std::exception();
}
/*static*/ int my_cpp_warn_proc_no_args(int id, const char* msg)
{
    return 1;
}

#ifdef __cpp_lib_source_location
#include <source_location>
/*static*/ int my_halt_proc_sl_args(int id, const char* msg, const std::source_location& location)
{
}
#endif
#ifdef __cpp_lib_stacktrace
#include <stacktrace>
/*static*/ void my_halt_proc_st_args(int id, const char* msg, const std::stacktrace& stacktrace)
{
}
#endif
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
static bool cpp_test_trigger_disabled_assert_not_null_ptr(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
    return true;
}
static bool cpp_test_success_disabled_assert_not_null_ptr(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Assert_NotNullPtr(ptr);
    return true;
}
static bool cpp_test_trigger_disabled_assert_check(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
    return true;
}
static bool cpp_test_success_disabled_assert_check(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 0);
    return true;
}
static bool cpp_test_trigger_disabled_assert_check_msg(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
    return true;
}
static bool cpp_test_success_disabled_assert_check_msg(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
    return true;
}
static bool cpp_test_disabled_assert_failure() noexcept(DIAG_NOEXCEPT)
{
    Assert_Failure();
    return true;
}
static bool cpp_test_disabled_assert_failure_msg() noexcept(DIAG_NOEXCEPT)
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
    return true;
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
static bool cpp_test_trigger_disabled_check_arg_array(unsigned len = 7, const char* str = "abc-def") noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool cpp_test_success_disabled_check_arg_array(unsigned len = 7, const char* str = "abc def") noexcept(DIAG_NOEXCEPT)
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
    CHECK(cpp_test_success_disabled_assert_not_null_ptr());
    CHECK(cpp_test_success_disabled_assert_check());
    CHECK(cpp_test_success_disabled_assert_check_msg());

    CHECK(cpp_test_trigger_disabled_assert_not_null_ptr());
    CHECK(cpp_test_trigger_disabled_assert_check());
    CHECK(cpp_test_trigger_disabled_assert_check_msg());

    CHECK(cpp_test_disabled_assert_failure());
    CHECK(cpp_test_disabled_assert_failure_msg());
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
static bool c_test_trigger_disabled_assert_not_null_ptr(const char* ptr = nullptr)
{
    Assert_NotNullPtr(ptr);
    return true;
}
static bool c_test_success_disabled_assert_not_null_ptr(const char* ptr = "abc")
{
    Assert_NotNullPtr(ptr);
    return true;
}
static bool c_test_trigger_disabled_assert_check(int arg = 0)
{
    Assert_Check(arg == 1);
    return true;
}
static bool c_test_success_disabled_assert_check(int arg = 0)
{
    Assert_Check(arg == 0);
    return true;
}
static bool c_test_trigger_disabled_assert_check_msg(int arg = 0)
{
    Assert_CheckMsg(arg == 1, TEST_ASSERT_CHECK_MESSAGE);
    return true;
}
static bool c_test_success_disabled_assert_check_msg(int arg = 0)
{
    Assert_CheckMsg(arg == 0, TEST_ASSERT_CHECK_MESSAGE);
    return true;
}
static bool c_test_disabled_assert_failure()
{
    Assert_Failure();
    return true;
}
static bool c_test_disabled_assert_failure_msg()
{
    Assert_FailureMsg(TEST_ASSERT_FAILURE_MESSAGE);
    return true;
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
static bool c_test_trigger_disabled_check_arg_array(unsigned len = 7, const char* str = "abc-def")
{
    Check_Arg_Array(pos, len, str[pos] != ' ', false);
    return true;
}
static bool c_test_success_disabled_check_arg_array(unsigned len = 7, const char* str = "abc def")
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
    CHECK(c_test_success_disabled_assert_not_null_ptr());
    CHECK(c_test_success_disabled_assert_check());
    CHECK(c_test_success_disabled_assert_check_msg());

    CHECK(c_test_trigger_disabled_assert_not_null_ptr());
    CHECK(c_test_trigger_disabled_assert_check());
    CHECK(c_test_trigger_disabled_assert_check_msg());

    CHECK(c_test_disabled_assert_failure());
    CHECK(c_test_disabled_assert_failure_msg());
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
