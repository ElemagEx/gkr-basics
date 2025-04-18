#include <gkr/defs.hpp>

#include <gkr/container/lockfree_bag.hpp>

#include <catch2/catch_template_test_macros.hpp>

#include <gkr/testing/allocator.hpp>

#include <string>
#include <thread>
#include <atomic>

struct Data
{
    Data() noexcept = default;
   ~Data() noexcept = default;

    Data           (const Data&) noexcept = default;
    Data& operator=(const Data&) noexcept = default;

    explicit Data(int a_, bool b_ = false) noexcept : a(a_), b(b_)
    {
    }
    Data(Data&& other) noexcept : a(std::exchange(other.a, 0)), b(std::exchange(other.b, false))
    {
    }
    Data& operator=(Data&& other) noexcept
    {
        a = std::exchange(other.a, 0);
        b = std::exchange(other.b, false);
        return *this;
    }
    template<typename... Args>
    explicit Data(Args&&... args) noexcept
    {
        int values[sizeof...(args)] = {args...};
        for(int value : values) a += value;
    }
    bool operator==(const Data& other) const noexcept
    {
        return (a == other.a) && (b == other.b);
    }
    bool operator!=(const Data& other) const noexcept
    {
        return (a != other.a) || (b != other.b);
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

std::ostream& operator<<(std::ostream& os, const Data& value);
std::ostream& operator<<(std::ostream& os, const Data& value)
{
    os << "Data(" << value.a << "," << std::boolalpha << value.b << ")";
    return os;
}

using flag = gkr::testing::allocator_flag;
using objects_ref_counting = gkr::testing::objects_ref_counting;

DEFINE_TEST_ALLOCATOR(allocator1, (flag::SingleThreaded | flag::EqualsAlways  | flag::PropagatesNever ));
DEFINE_TEST_ALLOCATOR(allocator2, (flag::SingleThreaded | flag::EqualsAlways  | flag::PropagatesAlways));
DEFINE_TEST_ALLOCATOR(allocator3, (flag::SingleThreaded | flag::EqualsByValue | flag::PropagatesNever ));
DEFINE_TEST_ALLOCATOR(allocator4, (flag::SingleThreaded | flag::EqualsNever   | flag::PropagatesNever ));

DEFINE_TEST_ALLOCATOR(allocatorM, (flag::MultiThreaded  | flag::EqualsAlways  | flag::PropagatesNever ));

template<typename T, typename Allocator>
int sum(const gkr::lockfree_bag<T, Allocator>& bag)
{
    T total {0};

    for(auto& item : bag)
    {
        total += item;
    }
    return int(total);
}

#ifdef _MSC_VER
#pragma warning(disable:4868)
#endif

TEMPLATE_PRODUCT_TEST_CASE("container.lockfree_bag. lifecycle", "", (allocator1, allocator2, allocator3, allocator4), (int, Data))
{
    using allocator_t = TestType;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag1;

        CHECK(bag1.empty());

        bag1.insert(value_t(1));
        bag1.insert(value_t(2));
        bag1.insert(value_t(3));

        CHECK(!bag1.empty());
        CHECK(bag1.size() == 3);
        CHECK(sum(bag1) == 6);

        SECTION("construction with allocator")
        {
            allocator_t allocator = bag1.get_allocator();

            bag_t bag2(allocator);

            CHECK(bag2.empty());
            CHECK(bag2.size() == 0);
            CHECK(sum(bag2) == 0);
        }

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
        SECTION("copy construction with allocator")
        {
            bag_t bag2(bag1, allocator_t());

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

            bag2.insert(value_t(10));
            bag2.insert(value_t(20));

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
        SECTION("move construction with allocator")
        {
            bag_t bag2(std::move(bag1), allocator_t());

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

            bag2.insert(value_t(10));
            bag2.insert(value_t(20));

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

            bag2.insert(value_t(10));
            bag2.insert(value_t(20));

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
    CHECK(objects_ref_counting::check());
}

TEST_CASE("container.lockfree_bag. getters")
{
    using allocator_t = allocator1<Data>;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag_src;

        const bag_t& bag = bag_src;

        SECTION("max_size")
        {
            const bag_t::size_type max_size = bag.max_size();

            CHECK(max_size > 0);
        }
        SECTION("size")
        {
            CHECK(bag.size() == 0);
            bag_src.insert();
            CHECK(bag.size() == 1);
            bag_src.insert();
            CHECK(bag.size() == 2);
            bag_src.insert();
            CHECK(bag.size() == 3);
        }
        SECTION("empty")
        {
            CHECK(bag.empty());
            bag_src.insert();
            CHECK(!bag.empty());
            bag_src.insert();
            CHECK(!bag.empty());
        }
        SECTION("get_allocator")
        {
            allocator_t a1 = bag.get_allocator();
            allocator_t a2 = bag.get_allocator();

            CHECK(a1 == a2);
        }
    }
    CHECK(objects_ref_counting::check());
}

TEMPLATE_TEST_CASE("container.lockfree_bag. clear", "", int, Data, std::string)
{
    using allocator_t = allocator1<TestType>;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag;

        CHECK(bag.empty());
        CHECK(bag.size() == 0);

        bag.insert();
        bag.insert();
        bag.insert();

        CHECK(!bag.empty());
        CHECK(bag.size() == 3);

        bag.clear();

        CHECK(bag.empty());
        CHECK(bag.size() == 0);
    }
    CHECK(objects_ref_counting::check());
}

TEST_CASE("container.lockfree_bag. iterators")
{
    using allocator_t = allocator1<Data>;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag;

        CHECK(bag.empty());
        CHECK(bag.size() == 0);
        CHECK(sum(bag) == 0);

        SECTION("regular")
        {
            CHECK(bag.begin() == bag.end());

            bag.insert();

            CHECK(  bag.begin()   != bag.end());
            CHECK(++bag.begin()   == bag.end());
            CHECK(  bag.begin()++ != bag.end());
        }
        SECTION("c prefixed")
        {
            CHECK(bag.cbegin() == bag.cend());

            bag.insert();

            CHECK(  bag.cbegin()   != bag.cend());
            CHECK(++bag.cbegin()   == bag.cend());
            CHECK(  bag.cbegin()++ != bag.cend());
        }
    }
    CHECK(objects_ref_counting::check());
}

TEMPLATE_TEST_CASE("container.lockfree_bag. comparison", "", int, Data)
{
    using allocator_t = allocator1<TestType>;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag1;

        CHECK(bag1.empty());

        bag1.insert(value_t(1));
        bag1.insert(value_t(2));
        bag1.insert(value_t(3));
        bag1.insert(value_t(1));
        bag1.insert(value_t(3));

        CHECK(!bag1.empty());
        CHECK(bag1.size() == 5);
        CHECK(sum(bag1) == 10);

        CHECK(bag1 == bag1);

        SECTION("Equality")
        {
            bag_t bag2(bag1);

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 5);
            CHECK(sum(bag2) == 10);

            CHECK(bag1 == bag2);
            CHECK(bag2 == bag1);

            bag_t bag3;

            CHECK(bag3.empty());
            CHECK(bag3 != bag1);

            bag3.insert(value_t(3));
            bag3.insert(value_t(3));
            bag3.insert(value_t(1));
            bag3.insert(value_t(1));
            bag3.insert(value_t(2));

            CHECK(!bag3.empty());
            CHECK(bag3.size() == 5);
            CHECK(sum(bag3) == 10);

            CHECK(bag1 == bag3);
            CHECK(bag3 == bag1);
        }
        SECTION("Inequality")
        {
            bag_t bag2(bag1);

            bag2.insert(value_t(2));

            CHECK(!bag2.empty());
            CHECK(bag2.size() == 6);
            CHECK(sum(bag2) == 12);

            CHECK(bag1 != bag2);
            CHECK(bag2 != bag1);

            bag_t bag3;
            bag3.insert(value_t(1));
            bag3.insert(value_t(2));
            bag3.insert(value_t(3));

            CHECK(!bag3.empty());
            CHECK(bag3.size() == 3);
            CHECK(sum(bag3) == 6);

            CHECK(bag1 != bag3);
            CHECK(bag3 != bag1);
        }
    }
    CHECK(objects_ref_counting::check());
}

