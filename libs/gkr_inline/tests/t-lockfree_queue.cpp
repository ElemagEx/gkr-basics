#include <gkr/container/lockfree_queue.hpp>

#include <catch2/catch_template_test_macros.hpp>

#include <gkr/testing/allocator.hpp>
#include <gkr/testing/named_requirements.hpp>

using flag = gkr::testing::allocator_flag;

struct Vec3
{
    int x, y, z;

    bool operator==(const Vec3& other) const
    {
        return (x == other.x) && (y == other.y) && (z == other.z);
    }
};
#if 0
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
#endif

template<bool Typeless, typename Type>
using queue_value_type = std::conditional<Typeless, void, Type>::type;

template<typename Queue>
constexpr bool is_typeless()
{
    return std::is_void<typename Queue::element_t>::value;
}
template<typename Type, typename Queue>
void push_elements(Queue& queue, unsigned number)
{
    for(int index = 0; index < int(number); ++index)
    {
        if constexpr(is_typeless<Queue>())
        {
            queue.try_start_push().as<Type>() = Type{index+1, index+2, index+3};
        }
        else
        {
            queue.try_push(Type{index+1, index+2, index+3});
        }
    }
}
template<typename Type, typename Queue>
void queue_reset_n(Queue& queue, std::size_t capacity)
{
    if constexpr(is_typeless<Queue>())
    {
        queue.reset(capacity, sizeof(Type));
    }
    else
    {
        queue.reset(capacity);
    }
}

template<typename Type, typename Queue>
void test_lifecycle_with_constructor_default()
{
    Queue queue;
    queue_reset_n<Type>(queue, 8);
    push_elements<Type>(queue, 3);
    CHECK(queue.count() == 3);
}
template<typename Type, typename Queue>
void test_lifecycle_with_constructor_reset()
{
    if constexpr(is_typeless<Queue>())
    {
        Queue queue(8, sizeof(Type));
        push_elements<Type>(queue, 3);
        CHECK(queue.count() == 3);
    }
    else
    {
        Queue queue(8);
        push_elements<Vec3>(queue, 3);
        CHECK(queue.count() == 3);
    }
}
template<typename Type, typename Queue>
void test_lifecycle_with_constructor_move()
{
    Queue queue1;
    queue_reset_n<Type>(queue1, 8);
    push_elements<Type>(queue1, 3);
    Queue queue2(std::move(queue1));
    CHECK(queue2.capacity() == 8);
    CHECK(queue2.count   () == 3);
}
template<typename Type, bool Typeless, bool Multiple, bool Pausable>
void test_lifecycle()
{
    using Element = queue_value_type<Typeless, Type>;

    using Allocator = gkr::testing::ref_counting_allocator<Type>;

    using Queue = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator>;

    test_lifecycle_with_constructor_default<Type, Queue>();
    test_lifecycle_with_constructor_reset  <Type, Queue>();
    test_lifecycle_with_constructor_move   <Type, Queue>();
}
TEST_CASE("container.lockfree_queue. lifecycle")
{
    using Allocator = gkr::testing::ref_counting_allocator<Vec3>;

    CHECK(Allocator::check());

    SECTION("typefull, single")
    {
        test_lifecycle<Vec3, false, false, false>();
    }
    SECTION("typefull, multiple")
    {
        test_lifecycle<Vec3, false, true , false>();
    }
    SECTION("typeless, single")
    {
        test_lifecycle<Vec3, true , false, false>();
    }
    SECTION("typeless, multiple")
    {
        test_lifecycle<Vec3, true , true , false>();
    }

    CHECK(Allocator::check());
}

DEFINE_REF_COUNTING_ALLOCATOR(Allocator_move_op_0, flag::PropagatesOnMoveAssignment);
DEFINE_REF_COUNTING_ALLOCATOR(Allocator_move_op_1, flag::EqualsAlways );
DEFINE_REF_COUNTING_ALLOCATOR(Allocator_move_op_2, flag::EqualsByValue);
DEFINE_REF_COUNTING_ALLOCATOR(Allocator_move_op_3, flag::EqualsNever  );

