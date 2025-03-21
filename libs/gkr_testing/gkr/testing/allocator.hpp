#pragma once

#include <new>
#include <mutex>
#include <atomic>
#include <cstddef>
#include <cstring>
#include <stdexcept>

#ifndef CHECK
#include <cassert>
#define CHECK assert
#endif
#ifndef REQUIRE
#include <cassert>
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

enum allocator_flag
{
    SingleThreaded = 0x00,
    MultiThreaded  = 0x01,

    PropagatesNever            = 0x00,
    PropagatesOnCopyAssignment = 0x02,
    PropagatesOnMoveAssignment = 0x04,
    PropagatesOnSwap           = 0x08,
    PropagatesAlways           = (PropagatesOnCopyAssignment | PropagatesOnMoveAssignment | PropagatesOnSwap),

    EqualsNever   = 0x00,
    EqualsAlways  = 0x10,
    EqualsByValue = 0x20,

    ExceptDestructor        = 0x0100,
    ExceptSwapOperation     = 0x0200,
    ExceptCopyOperatorEq    = 0x0400,
    ExceptMoveOperatorEq    = 0x0800,
    ExceptDefConstructor    = 0x1000,
    ExceptCopyConstructor   = 0x2000,
    ExceptMoveConstructor   = 0x4000,
    ExceptRebindConstructor = 0x8000,
};

class objects_ref_counting
{
protected:
    inline static std::atomic_int m_allocations   { 0 };
    inline static std::atomic_int m_constructions { 0 };

public:
    static bool check() noexcept
    {
        return ((m_allocations == 0) && (m_constructions == 0));
    }
};

template<typename T>
class ref_counting_allocator : public objects_ref_counting
{
public:
    using      value_type = T;
    using       size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

public:
    ref_counting_allocator() noexcept = default;
   ~ref_counting_allocator() noexcept = default;

    ref_counting_allocator(const ref_counting_allocator<T>& ) noexcept = default;
    ref_counting_allocator(      ref_counting_allocator<T>&&) noexcept = default;

    template<class U>
    ref_counting_allocator(const ref_counting_allocator<U>& other) noexcept
    {
    }

    ref_counting_allocator& operator=(const ref_counting_allocator& ) noexcept = default;
    ref_counting_allocator& operator=(      ref_counting_allocator&&) noexcept = default;

    void swap(ref_counting_allocator&) noexcept {}

public:
#ifdef __cpp_lib_allocate_at_least
    [[nodiscard]]
    std::allocation_result<T*, std::size_t> allocate_at_least(std::size_t n) noexcept(false)
    {
        if(n >= 1024*1024*1024) throw std::bad_alloc();

        T* ptr = static_cast<T*>(std::malloc(sizeof(T) * n));

        if(ptr == nullptr) throw std::bad_alloc();

        m_allocations.fetch_add(int(n));

        return {ptr, n};
    }
#endif
    [[nodiscard]]
    T* allocate(std::size_t n) noexcept(false)
    {
        T* ptr = static_cast<T*>(std::malloc(sizeof(T) * n));

        if(ptr == nullptr) throw std::bad_alloc();

        m_allocations.fetch_add(int(n));

        return ptr;
    }
    void deallocate(T* ptr, std::size_t n) noexcept
    {
        if((m_allocations.fetch_add(-int(n)) - int(n)) < 0)
        {
            Assert_Failure();
        }
        std::free(ptr);
    }
public:
    template<class U, class... Args>
    void construct(U* ptr, Args&&... args) noexcept(std::is_nothrow_constructible<U>::value)
    {
        ::new(ptr) U(std::forward<Args>(args)...);

        if(++m_constructions > m_allocations)
        {
            Assert_Failure();
        }
    }
    template<class U>
    void destroy(U* ptr) noexcept(std::is_nothrow_destructible<U>::value)
    {
        if(--m_constructions < 0)
        {
            Assert_Failure();
        }
        ptr->~U();
    }
};
template<typename T, int FLAGS>
class ref_counting_allocator_ex : public ref_counting_allocator<T>
{
public:
    using is_always_equal = std::integral_constant<bool, (FLAGS & EqualsAlways) != 0>;

