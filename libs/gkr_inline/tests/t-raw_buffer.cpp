#include <gkr/defs.hpp>

#include <gkr/container/raw_buffer.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <gkr/testing/allocator.hpp>

using flag = gkr::testing::allocator_flag;
using objects_ref_counting = gkr::testing::objects_ref_counting;

struct X
{
    char str[32];
};

template<typename Type>
void raw_buffer_test_reserve()
{
    using Allocator = gkr::testing::ref_counting_allocator<Type>;

    constexpr const char* sample = "0123456789ABCDEF";

    const std::size_t len = std::min(std::strlen(sample), sizeof(Type));

    gkr::basic_raw_buffer<Allocator> buff;

    CHECK(buff.capacity() == 0);
    CHECK(buff.size    () == 0);
    CHECK(buff.data    () == nullptr);

    CHECK_NOTHROW(buff.reserve(1));
    CHECK(buff.capacity() == sizeof(Type));
    CHECK(buff.size    () == 0);
    CHECK(buff.data    () != nullptr);

    REQUIRE(buff.change_size(len));
    std::memcpy(buff.data(), sample, buff.size());

    CHECK_NOTHROW(buff.reserve(16));
    CHECK(buff.capacity() >= 16);
    CHECK(buff.size    () == len);
    CHECK(buff.data    () != nullptr);
    CHECK_FALSE(std::memcmp(buff.data(), sample, buff.size()));

    REQUIRE(buff.change_size(16));
    std::memcpy(buff.data(), sample, 16);

    CHECK_NOTHROW(buff.reserve(32));
    CHECK(buff.capacity() >= 32);
    CHECK(buff.size    () == 16);
    CHECK(buff.data    () != nullptr);
    CHECK_FALSE(std::memcmp(buff.data(), sample, buff.size()));

    CHECK_NOTHROW(buff.reserve(1));
    CHECK(buff.capacity() >= 32);
    CHECK(buff.size    () == 16);
    CHECK(buff.data    () != nullptr);
    CHECK_FALSE(std::memcmp(buff.data(), sample, buff.size()));

    std::size_t TOO_LARGE_SIZE_TO_ALLOC = std::allocator_traits<Allocator>::max_size(buff.get_allocator()) / 2;

    CHECK_THROWS_AS(buff.reserve(TOO_LARGE_SIZE_TO_ALLOC), std::bad_alloc);
};

TEST_CASE("container.raw_buffer. reserve")
{
    CHECK(objects_ref_counting::check());

    SECTION("With std::max_align_t value type allocator")
    {
        raw_buffer_test_reserve<std::max_align_t>();
    }
    SECTION("With char value type allocator")
    {
        raw_buffer_test_reserve<char>();
    }
    SECTION("With short value type allocator")
    {
        raw_buffer_test_reserve<short>();
    }
    SECTION("With int value type allocator")
    {
        raw_buffer_test_reserve<int>();
    }
    SECTION("With long long value type allocator")
    {
        raw_buffer_test_reserve<long long>();
    }
    SECTION("With std::size_t value type allocator")
    {
        raw_buffer_test_reserve<std::size_t>();
    }
    SECTION("With struct X value type allocator")
    {
        raw_buffer_test_reserve<struct X>();
    }

    CHECK(objects_ref_counting::check());
}
TEST_CASE("container.raw_buffer. clear")
{
}
