#include <gkr/diag/diagnostics.h>

#include <catch2/catch_test_macros.hpp>

void foo() noexcept
{
}

TEST_CASE("general.diagnostics. main")
{
    CHECK_NOTHROW(foo());
}