    using propagate_on_container_copy_assignment = std::integral_constant<bool, (FLAGS & PropagatesOnCopyAssignment) != 0>;
    using propagate_on_container_move_assignment = std::integral_constant<bool, (FLAGS & PropagatesOnMoveAssignment) != 0>;
    using propagate_on_container_swap            = std::integral_constant<bool, (FLAGS & PropagatesOnSwap          ) != 0>;

protected:
    ref_counting_allocator_ex() noexcept(std::integral_constant<bool, (FLAGS & ExceptDestructor    ) == 0>::value) {}
   ~ref_counting_allocator_ex() noexcept(std::integral_constant<bool, (FLAGS & ExceptDefConstructor) == 0>::value) {}

    ref_counting_allocator_ex(const ref_counting_allocator_ex& ) noexcept(std::integral_constant<bool, (FLAGS & ExceptCopyConstructor) == 0>::value) {}
    ref_counting_allocator_ex(      ref_counting_allocator_ex&&) noexcept(std::integral_constant<bool, (FLAGS & ExceptMoveConstructor) == 0>::value) {}

    template<class U>
    ref_counting_allocator_ex(const ref_counting_allocator<U>& other) noexcept(std::integral_constant<bool, (FLAGS & ExceptRebindConstructor) == 0>::value) {}

    ref_counting_allocator_ex& operator=(const ref_counting_allocator_ex& ) noexcept(std::integral_constant<bool, (FLAGS & ExceptCopyOperatorEq) == 0>::value) { return *this; }
    ref_counting_allocator_ex& operator=(      ref_counting_allocator_ex&&) noexcept(std::integral_constant<bool, (FLAGS & ExceptMoveOperatorEq) == 0>::value) { return *this; }

    void swap(ref_counting_allocator_ex&) noexcept(std::integral_constant<bool, (FLAGS & ExceptSwapOperation) == 0>::value) {}

protected:
    bool equals_by_value() const noexcept
    {
        return ((FLAGS & (EqualsAlways | EqualsByValue)) != 0);
    }
};

}
}

#define DEFINE_TEST_ALLOCATOR(NAME, FLAGS) \
template<typename T>                                                                                                                                    \
class NAME : public gkr::testing::ref_counting_allocator_ex<T, FLAGS>                                                                                   \
{                                                                                                                                                       \
    using base_t = gkr::testing::ref_counting_allocator_ex<T, FLAGS>;                                                                                   \
public:                                                                                                                                                 \
   ~NAME() noexcept(std::is_nothrow_destructible<base_t>::value) {}                                                                                     \
    NAME() noexcept(std::is_nothrow_constructible<base_t>::value) : base_t() {}                                                                         \
    NAME(const NAME&  other) noexcept(std::is_nothrow_copy_constructible<base_t>::value) : base_t(          other ) {}                                  \
    NAME(      NAME&& other) noexcept(std::is_nothrow_move_constructible<base_t>::value) : base_t(std::move(other)) {}                                  \
    NAME& operator=(const NAME&  other) noexcept(std::is_nothrow_copy_assignable<base_t>::value) { base_t::operator=(          other ); return *this; } \
    NAME& operator=(      NAME&& other) noexcept(std::is_nothrow_move_assignable<base_t>::value) { base_t::operator=(std::move(other)); return *this; } \
    bool operator==(const NAME&) const noexcept { return  base_t::equals_by_value(); }                                                                  \
    bool operator!=(const NAME&) const noexcept { return !base_t::equals_by_value(); }                                                                  \
    void swap(NAME& other) noexcept(std::is_nothrow_swappable<base_t>::value) { base_t::swap(other); }                                                  \
    template<typename U> NAME(const NAME<U>& other) noexcept(std::is_nothrow_constructible<base_t, gkr::testing::ref_counting_allocator_ex<U, FLAGS>>::value) : base_t(other) {} \
}