TEST_CASE("container.lockfree_bag. insert")
{
    using allocator_t = allocator1<Data>;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag;

        CHECK(bag.empty());
        CHECK(bag.size() == 0);

        SECTION("default construct")
        {
            bag.insert()->a = 1;
            bag.insert()->a = 2;
            bag.insert()->a = 3;
        }
        SECTION("move construct")
        {
            bag.insert(Data{1});
            bag.insert(Data{2});
            bag.insert(Data{3});
        }
        SECTION("copy construct")
        {
            Data d;
            bag.insert(d)->a = 1;
            bag.insert(d)->a = 2;
            bag.insert(d)->a = 3;
        }
        SECTION("emplace")
        {
            bag.emplace(-10, 1, 5, 5);
            bag.emplace(-20, 2, 3, 7, 4, 6);
            bag.emplace(-30, 3, 1, 9, 2, 8, 0, 10);
        }

        CHECK(!bag.empty());
        CHECK(bag.size() == 3);
        CHECK(sum(bag) == 6);
    }
    CHECK(objects_ref_counting::check());
}

TEMPLATE_TEST_CASE("container.lockfree_bag. erase", "", int, Data)
{
    using allocator_t = allocator1<TestType>;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag1;

        CHECK(bag1.empty());
        CHECK(bag1.size() == 0);
        CHECK(sum(bag1) == 0);

        bag1.insert(value_t(1));
        bag1.insert(value_t(2));
        bag1.insert(value_t(1));
        bag1.insert(value_t(0));
        bag1.insert(value_t(3));
        bag1.insert(value_t(1));
        bag1.insert(value_t(3));

        CHECK(!bag1.empty());
        CHECK(bag1.size() == 7);
        CHECK(sum(bag1) == 11);

        SECTION("by iterator")
        {
            bag_t bag2(bag1);
            while(!bag2.empty())
            {
                bag2.erase(bag2.begin());
            }
            bag_t bag3(bag1);
            while(!bag3.empty())
            {
                typename bag_t::iterator it;
                for(auto i = bag3.begin(); i != bag3.end(); ) it = i++;
                bag3.erase(it);
            }
            for(int v: {4, 3, 2, 1, 0})
            {
                while(bag1.contains(value_t(v))) bag1.erase(bag1.find(value_t(v)));
            }
            CHECK(bag2 == bag1);
            CHECK(bag3 == bag1);
        }
        SECTION("by range")
        {
            bag_t bag2(bag1);
            while(bag2.size() > 1)
            {
                bag2.erase(++bag2.begin(), ++(++(++bag2.begin())));
            }
            CHECK(bag2.size() == 1);
            bag2.erase(bag2.begin(), bag2.end());

            bag_t bag3(bag1);
            while(bag3.size() > 1)
            {
                bag3.erase(++bag3.begin(), ++(++(++(++bag3.begin()))));
            }
            CHECK(bag3.size() == 1);
            bag3.erase(bag3.begin(), bag3.end());

            bag1.erase(bag1.begin(), bag1.end());
            CHECK(bag2 == bag1);
            CHECK(bag3 == bag1);
        }
        SECTION("by value")
        {
            CHECK(bag1.erase(value_t(4)) == 0);
            CHECK(bag1.erase(value_t(3)) == 2);
            CHECK(bag1.erase(value_t(2)) == 1);
            CHECK(bag1.erase(value_t(1)) == 3);
            CHECK(bag1.erase(value_t(0)) == 1);
        }

        CHECK(bag1.empty());
        CHECK(bag1.size() == 0);
        CHECK(sum(bag1) == 0);
    }
    CHECK(objects_ref_counting::check());
}