template<typename Type, typename Queue>
void test_move_assignment()
{
    Queue queue0;
    queue_reset_n<Type>(queue0, 8);
    push_elements<Type>(queue0, 3);

    Queue queue1;
    queue_reset_n<Type>(queue1, 8);
    push_elements<Type>(queue1, 3);
    CHECK(queue0 == queue1);

    Queue queue2;
    queue_reset_n<Type>(queue2, 8);
    push_elements<Type>(queue2, 1);
    CHECK_FALSE(queue0 == queue2);

    queue2 = std::move(queue1);

    CHECK(queue2.capacity() == 8);
    CHECK(queue2.count   () == 3);
    CHECK(queue0 == queue2);
}
template<typename Type, bool Typeless, bool Multiple, bool Pausable>
void test_move_assignments()
{
    using Element = queue_value_type<Typeless, Type>;

    using Queue0 = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator_move_op_0<Type>>;
    using Queue1 = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator_move_op_1<Type>>;
    using Queue2 = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator_move_op_2<Type>>;
    using Queue3 = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator_move_op_3<Type>>;
    test_move_assignment<Type, Queue0>();
    test_move_assignment<Type, Queue1>();
    test_move_assignment<Type, Queue2>();
    test_move_assignment<Type, Queue3>();
}
TEST_CASE("container.lockfree_queue. move_assignment")
{
    using Allocator = gkr::testing::ref_counting_allocator<Vec3>;

    CHECK(Allocator::check());

    SECTION("typefull, single")
    {
        test_move_assignments<Vec3, false, false, false>();
    }
    SECTION("typefull, multiple")
    {
        test_move_assignments<Vec3, false, true , false>();
    }
    SECTION("typeless, single")
    {
        test_move_assignments<Vec3, true , false, false>();
    }
    SECTION("typeless, multiple")
    {
        test_move_assignments<Vec3, true , true , false>();
    }

    CHECK(Allocator::check());
}

DEFINE_REF_COUNTING_ALLOCATOR(Allocator_swap_op_0, flag::PropagatesOnSwap);
DEFINE_REF_COUNTING_ALLOCATOR(Allocator_swap_op_1, flag::EqualsAlways );
DEFINE_REF_COUNTING_ALLOCATOR(Allocator_swap_op_2, flag::EqualsByValue);
DEFINE_REF_COUNTING_ALLOCATOR(Allocator_swap_op_3, flag::EqualsNever  );

template<typename Type, typename Queue>
void test_swap()
{
    Queue queue0;
    queue_reset_n<Type>(queue0, 8);
    push_elements<Type>(queue0, 3);

    Queue queue1;
    queue_reset_n<Type>(queue1, 8);
    push_elements<Type>(queue1, 2);

    Queue queue2;
    queue_reset_n<Type>(queue2, 8);
    push_elements<Type>(queue2, 3);
    CHECK(queue0 == queue2);

    Queue queue3;
    queue_reset_n<Type>(queue3, 8);
    push_elements<Type>(queue3, 2);

    CHECK(queue0 == queue2);
    CHECK(queue1 == queue3);

    CHECK_FALSE(queue0 == queue3);
    CHECK_FALSE(queue1 == queue2);

    std::swap(queue2, queue3);

    CHECK(queue0 == queue3);
    CHECK(queue1 == queue2);
}
template<typename Type, bool Typeless, bool Multiple, bool Pausable>
void test_swaps()
{
    using Element = queue_value_type<Typeless, Type>;

    using Queue0 = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator_swap_op_0<Type>>;
    using Queue1 = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator_swap_op_1<Type>>;
    using Queue2 = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator_swap_op_2<Type>>;
    using Queue3 = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator_swap_op_3<Type>>;
    test_swap<Type, Queue0>();
    test_swap<Type, Queue1>();
    test_swap<Type, Queue2>();
    test_swap<Type, Queue3>();
}
TEST_CASE("container.lockfree_queue. swap")
{
    using Allocator = gkr::testing::ref_counting_allocator<Vec3>;

    CHECK(Allocator::check());

    SECTION("typefull, single")
    {
        test_swaps<Vec3, false, false, false>();
    }
    SECTION("typefull, multiple")
    {
        test_swaps<Vec3, false, true , false>();
    }
    SECTION("typeless, single")
    {
        test_swaps<Vec3, true , false, false>();
    }
    SECTION("typeless, multiple")
    {
        test_swaps<Vec3, true , true , false>();
    }

    CHECK(Allocator::check());
}

