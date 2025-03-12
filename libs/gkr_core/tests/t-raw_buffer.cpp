#include <gkr/defs.hpp>

#include <gkr/container/raw_buffer.hpp>

#include <catch2/catch_test_macros.hpp>
#include <catch2/catch_template_test_macros.hpp>

#include <gkr/testing/allocator.hpp>

using flag = gkr::testing::allocator_flag;

DEFINE_TEST_ALLOCATOR(allocator1, (flag::SingleThreaded | flag::EqualsAlways  | flag::PropagatesNever ));
DEFINE_TEST_ALLOCATOR(allocator2, (flag::SingleThreaded | flag::EqualsAlways  | flag::PropagatesAlways));
DEFINE_TEST_ALLOCATOR(allocator3, (flag::SingleThreaded | flag::EqualsByValue | flag::PropagatesNever ));
DEFINE_TEST_ALLOCATOR(allocator4, (flag::SingleThreaded | flag::EqualsNever   | flag::PropagatesNever ));

DEFINE_TEST_ALLOCATOR(allocatorM, (flag::MultiThreaded  | flag::EqualsAlways  | flag::PropagatesNever ));

struct X
{
    char str[32];
};

using allocator1_0_t = allocator1<std::max_align_t>;
using allocator1_1_t = allocator1<char>;
using allocator1_2_t = allocator1<short>;
using allocator1_4_t = allocator1<int>;
using allocator1_8_t = allocator1<long long>;
using allocator1_p_t = allocator1<std::size_t>;
using allocator1_x_t = allocator1<struct X>;

//using allocator1_t = allocator1<char>;
//using allocator2_t = allocator1<char>;
//using allocator3_t = allocator1<char>;
//using allocator4_t = allocator1<char>;
//using allocatorM_t = allocator1<char>;

template<class Allocator>
void raw_buffer_test_reserve()
{
    using namespace gkr;

    using value_t = typename Allocator::value_type;

    constexpr const char* sample = "0123456789ABCDEF";

    const std::size_t len = std::min(std::strlen(sample), sizeof(value_t));

    gkr::testing::get_singlethreaded_pre_allocated_storage<value_t>().reset(1+16+32, true);
    {
        basic_raw_buffer<Allocator> buff;

        CHECK(buff.capacity() == 0);
        CHECK(buff.size    () == 0);
        CHECK(buff.data    () == nullptr);

        CHECK_NOTHROW(buff.reserve(1));
        CHECK(buff.capacity() == sizeof(value_t));
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

        constexpr std::size_t TOO_LARGE_SIZE_TO_ALLOC = std::size_t(1) << ((sizeof(void*)*8) - 1);

        CHECK_THROWS_AS(buff.reserve(TOO_LARGE_SIZE_TO_ALLOC), std::bad_alloc);
    }
    gkr::testing::get_singlethreaded_pre_allocated_storage<value_t>().reset();
};

TEST_CASE("container.raw_buffer. reserve")
{
    SECTION("With std::max_align_t value type allocator")
    {
        raw_buffer_test_reserve<allocator1_0_t>();
    }
    SECTION("With char value type allocator")
    {
        raw_buffer_test_reserve<allocator1_1_t>();
    }
    SECTION("With short value type allocator")
    {
        raw_buffer_test_reserve<allocator1_2_t>();
    }
    SECTION("With int value type allocator")
    {
        raw_buffer_test_reserve<allocator1_4_t>();
    }
    SECTION("With long long value type allocator")
    {
        raw_buffer_test_reserve<allocator1_8_t>();
    }
    SECTION("With std::size_t value type allocator")
    {
        raw_buffer_test_reserve<allocator1_p_t>();
    }
    SECTION("With struct X value type allocator")
    {
        raw_buffer_test_reserve<allocator1_x_t>();
    }
}
TEST_CASE("container.raw_buffer. clear")
{
}
