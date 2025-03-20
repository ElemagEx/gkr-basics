#include <gkr/container/lockfree_queue.hpp>

#include <catch2/catch_template_test_macros.hpp>

#include <gkr/testing/allocator.hpp>
#include <gkr/testing/named_requirements.hpp>

struct Vec3
{
    int x, y, z;
};
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

using uint = unsigned int;

template<typename Allocator> using queue1_uint_t = gkr::lockfree_queue<uint, false, false, Allocator>;
template<typename Allocator> using queue2_uint_t = gkr::lockfree_queue<uint, true , false, Allocator>;
template<typename Allocator> using queue3_uint_t = gkr::lockfree_queue<uint, false, true , Allocator>;
template<typename Allocator> using queue4_uint_t = gkr::lockfree_queue<uint, true , true , Allocator>;

template<typename Allocator> using queue1_data_t = gkr::lockfree_queue<Data, false, false, Allocator>;
template<typename Allocator> using queue2_data_t = gkr::lockfree_queue<Data, true , false, Allocator>;
template<typename Allocator> using queue3_data_t = gkr::lockfree_queue<Data, false, true , Allocator>;
template<typename Allocator> using queue4_data_t = gkr::lockfree_queue<Data, true , true , Allocator>;

template<typename Allocator> using queue1_void_t = gkr::lockfree_queue<void, false, false, Allocator>;
template<typename Allocator> using queue2_void_t = gkr::lockfree_queue<void, true , false, Allocator>;
template<typename Allocator> using queue3_void_t = gkr::lockfree_queue<void, false, true , Allocator>;
template<typename Allocator> using queue4_void_t = gkr::lockfree_queue<void, true , true , Allocator>;

using flag = gkr::testing::allocator_flag;

DEFINE_TEST_ALLOCATOR(allocator1, (flag::SingleThreaded | flag::EqualsAlways  | flag::PropagatesNever ));
DEFINE_TEST_ALLOCATOR(allocator2, (flag::SingleThreaded | flag::EqualsAlways  | flag::PropagatesAlways));
DEFINE_TEST_ALLOCATOR(allocator3, (flag::SingleThreaded | flag::EqualsByValue | flag::PropagatesNever ));
DEFINE_TEST_ALLOCATOR(allocator4, (flag::SingleThreaded | flag::EqualsNever   | flag::PropagatesNever ));

DEFINE_TEST_ALLOCATOR(allocatorM, (flag::MultiThreaded  | flag::EqualsAlways  | flag::PropagatesNever ));

using std_allocator_t = std::allocator<char>;

using allocator1_t = allocator1<char>;
using allocator2_t = allocator1<char>;
using allocator3_t = allocator1<char>;
using allocator4_t = allocator1<char>;

using allocatorM_t = allocator1<char>;

#ifdef _MSC_VER
#pragma warning(disable:4868)
#endif
#if 0
TEMPLATE_PRODUCT_TEST_CASE("container.lockfree_queue.fixed_type. lifecycle", "", (queue1_data_t, queue2_data_t, queue3_data_t, queue4_data_t), (std_allocator_t, allocator1_t, allocator2_t, allocator3_t, allocator4_t))
{
    using queue_t = TestType;
    using allocator_t = typename queue_t::TypeAllocator;

    using value_t = typename allocator_t::value_type;

    gkr::testing::get_singlethreaded_pre_allocated_storage<value_t>().reset(16);
    {
        queue_t queue1;
        queue1.reset(8);

        CHECK(queue1.empty());

        REQUIRE(queue1.try_push(value_t(1)));
        REQUIRE(queue1.try_push(value_t(2)));
        REQUIRE(queue1.try_push(value_t(3)));

    }
    gkr::testing::get_singlethreaded_pre_allocated_storage<value_t>().reset();
}
#endif

