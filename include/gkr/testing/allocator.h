#pragma once

#include <new>

#if !defined(CHECK) || !defined(REQUIRE)
#include <cassert>
#endif
#ifndef CHECK
#define CHECK assert
#endif
#ifndef REQUIRE
#define REQUIRE assert
#endif

#ifndef if_constexpr
#ifdef __cpp_if_constexpr
#define if_constexpr if constexpr
#else
#define if_constexpr if
#endif
#endif

namespace gkr
{
namespace testing
{

template<typename T>
class pre_allocated_storage
{
    static_assert(alignof(T) <= alignof(std::max_align_t), "Only supports types that have align not greater than align of std::max_align_t");

    pre_allocated_storage           (pre_allocated_storage&&) noexcept = delete;
    pre_allocated_storage& operator=(pre_allocated_storage&&) noexcept = delete;

    pre_allocated_storage           (const pre_allocated_storage&) noexcept = delete;
    pre_allocated_storage& operator=(const pre_allocated_storage&) noexcept = delete;

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
    pre_allocated_storage()
    {
    }
    ~pre_allocated_storage()
    {
        if(m_ptrs != nullptr) std::free(m_ptrs);
        if(m_data != nullptr) std::free(m_data);
    }

public:
    void reset(std::size_t count = 0)
    {
        make_checks();

        m_total_allocations = 0;
        m_total_elements    = 0;

        if(count > m_count)
        {
            if(m_ptrs != nullptr) std::free(m_ptrs);
            if(m_data != nullptr) std::free(m_data);

            constexpr std::size_t size1 = (alignof(T) >= sizeof(T)) ? alignof(T) : sizeof(T);
            constexpr std::size_t size2 =  sizeof(T*);

            m_data = static_cast<T* >(std::malloc(count * size1));
            m_ptrs = static_cast<T**>(std::malloc(count * size2));

            std::memset(m_ptrs, 0, count * size2);

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

                for(std::size_t i = 0; i < n; ++i)
                {
                    m_ptrs[index + i] = m_data + index + i;
                }
                return (m_data + index);
            }
        }
        throw std::bad_alloc();
    }
    void deallocate(T* ptr, std::size_t n)
    {
        std::size_t index = std::size_t(ptr - m_data);

        REQUIRE((index + 0) <  m_count);
        REQUIRE((index + n) <= m_count);

        m_total_allocations -= 1;
        m_total_elements    -= int(n);

        while(n-- > 0)
        {
            m_ptrs[index++] = nullptr;
        }
    }
};

template<typename T>
pre_allocated_storage<T>& get_this_thread_preallocated_storage()
{
    static thread_local pre_allocated_storage<T> storage;

    return storage;
}

enum allocator_flag
{
    PropagatesNever            = 0x00,
    PropagatesOnCopyAssignment = 0x01,
    PropagatesOnMoveAssignment = 0x02,
    PropagatesOnSwap           = 0x04,
    PropagatesAlways           = (PropagatesOnCopyAssignment | PropagatesOnMoveAssignment | PropagatesOnSwap),

    EqualsNever   = 0x00,
    EqualsAlways  = 0x10,
    EqualsByValue = 0x20,
};

template<typename T, int FLAGS>
class allocator
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
        constexpr bool check =
            propagate_on_container_copy_assignment::value ||
            propagate_on_container_move_assignment::value;

        if_constexpr(check)
        {
            CHECK(m_self_allocations == 0);
            CHECK(m_self_elements    == 0);
        }
        m_self_allocations = 0;
        m_self_elements    = 0;
    }

protected:
    allocator() noexcept
    {
    }
    ~allocator()
    {
        reset();
    }

    template<typename U>
    allocator(const allocator<U, FLAGS>&) noexcept
    {
    }
    allocator(const allocator&) noexcept
    {
    }
    allocator& operator=(const allocator&) noexcept
    {
        reset();
        return *this;
    }

    allocator(allocator&& other) noexcept
        : m_self_allocations(std::exchange(other.m_self_allocations, 0))
        , m_self_elements   (std::exchange(other.m_self_elements   , 0))
    {
    }
    allocator& operator=(allocator&& other) noexcept
    {
        m_self_allocations = std::exchange(other.m_self_allocations, 0);
        m_self_elements    = std::exchange(other.m_self_elements   , 0);
        return *this;
    }

    void swap(allocator& other) noexcept
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
        return get_this_thread_preallocated_storage<T>().allocate(n);
    }
    void deallocate(T* ptr, std::size_t n)
    {
        get_this_thread_preallocated_storage<T>().deallocate(ptr, n);
    }
};

}
}

#define DEFINE_TEST_ALLOCATOR(NAME, FLAGS) \
template<typename T>                                                                                    \
class NAME : public gkr::testing::allocator<T, FLAGS>                                                   \
{                                                                                                       \
public:                                                                                                 \
    using base_t = gkr::testing::allocator<T, FLAGS>;                                                   \
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
