#include <gkr/container/raw_buffer.h>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("container.raw_buffer. main")
{
    gkr::raw_buffer b1;

    b1.reserve(1024);
}
