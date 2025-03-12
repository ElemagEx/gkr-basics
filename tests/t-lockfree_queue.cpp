#include <gkr/container/lockfree_queue.hpp>

#include <catch2/catch_template_test_macros.hpp>

#include <gkr/testing/allocator.hpp>

struct V
{
    float x, y, z;
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

TEST_CASE("container.lockfree_queue. main")
{
    gkr::lockfree_queue<V, false> queue2;
    queue2.reset(20);

    gkr::lockfree_queue<V, false> queue;

    queue = std::move(queue2);

    queue.threading.set_current_thread_as_exclusive_producer();
    queue.threading.any_thread_can_be_producer();
    queue.capacity();
    queue.count();

    {
        auto element = queue.try_start_push({1.f, 2.f, 3.f});

        if(element.push_in_progress())
        {
            element->x += 1.f;
            element->y += 2.f;
            element->z += 3.f;
        }
    }

    gkr::lockfree_queue<void, true> q1;
    q1.reset(10, 16);
    q1.try_push();

    gkr::lockfree_queue<void, true> q2;
    q1.reset(10, 16);
    q2.try_push();

    q1 = std::move(q2);

    float s = 0.f;

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
