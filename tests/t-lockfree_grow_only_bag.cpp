#include <gkr/lockfree_grow_only_bag.h>

#include <string>
#include <atomic>
#include <utility>

#include <catch2/catch_test_macros.hpp>

#ifndef if_constexpr
#ifdef __cpp_if_constexpr
#define if_constexpr if constexpr
#else
#define if_constexpr if
#endif
#endif

using namespace gkr;

template<typename T>
class PreAllocatedStorage
{
    PreAllocatedStorage           (PreAllocatedStorage&&) noexcept = delete;
    PreAllocatedStorage& operator=(PreAllocatedStorage&&) noexcept = delete;

    PreAllocatedStorage           (const PreAllocatedStorage&) noexcept = delete;
    PreAllocatedStorage& operator=(const PreAllocatedStorage&) noexcept = delete;

private:
    std::size_t m_count = 0;
    T*          m_data  = nullptr;
    T**         m_ptrs  = nullptr;

    int m_total_allocations = 0;
    int m_total_elements    = 0;

private:
    void make_checks()
    {
        CHECK(m_total_allocations == 0);
        CHECK(m_total_elements    == 0);
    }

public:
    PreAllocatedStorage()
    {
    }
    ~PreAllocatedStorage()
    {
        make_checks();
        if(m_ptrs != nullptr) delete [] m_ptrs;
        if(m_data != nullptr) delete [] m_data;
    }

public:
    void reset(std::size_t count = 0)
    {
        make_checks();

        m_total_allocations = 0;
        m_total_elements    = 0;

        if(count > m_count)
        {
            if(m_ptrs != nullptr) delete [] m_ptrs;
            if(m_data != nullptr) delete [] m_data;

            m_data = new T [count];
            m_ptrs = new T*[count];

            m_count = count;
        }
    }

public:
    T* allocate(std::size_t n)
    {
        REQUIRE(n > 0);

        for(std::size_t pos, index = 0; index < m_count; ++index)
        {
            for(pos = 0; pos < n; ++pos)
            {
                if(m_ptrs[index + pos] != nullptr) break;
            }
            if(pos == n)
            {
                m_total_allocations += 1;
                m_total_elements    += int(n);

                for( ; n-- > 0; ++index)
                {
                    m_ptrs[index] = m_data + index;
                }
                return m_data + pos;
            }
        }
        throw std::bad_alloc();
    }
    void deallocate(T* ptr, std::size_t n)
    {
        std::size_t index = std::size_t(m_data - ptr);

        REQUIRE((index + 0) <  m_count);
        REQUIRE((index + n) <= m_count);

        m_total_allocations -= 1;
        m_total_elements    -= n;

        for( ; n-- > 0; ++index)
        {
            m_ptrs[index] = nullptr;
        }
    }
};

template<typename T>
PreAllocatedStorage<T>& get_preallocated_storage()
{
    static thread_local PreAllocatedStorage<T> storage;

    return storage;
}

enum
{
    EqualsNever   = 0,
    EqualsAlways  = 16,
    EqualsByValue = 32,

    PropagatesNever            = 0,
    PropagatesOnCopyAssignment = 1,
    PropagatesOnMoveAssignment = 2,
    PropagatesOnSwap           = 4,
};
template<typename T, int FLAGS>
class unit_test_allocator
{
public:
    using      value_type = T;
    using       size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

public:
    using is_always_equal = std::bool_constant<(FLAGS & EqualsAlways) != 0>;

    using propagate_on_container_copy_assignment = std::bool_constant<(FLAGS & PropagatesOnCopyAssignment) != 0>;
    using propagate_on_container_move_assignment = std::bool_constant<(FLAGS & PropagatesOnMoveAssignment) != 0>;
    using propagate_on_container_swap            = std::bool_constant<(FLAGS & PropagatesOnSwap          ) != 0>;

private:
    int m_self_allocations = 0;
    int m_self_elements    = 0;

protected:
    void reset()
    {
        if_constexpr(propagate_on_container_copy_assignment::value)
        {
            CHECK(m_self_allocations == 0);
            CHECK(m_self_elements    == 0);
        }
        m_self_allocations = 0;
        m_self_elements    = 0;
    }
    void swap(int&)
    {
    }

protected:
    unit_test_allocator() noexcept
    {
    }
    ~unit_test_allocator()
    {
        reset();
    }

    template<typename U>
    unit_test_allocator(const unit_test_allocator<U, FLAGS>&) noexcept
    {
    }
    unit_test_allocator(const unit_test_allocator&) noexcept
    {
    }
    unit_test_allocator& operator=(const unit_test_allocator&) noexcept
    {
        reset();
        return *this;
    }

    unit_test_allocator(unit_test_allocator&& other) noexcept
        : m_self_allocations(std::exchange(other.m_self_allocations, 0))
        , m_self_elements   (std::exchange(other.m_self_elements   , 0))
    {
    }
    unit_test_allocator& operator=(unit_test_allocator&& other) noexcept
    {
        m_self_allocations = std::exchange(other.m_self_allocations, 0);
        m_self_elements    = std::exchange(other.m_self_elements   , 0);
        return *this;
    }

