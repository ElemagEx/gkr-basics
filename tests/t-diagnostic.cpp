#include <gkr/diag/diagnostics.h>

#include <catch2/catch_test_macros.hpp>

void foo() noexcept
{
}

TEST_CASE("A")
{
    CHECK_NOTHROW(foo());
}
