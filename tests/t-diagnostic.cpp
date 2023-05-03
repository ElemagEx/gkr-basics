#include <catch2/catch_test_macros.hpp>

#define DIAG_UNIT_TESTING
#include <gkr/diag/diagnostics.h>

#include <exception>

void my_cpp_halt_proc_no_args(int id, const char* msg)
{
    throw std::exception();
}
int my_cpp_stop_proc_no_args(int id, const char* msg)
{
    throw std::exception();
}
int my_cpp_warn_proc_no_args(int id, const char* msg)
{
    return 1;
}

#ifdef __cpp_lib_stacktrace
#include <source_location>
void my_halt_proc_sl_args(int id, const char* msg, const std::source_location& location)
{
}
#endif
#ifdef __cpp_lib_source_location
#include <stacktrace>
void my_halt_proc_sl_args(int id, const char* msg, const std::stacktrace& stacktrace)
{
}
#endif

#include <gkr/diag/undefines.h>
#define DIAG_EXTERNAL_API
#define DIAG_HALT           my_cpp_halt_proc_no_args
#define DIAG_STOP           my_cpp_stop_proc_no_args
#define DIAG_WARN           my_cpp_warn_proc_no_args
#define DIAG_NOEXCEPT       false
#define DIAG_SRC_INFO       DIAG_SRC_INFO_DISABLED
#define DIAG_MODE           DIAG_MODE_DISABLED
#include <gkr/diag/diagnostics.h>

int cpp_test_trigger_disabled_assert_not_null_ptr(const char* ptr = nullptr) noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr);
    return 1;
}
int cpp_test_success_disabled_assert_not_null_ptr(const char* ptr = "abc") noexcept(DIAG_NOEXCEPT)
{
    Check_NotNullPtr(ptr);
    return 1;
}
int cpp_test_trigger_disabled_assert_check(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 1);
    return 1;
}
int cpp_test_success_disabled_assert_check(int arg = 0) noexcept(DIAG_NOEXCEPT)
{
    Assert_Check(arg == 0);
    return 1;
}

TEST_CASE("general.diagnostics. lang=C++, mode=disabled, src=nevermind")
{
    CHECK_NOTHROW(cpp_test_trigger_disabled_assert_not_null_ptr());
    CHECK_NOTHROW(cpp_test_success_disabled_assert_not_null_ptr());

    CHECK_NOTHROW(cpp_test_trigger_disabled_assert_check());
    CHECK_NOTHROW(cpp_test_success_disabled_assert_check());
}