TEMPLATE_TEST_CASE("container.lockfree_bag. lookup", "", int, Data)
{
    using allocator_t = allocator1<TestType>;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag_src;

        CHECK(bag_src.empty());
        CHECK(bag_src.size() == 0);
        CHECK(sum(bag_src) == 0);

        bag_src.insert(value_t(1));
        bag_src.insert(value_t(2));
        bag_src.insert(value_t(1));
        bag_src.insert(value_t(3));
        bag_src.insert(value_t(1));
        bag_src.insert(value_t(3));

        CHECK(!bag_src.empty());
        CHECK(bag_src.size() == 6);
        CHECK(sum(bag_src) == 11);

        const bag_t& bag = bag_src;

        SECTION("find")
        {
            CHECK(bag_src.find(value_t(0)) == bag_src.end());
            CHECK(bag_src.find(value_t(1)) != bag_src.end());
            CHECK(bag_src.find(value_t(2)) != bag_src.end());
            CHECK(bag_src.find(value_t(3)) != bag_src.end());
            CHECK(bag_src.find(value_t(4)) == bag_src.end());
        }
        SECTION("const find")
        {
            CHECK(bag.find(value_t(0)) == bag.end());
            CHECK(bag.find(value_t(1)) != bag.end());
            CHECK(bag.find(value_t(2)) != bag.end());
            CHECK(bag.find(value_t(3)) != bag.end());
            CHECK(bag.find(value_t(4)) == bag.end());
        }
        SECTION("contains")
        {
            CHECK(!bag.contains(value_t(0)));
            CHECK( bag.contains(value_t(1)));
            CHECK( bag.contains(value_t(2)));
            CHECK( bag.contains(value_t(3)));
            CHECK(!bag.contains(value_t(4)));
        }
        SECTION("count")
        {
            CHECK(bag.count(value_t(0)) == 0);
            CHECK(bag.count(value_t(1)) == 3);
            CHECK(bag.count(value_t(2)) == 1);
            CHECK(bag.count(value_t(3)) == 2);
            CHECK(bag.count(value_t(4)) == 0);
        }
    }
    CHECK(objects_ref_counting::check());
}

TEST_CASE("container.lockfree_bag. multithreading")
{
    using allocator_t = allocatorM<Data>;
    using value_t     = typename allocator_t::value_type;

    using bag_t  = gkr::lockfree_bag<value_t, allocator_t>;
//  using slot_t = typename bag_t::allocator_value_type;

    constexpr std::size_t threads_count = 20;
    constexpr std::size_t nodes_to_add  = 500;
    constexpr std::size_t total_nodes   = threads_count * nodes_to_add;

    CHECK(objects_ref_counting::check());
    {
        bag_t bag;

        std::atomic<int> n(threads_count);
        std::atomic<int> m(0);

        std::thread threads[threads_count];

        for(std::thread& thread : threads)
        {
            thread = std::thread([&bag, &n, &m] () {
                for(--n; n > 0; );
                for(std::size_t index = 0; index < nodes_to_add; ++index)
                {
                    int a = ++m;
                    std::this_thread::yield();
                    bag.insert()->a = a;
                }
            });
        }
        for(std::thread& thread : threads)
        {
            thread.join();
        }

        CHECK(bag.size() == total_nodes);
        CHECK(sum(bag) == (total_nodes*(total_nodes+1)/2));
    }
    CHECK(objects_ref_counting::check());
}