template<typename Type, bool Typeless, bool Multiple, bool Pausable>
void test_getters()
{
    using Element = queue_value_type<Typeless, Type>;

    using Allocator = gkr::testing::ref_counting_allocator<Type>;

    using Queue = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator>;

    Allocator a1;
    Queue queue(a1);

    auto a2 = queue.get_allocator();

    CHECK(std::allocator_traits<decltype(a2)>::max_size(a2) == std::allocator_traits<Allocator>::max_size(a1));

    CHECK(queue.element_alignment() == alignof(Type));
    CHECK(queue.element_size     () ==  sizeof(Type));

    CHECK( queue.empty());
    CHECK(!queue.full());
    CHECK( queue.count() == 0);
    CHECK( queue.capacity() == 0);

    queue_reset_n<Type>(queue, 8);

    CHECK( queue.empty());
    CHECK(!queue.full());
    CHECK( queue.count() == 0);
    CHECK( queue.capacity() == 8);

    push_elements<Type>(queue, 3);

    CHECK(!queue.empty());
    CHECK(!queue.full());
    CHECK( queue.count() == 3);
    CHECK( queue.capacity() == 8);

    push_elements<Type>(queue, 5);

    CHECK(!queue.empty());
    CHECK( queue.full());
    CHECK( queue.count() == 8);
    CHECK( queue.capacity() == 8);
}
TEST_CASE("container.lockfree_queue. getters")
{
    using Allocator = gkr::testing::ref_counting_allocator<Vec3>;

    CHECK(Allocator::check());

    SECTION("typefull, single")
    {
        test_swaps<Vec3, false, false, false>();
    }
    SECTION("typefull, multiple")
    {
        test_swaps<Vec3, false, true , false>();
    }
    SECTION("typeless, single")
    {
        test_swaps<Vec3, true , false, false>();
    }
    SECTION("typeless, multiple")
    {
        test_swaps<Vec3, true , true , false>();
    }

    CHECK(Allocator::check());
}

template<typename Type, bool Typeless, bool Multiple, bool Pausable>
void test_clear()
{
    using Element = queue_value_type<Typeless, Type>;

    using Allocator = gkr::testing::ref_counting_allocator<Type>;

    using Queue = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator>;

    Queue queue;

    CHECK( queue.empty());
    CHECK( queue.capacity() == 0);

    queue_reset_n<Type>(queue, 8);
    push_elements<Type>(queue, 3);

    CHECK(!queue.empty());
    CHECK( queue.capacity() == 8);

    queue.clear();

    CHECK( queue.empty());
    CHECK( queue.capacity() == 0);
}
TEST_CASE("container.lockfree_queue. clear")
{
    using Allocator = gkr::testing::ref_counting_allocator<Vec3>;

    CHECK(Allocator::check());

    SECTION("typefull, single")
    {
        test_clear<Vec3, false, false, false>();
    }
    SECTION("typefull, multiple")
    {
        test_clear<Vec3, false, true , false>();
    }
    SECTION("typeless, single")
    {
        test_clear<Vec3, true , false, false>();
    }
    SECTION("typeless, multiple")
    {
        test_clear<Vec3, true , true , false>();
    }

    CHECK(Allocator::check());
}

