#include <catch2/catch_template_test_macros.hpp>

#include <gkr/lockfree_grow_only_bag.h>

#include <gkr/testing/allocator.h>
#include <string>
#include <thread>

struct Data
{
    Data() noexcept = default;
   ~Data() noexcept = default;

    Data           (const Data&) noexcept = default;
    Data& operator=(const Data&) noexcept = default;

    Data(int a_, bool b_ = false) noexcept : a(a_), b(b_)
    {
    }
    Data(Data&& other) noexcept : a(std::exchange(other.a, 0)), b(std::exchange(other.b, false))
    {
    }
    template<typename... Args>
    Data(Args&&... args)
    {
        int values[sizeof...(args)] = {args...};
        for(int value : values) a += value;
    }
    Data& operator=(Data&& other) noexcept
    {
        a = std::exchange(other.a, 0);
        b = std::exchange(other.b, false);
        return *this;
    }
    Data& operator+=(const Data& other) noexcept
    {
        a += other.a;
        return *this;
    }
    explicit operator int() const
    {
        return a;
    }
    int  a = 0;
    bool b = false;
};

using flag = gkr::testing::allocator_flag;

template<typename T, typename Allocator>
int sum(const gkr::lockfree_grow_only_bag<T, Allocator>& bag)
{
    T total {0};

    for(auto& item : bag)
    {
        total += item;
    }
    return int(total);
}

DEFINE_TEST_ALLOCATOR(allocator1, (flag::EqualsAlways  | flag::PropagatesNever ));
DEFINE_TEST_ALLOCATOR(allocator2, (flag::EqualsAlways  | flag::PropagatesAlways));
DEFINE_TEST_ALLOCATOR(allocator3, (flag::EqualsByValue | flag::PropagatesNever ));
DEFINE_TEST_ALLOCATOR(allocator4, (flag::EqualsNever   | flag::PropagatesNever ));

template<typename T>
using allocator0 = std::allocator<T>;

using std_string = std::string;

#ifdef _MSC_VER
#pragma warning(disable:4868)
#endif

TEMPLATE_PRODUCT_TEST_CASE("container.lockfree_bag. Lifecycle", "", (allocator0, allocator1, allocator2, allocator3, allocator4), (int, Data))
{
    using allocator_t = TestType;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_grow_only_bag<value_t, allocator_t>;
    using slot_t = typename bag_t::allocator_value_type;

    gkr::testing::get_this_thread_preallocated_storage<slot_t>().reset(16);
    {
        bag_t bag1;

        CHECK(bag1.empty());

        bag1.add(value_t(1));
        bag1.add(value_t(2));
        bag1.add(value_t(3));

        CHECK(!bag1.empty());
        CHECK(bag1.size() == 3);
        CHECK(sum(bag1) == 6);

        SECTION("copy construction")
        {
            bag_t bag2(bag1);

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 3);
            CHECK(sum(bag2) == 6);

            CHECK(!bag1.empty());
            CHECK(bag1.size() == 3);
            CHECK(sum(bag1) == 6);
        }
        SECTION("copy assignment")
        {
            bag_t bag2;

            bag2.add(value_t(10));
            bag2.add(value_t(20));

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 2);
            CHECK(sum(bag2) == 30);

            bag2 = bag1;

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 3);
            CHECK(sum(bag2) == 6);

            CHECK(!bag1.empty());
            CHECK(bag1.size() == 3);
            CHECK(sum(bag1) == 6);
        }
        SECTION("move construction")
        {
            bag_t bag2(std::move(bag1));

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 3);
            CHECK(sum(bag2) == 6);

            CHECK(bag1.empty());
            CHECK(bag1.size() == 0);
            CHECK(sum(bag1) == 0);
        }
        SECTION("move assignment")
        {
            bag_t bag2;

            bag2.add(value_t(10));
            bag2.add(value_t(20));

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 2);
            CHECK(sum(bag2) == 30);

            bag2 = std::move(bag1);

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 3);
            CHECK(sum(bag2) == 6);

            CHECK(bag1.empty());
            CHECK(bag1.size() == 0);
            CHECK(sum(bag1) == 0);
        }
        SECTION("swap")
        {
            bag_t bag2;

            bag2.add(value_t(10));
            bag2.add(value_t(20));

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 2);
            CHECK(sum(bag2) == 30);

            std::swap(bag1, bag2);

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 3);
            CHECK(sum(bag2) == 6);

            CHECK(!bag1.empty());
            CHECK(bag1.size() == 2);
            CHECK(sum(bag1) == 30);
        }
    }
    gkr::testing::get_this_thread_preallocated_storage<slot_t>().reset();
}

TEMPLATE_PRODUCT_TEST_CASE("container.lockfree_bag. Clear", "", (allocator0, allocator1), (int, Data, std_string))
{
    using value_t = TestType;

    using bag_t  = gkr::lockfree_grow_only_bag<value_t, allocator1<value_t>>;
    using slot_t = typename bag_t::allocator_value_type;

    gkr::testing::get_this_thread_preallocated_storage<slot_t>().reset(8);
    {
        bag_t bag;

        CHECK(bag.empty());

        bag.add();
        bag.add();
        bag.add();

        CHECK(!bag.empty());
        CHECK(bag.size() == 3);

        bag.clear();

        CHECK(bag.empty());
        CHECK(bag.size() == 0);
    }
    gkr::testing::get_this_thread_preallocated_storage<slot_t>().reset();
}

TEST_CASE("container.lockfree_bag. Add")
{
    using bag_t  = gkr::lockfree_grow_only_bag<Data, allocator1<Data>>;
    using slot_t = typename bag_t::allocator_value_type;

    gkr::testing::get_this_thread_preallocated_storage<slot_t>().reset(8);
    {
        bag_t bag;

        CHECK(bag.empty());
        CHECK(bag.size() == 0);

        SECTION("default construct")
        {
            bag.add().a = 1;
            bag.add().a = 2;
            bag.add().a = 3;
        }
        SECTION("move construct")
        {
            bag.add({1});
            bag.add({2});
            bag.add({3});
        }
        SECTION("copy construct")
        {
            Data d;
            bag.add(d).a = 1;
            bag.add(d).a = 2;
            bag.add(d).a = 3;
        }
        SECTION("emplace")
        {
            bag.add(Data(-10, 1, 5, 5));
            bag.add(Data(-20, 2, 3, 7, 4, 6));
            bag.add(Data(-30, 3, 1, 9, 2, 8, 0, 10));
        }

        CHECK(!bag.empty());
        CHECK(bag.size() == 3);
        CHECK(sum(bag) == 6);
    }
    gkr::testing::get_this_thread_preallocated_storage<slot_t>().reset();
}

TEST_CASE("container.lockfree_bag. Multithreading")
{
    using bag_t  = gkr::lockfree_grow_only_bag<Data, allocator0<Data>>;
    using slot_t = typename bag_t::allocator_value_type;

    constexpr std::size_t threads_count = 20;
    constexpr std::size_t nodes_to_add  = 500;

    {
        bag_t bag;

        std::thread threads[threads_count];

        for(std::thread& thread : threads)
        {
            thread = std::move(std::thread([&bag] () {
                for(std::size_t index = 0; index < nodes_to_add; ++index)
                {
                    bag.add();
                }
            }));
        }
        for(std::thread& thread : threads)
        {
            thread.join();
        }

        CHECK(bag.size() == (threads_count * nodes_to_add));
    }
}