    void swap(unit_test_allocator& other) noexcept
    {
        std::swap(m_self_allocations, other.m_self_allocations);
        std::swap(m_self_elements   , other.m_self_elements);
    }

protected:
    bool equals_by_value() const noexcept
    {
        return ((FLAGS & (EqualsAlways | EqualsByValue)) != 0);
    }

public:
    [[nodiscard]]
    T* allocate(std::size_t n)
    {
        return get_preallocated_storage<T>().allocate(n);
    }
    void deallocate(T* ptr, std::size_t n)
    {
        get_preallocated_storage<T>().deallocate(ptr, n);
    }
};

#define DEFINE_ALLOCATOR(NAME, FLAGS) \
template<class T>                                                                                       \
class NAME : public unit_test_allocator<T, FLAGS>                                                       \
{                                                                                                       \
public:                                                                                                 \
    using base_t = unit_test_allocator<T, FLAGS>;                                                       \
   ~NAME() noexcept {}                                                                                  \
    NAME() noexcept : base_t() {}                                                                       \
    NAME(const NAME&  other) noexcept : base_t(          other ) {}                                     \
    NAME(      NAME&& other) noexcept : base_t(std::move(other)) {}                                     \
    NAME& operator=(const NAME&  other) noexcept { base_t::operator=(          other ); return *this; } \
    NAME& operator=(      NAME&& other) noexcept { base_t::operator=(std::move(other)); return *this; } \
    bool operator==(const NAME&) const noexcept { return  base_t::equals_by_value(); }                  \
    bool operator!=(const NAME&) const noexcept { return !base_t::equals_by_value(); }                  \
    template<typename U> NAME(const NAME<U>& other) : base_t(other) {}                                  \
}

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

template<typename T, typename Allocator>
int sum(const lockfree_grow_only_bag<T, Allocator>& bag)
{
    T total {0};

    for(auto& item : bag)
    {
        total += item;
    }
    return int(total);
}

DEFINE_ALLOCATOR(allocator1, (EqualsAlways | PropagatesNever));

TEST_CASE("container.lockfree_bag. Lifecycle")
{
    using bag_t = lockfree_grow_only_bag<Data, allocator1<Data>>;

    bag_t bag1;

    CHECK(bag1.empty());

    bag1.add(Data(1));
    bag1.add(Data(2));
    bag1.add(Data(3));

    CHECK(!bag1.empty());
    CHECK(sum(bag1) == 6);

    SECTION("copy construction")
    {
        bag_t bag2(bag1);

        CHECK(!bag2.empty());
        CHECK(sum(bag2) == 6);

        CHECK(!bag1.empty());
        CHECK(sum(bag1) == 6);
    }
    SECTION("copy assignment")
    {
        bag_t bag2;

        bag2.add(Data(10));
        bag2.add(Data(30));

        CHECK(!bag2.empty());
        CHECK(sum(bag2) == 30);

        bag2 = bag1;

        CHECK(!bag2.empty());
        CHECK(sum(bag2) == 6);

        CHECK(!bag1.empty());
        CHECK(sum(bag1) == 6);
    }
    SECTION("move construction")
    {
        bag_t bag2(std::move(bag1));

        CHECK(!bag2.empty());
        CHECK(sum(bag2) == 6);

        CHECK(bag1.empty());
        CHECK(sum(bag1) == 0);
    }
    SECTION("move assignment")
    {
        bag_t bag2;

        bag2.add(Data(10));
        bag2.add(Data(30));

        CHECK(!bag2.empty());
        CHECK(sum(bag2) == 30);

        bag2 = std::move(bag1);

        CHECK(!bag2.empty());
        CHECK(sum(bag2) == 6);

        CHECK(bag1.empty());
        CHECK(sum(bag1) == 0);
    }
    SECTION("swap")
    {
        bag_t bag2;

        bag2.add(Data(10));
        bag2.add(Data(30));

        CHECK(!bag2.empty());
        CHECK(sum(bag2) == 30);

        std::swap(bag1, bag2);

        CHECK(!bag2.empty());
        CHECK(sum(bag2) == 6);

        CHECK(!bag1.empty());
        CHECK(sum(bag1) == 30);
    }
}

TEST_CASE("container.lockfree_bag. Clear")
{
    lockfree_grow_only_bag<int>         bag1;
    lockfree_grow_only_bag<Data>        bag2;
    lockfree_grow_only_bag<std::string> bag3;

    CHECK(bag1.empty());
    CHECK(bag2.empty());
    CHECK(bag3.empty());

    bag1.add();
    bag2.add();
    bag3.add();

    CHECK(!bag1.empty());
    CHECK(!bag2.empty());
    CHECK(!bag3.empty());

    bag1.clear();
    bag2.clear();
    bag3.clear();

    CHECK(bag1.empty());
    CHECK(bag2.empty());
    CHECK(bag3.empty());
}