template<typename Type, bool Typeless, bool Multiple, bool Pausable>
void test_reset()
{
    using Element = queue_value_type<Typeless, Type>;

    using Allocator = gkr::testing::ref_counting_allocator<Type>;

    using Queue = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator>;

    Queue queue;
    if constexpr(is_typeless<Queue>())
    {
        queue.reset(0, sizeof(Type));
    }

    CHECK( queue.empty());
    CHECK( queue.capacity() == 0);

    queue.reset(8);

    CHECK( queue.empty());
    CHECK( queue.capacity() == 8);

    push_elements<Type>(queue, 3);

    CHECK(!queue.empty());
    CHECK( queue.capacity() == 8);

    queue.reset(4);

    CHECK( queue.empty());
    CHECK( queue.capacity() == 4);

    push_elements<Type>(queue, 3);

    queue.reset(0);

    CHECK( queue.empty());
    CHECK( queue.capacity() == 0);
}
TEST_CASE("container.lockfree_queue. reset")
{
    using Allocator = gkr::testing::ref_counting_allocator<Vec3>;

    CHECK(Allocator::check());

    SECTION("typefull, single")
    {
        test_reset<Vec3, false, false, false>();
    }
    SECTION("typefull, multiple")
    {
        test_reset<Vec3, false, true , false>();
    }
    SECTION("typeless, single")
    {
        test_reset<Vec3, true , false, false>();
    }
    SECTION("typeless, multiple")
    {
        test_reset<Vec3, true , true , false>();
    }

    CHECK(Allocator::check());
}
template<typename Value, typename Iterator>
void test_iterator_old(Iterator begin, Iterator end)
{
    for(auto it = begin; it != end; ++it)
    {
        if constexpr(is_typeless<typename Iterator::queue_t>())
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
template<typename Value, typename Queue>
void test_iterator_new(Queue& queue)
{
    if constexpr(is_typeless<Queue>())
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
template<typename Type, bool Typeless, bool Multiple, bool Pausable>
void test_iterator_all()
{
    using Element = queue_value_type<Typeless, Type>;

    using Allocator = gkr::testing::ref_counting_allocator<Type>;

    using Queue = gkr::lockfree_queue<Element, Multiple, Pausable, Allocator>;

    Queue queue;
    queue_reset_n<Type>(queue, 8);
    push_elements<Type>(queue, 3);

    test_iterator_new<      Type>(queue);

    test_iterator_old<      Type>(queue.  begin(), queue.  end());
    test_iterator_old<      Type>(queue. rbegin(), queue. rend());
    test_iterator_old<const Type>(queue. cbegin(), queue. cend());
    test_iterator_old<const Type>(queue.crbegin(), queue.crend());

    const Queue& cqueue = queue;

    test_iterator_new<const Type>(cqueue);

    test_iterator_old<const Type>(cqueue.  begin(), cqueue.  end());
    test_iterator_old<const Type>(cqueue. rbegin(), cqueue. rend());
    test_iterator_old<const Type>(cqueue. cbegin(), cqueue. cend());
    test_iterator_old<const Type>(cqueue.crbegin(), cqueue.crend());
}
TEST_CASE("container.lockfree_queue. iterators")
{
    using Allocator = gkr::testing::ref_counting_allocator<Vec3>;

    CHECK(Allocator::check());

    SECTION("typefull, single")
    {
        test_iterator_all<Vec3, false, false, false>();
    }
    SECTION("typefull, multiple")
    {
        test_iterator_all<Vec3, false, true , false>();
    }
    SECTION("typeless, single")
    {
        test_iterator_all<Vec3, true , false, false>();
    }
    SECTION("typeless, multiple")
    {
        test_iterator_all<Vec3, true , true , false>();
    }

    CHECK(Allocator::check());
}

TEST_CASE("container.lockfree_queue. compliance")
{
    gkr::lockfree_queue<Vec3> queue1;

    gkr::testing::is_container(queue1);
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
