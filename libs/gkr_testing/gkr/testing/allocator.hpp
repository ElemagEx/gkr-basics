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
namespace impl
{
struct fake_mutex
{
    void lock  () {}
    void unlock() {}
    [[nodiscard]] bool try_lock() { return true; }
};
template<bool> struct actual_mutex;
template<> struct actual_mutex<false> { using type = fake_mutex; };
template<> struct actual_mutex<true > { using type = std::mutex; };
}

template<typename T, bool MULTITHREADED>
class pre_allocated_storage
{
    static_assert(alignof(T) <= alignof(std::max_align_t), "Only supports types that have alignment not greater than alignment of std::max_align_t");

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

    using mutex_t = typename impl::actual_mutex<MULTITHREADED>::type;

    mutex_t m_mutex;

private:
    void make_checks()
    {
        CHECK(m_total_allocations == 0);
        CHECK(m_total_elements    == 0);
    }
    void free_storage()
    {
        if(m_ptrs != nullptr) std::free(m_ptrs);
        if(m_data != nullptr) std::free(m_data);
    }

private:
    pre_allocated_storage()
    {
    }
    ~pre_allocated_storage()
    {
        free_storage();
    }

public:
    static pre_allocated_storage& get_pre_allocated_storage()
    {
        static thread_local pre_allocated_storage storage;
        return storage;
    }

public:
    void reset(std::size_t count = 0, bool count_is_bytes=false)
    {
        if(count_is_bytes)
        {
            REQUIRE(alignof(T) <= sizeof(T));
            count = ((count % sizeof(T)) == 0)
                ? (0+(count / sizeof(T)))
                : (1+(count / sizeof(T)))
                ;
        }

        std::lock_guard<mutex_t> guard(m_mutex);

        make_checks();

        m_total_allocations = 0;
        m_total_elements    = 0;

        if(count > m_count)
        {
            free_storage();

            constexpr std::size_t size1 = (alignof(T) >= sizeof(T)) ? alignof(T) : sizeof(T);
            constexpr std::size_t size2 =  sizeof(T*);

            m_data = static_cast<T* >(std::malloc(count * size1));
            m_ptrs = static_cast<T**>(std::malloc(count * size2));

            if((m_data == nullptr) || (m_ptrs == nullptr))
            {
                throw std::bad_alloc();
            }

            std::memset(m_ptrs, 0, count * size2);

            m_count = count;
        }
    }

public:
    T* allocate(std::size_t n)
    {
        std::lock_guard<mutex_t> guard(m_mutex);

        if((n < 1) || (n > m_count))
        {
            throw std::bad_alloc();
        }
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
        std::lock_guard<mutex_t> guard(m_mutex);

        std::size_t index = std::size_t(ptr - m_data);

        if( !((index + 0) <  m_count) ||
            !((index + n) <= m_count))
        {
            throw std::invalid_argument("ptr or n is invalid (or both)");
        }
        m_total_allocations -= 1;
        m_total_elements    -= int(n);

        while(n-- > 0)
        {
            m_ptrs[index++] = nullptr;
        }
    }
};

template<typename T>
pre_allocated_storage<T, false>& get_singlethreaded_pre_allocated_storage()
{
    return pre_allocated_storage<T, false>::get_pre_allocated_storage();
}
template<typename T>
pre_allocated_storage<T, true>& get_multithreaded_pre_allocated_storage()
{
    return pre_allocated_storage<T, true>::get_pre_allocated_storage();
}

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

template<typename T, int FLAGS>
class allocator
{
public:
    using      value_type = T;
    using       size_type = std::size_t;
    using difference_type = std::ptrdiff_t;

public:
    using is_always_equal = std::integral_constant<bool, (FLAGS & EqualsAlways) != 0>;

    using propagate_on_container_copy_assignment = std::integral_constant<bool, (FLAGS & PropagatesOnCopyAssignment) != 0>;
    using propagate_on_container_move_assignment = std::integral_constant<bool, (FLAGS & PropagatesOnMoveAssignment) != 0>;
    using propagate_on_container_swap            = std::integral_constant<bool, (FLAGS & PropagatesOnSwap          ) != 0>;

private:
    static constexpr bool MULTITHREADED = ((FLAGS & MultiThreaded) != 0);

    using pre_allocated_storage_t = pre_allocated_storage<T, MULTITHREADED>;

    pre_allocated_storage_t& m_pre_allocated_storage;

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
        : m_pre_allocated_storage(pre_allocated_storage_t::get_pre_allocated_storage())
    {
    }
    ~allocator()
    {
        reset();
    }

    template<typename U>
    allocator(const allocator<U, FLAGS>&) noexcept
        : m_pre_allocated_storage(pre_allocated_storage_t::get_pre_allocated_storage())
    {
    }
    allocator(const allocator& other) noexcept
        : m_pre_allocated_storage(other.m_pre_allocated_storage)
    {
    }
    allocator& operator=(const allocator& other) noexcept
    {
        reset();
        return *this;
    }

    allocator(allocator&& other) noexcept
        : m_pre_allocated_storage(other.m_pre_allocated_storage)
        , m_self_allocations(std::exchange(other.m_self_allocations, 0))
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
        return m_pre_allocated_storage.allocate(n);
    }
    void deallocate(T* ptr, std::size_t n)
    {
        m_pre_allocated_storage.deallocate(ptr, n);
    }
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

#define DEFINE_REF_COUNTING_ALLOCATOR(NAME, FLAGS) \
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