template<bool TYPELESS, typename Value, typename Iterator>
void test_iterator_old(Iterator begin, Iterator end)
{
    for(auto it = begin; it != end; ++it)
    {
        if constexpr(TYPELESS)
        {
            Value& val = *static_cast<Value*>(*it);
            CHECK(val.x > 0);
        }
        else
        {
            Value& val = *it;
            CHECK(val.x == it->x);
        }
    }
}
template<bool TYPELESS, typename Value, typename Queue>
void test_iterator_new(Queue& queue)
{
    if constexpr(TYPELESS)
    {
        for(auto element : queue)
        {
            Value& val = *static_cast<Value*>(element);
            CHECK(val.x > 0);
        }
    }
    else
    {
        for(auto& element : queue)
        {
            Value& val = element;
            CHECK(val.x > 0);
        }
    }
}
template<bool TYPELESS, typename Queue>
void test_iterators(Queue& queue)
{
    REQUIRE(queue.count() == 3);

    test_iterator_new<TYPELESS,       Vec3>(queue);

    test_iterator_old<TYPELESS,       Vec3>(queue.  begin(), queue.  end());
    test_iterator_old<TYPELESS,       Vec3>(queue. rbegin(), queue. rend());
    test_iterator_old<TYPELESS, const Vec3>(queue. cbegin(), queue. cend());
    test_iterator_old<TYPELESS, const Vec3>(queue.crbegin(), queue.crend());

    const Queue& cqueue = queue;

    test_iterator_new<TYPELESS, const Vec3>(cqueue);

    test_iterator_old<TYPELESS, const Vec3>(cqueue.  begin(), cqueue.  end());
    test_iterator_old<TYPELESS, const Vec3>(cqueue. rbegin(), cqueue. rend());
    test_iterator_old<TYPELESS, const Vec3>(cqueue. cbegin(), cqueue. cend());
    test_iterator_old<TYPELESS, const Vec3>(cqueue.crbegin(), cqueue.crend());
}

TEST_CASE("container.lockfree_queue. iterators")
{
    using namespace gkr;

    SECTION("typefull, single")
    {
        lockfree_queue<Vec3, false> queue(8);
        queue.try_push(Vec3{1, 2, 3});
        queue.try_push(Vec3{2, 3, 4});
        queue.try_push(Vec3{3, 4, 5});
        test_iterators<false>(queue);
    }
    SECTION("typefull, multiple")
    {
        lockfree_queue<Vec3, true > queue(8);
        queue.try_push(Vec3{1, 2, 3});
        queue.try_push(Vec3{2, 3, 4});
        queue.try_push(Vec3{3, 4, 5});
        test_iterators<false>(queue);
    }
    SECTION("typeless, single")
    {
        lockfree_queue<void, false> queue(8, sizeof(Vec3));
        queue.try_start_push().as<Vec3>() = Vec3{1, 2, 3};
        queue.try_start_push().as<Vec3>() = Vec3{2, 3, 4};
        queue.try_start_push().as<Vec3>() = Vec3{3, 4, 5};
        test_iterators<true >(queue);
    }
    SECTION("typeless, multiple")
    {
        lockfree_queue<void, true > queue(8, sizeof(Vec3));
        queue.try_start_push().as<Vec3>() = Vec3{1, 2, 3};
        queue.try_start_push().as<Vec3>() = Vec3{2, 3, 4};
        queue.try_start_push().as<Vec3>() = Vec3{3, 4, 5};
        test_iterators<true >(queue);
    }
}

TEST_CASE("container.lockfree_queue. compliance")
{
    using namespace gkr;

    lockfree_queue<Vec3> queue1;

    testing::is_container(queue1);
}

TEST_CASE("container.lockfree_queue. main")
{
    gkr::lockfree_queue<Vec3, false> queue2;
    queue2.reset(20);

    gkr::lockfree_queue<Vec3, false> queue;

    queue = std::move(queue2);

    queue.threading.set_current_thread_as_exclusive_producer();
    queue.threading.any_thread_can_be_producer();
    queue.capacity();
    queue.count();

    {
        auto element = queue.try_start_push({1, 2, 3});

        if(element.push_in_progress())
        {
            element->x += 1;
            element->y += 2;
            element->z += 3;
        }
    }

    gkr::lockfree_queue<void, true> q1;
    q1.reset(10, 16);
    q1.try_push();

    gkr::lockfree_queue<void, true> q2;
    q1.reset(10, 16);
    q2.try_push();

    q1 = std::move(q2);

    int s = 0;

    while(!queue.empty())
    {
        auto element(queue.try_start_pop());

        if(element.pop_in_progress())
        {
            s += element->x;
            s += element->y;
            s += element->z;
        }
    }
    INFO(s);

    gkr::lockfree_queue<void> q3;

    q3.try_push();
    q3.try_pop();
}

TEST_CASE("container.lockfree_queue. sync")
{
    gkr::lockfree_queue<Vec3, false, true> queue;

    CHECK(queue.empty());

    auto count = queue.sync([&queue] () {

        return queue.count();

    });

    CHECK(count == 0);
}
