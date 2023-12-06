#include <gkr/container/raw_buffer.hpp>

#include <catch2/catch_test_macros.hpp>

TEST_CASE("container.raw_buffer. main")
{
    gkr::raw_buffer_t b1;

    b1.reserve(256);

    b1.change_size(16);

    b1.data();
}
