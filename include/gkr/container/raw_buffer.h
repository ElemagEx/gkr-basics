#pragma once

#include <memory>
#include <cstddef>
#include <cstring>
#include <utility>
#include <type_traits>

#ifndef GKR_RAW_BUFFER_SINGLE_HEADER
#if 0
#include <gkr/diag/diagnostics.h>
#endif
#ifndef __cpp_lib_exchange_function
#include <gkr/cpp/lib_exchange_function.h>
#endif

#else

#ifndef __cpp_lib_exchange_function
#error  You must use C++14 or preinclude implementation of std::exchange
#endif
#if 0
#include <cassert>

#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif
#ifndef Assert_NotNullPtr
#define Assert_NotNullPtr(cond) assert(cond)
#endif
#ifndef Check_Recovery
#define Check_Recovery(msg)
#endif
#endif
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
using a = std::max_align_t;
template<typename Allocator=std::allocator<char>>
class raw_buffer
{
    using allocator_traits = std::allocator_traits<Allocator>;

    using value_t = typename allocator_traits::value_type;

    static constexpr std::size_t granularity = (sizeof(value_t) >= alignof(value_t))
        ?  sizeof(value_t)
        : alignof(value_t)
        ;
public:
    static constexpr std::size_t alignment = (alignof(std::max_align_t) >= alignof(value_t))
        ? alignof(std::max_align_t)
        : alignof(value_t)
        ;

    static constexpr bool move_constructor_is_nothrow = (
        std::is_nothrow_move_constructible<Allocator>::value
        );
    static constexpr bool move_assignment_is_nothrow = (
        allocator_traits::is_always_equal::value
        ||
        (allocator_traits::propagate_on_container_move_assignment::value &&
        std::is_nothrow_move_constructible<Allocator>::value)
        );
    static constexpr bool swap_is_nothrow = (
        allocator_traits::is_always_equal::value
        ||
        (allocator_traits::propagate_on_container_swap::value &&
        std::is_nothrow_move_constructible<Allocator>::value &&
        std::is_nothrow_move_assignable   <Allocator>::value)
        ||
        (move_constructor_is_nothrow && move_assignment_is_nothrow)
        );

private:
    char*       m_data     = nullptr;
    std::size_t m_size     = 0;
    std::size_t m_capacity = 0;

    [[no_unique_address]]
    Allocator   m_allocator;

public:
    raw_buffer(std::size_t capacity, const Allocator& allocator = Allocator())
        : m_allocator(allocator)
    {
        reserve(capacity);
    }
    raw_buffer(const Allocator& allocator = Allocator()) noexcept(
        std::is_nothrow_copy_constructible<Allocator>::value
        )
        : m_allocator(allocator)
    {
    }
    ~raw_buffer()
    {
        clear();
    }

public:
    raw_buffer(const raw_buffer& other, const Allocator& allocator)
        : m_allocator(allocator)
    {
        reserve(other.m_capacity);

        m_size = other.m_size;

        if(m_size > 0)
        {
            std::memcpy(m_data, other.m_data, m_size);
        }
    }
    raw_buffer(const raw_buffer& other)
        : m_allocator(allocator_traits::select_on_container_copy_construction(other.m_allocator))
    {
        reserve(other.m_capacity);

        m_size = other.m_size;

        if(m_size > 0)
        {
            std::memcpy(m_data, other.m_data, m_size);
        }
    }
    raw_buffer& operator=(const raw_buffer& other)
    {
        if(this != &other)
        {
            if_constexpr(allocator_traits::propagate_on_container_copy_assignment::value)
            {
                clear();
                m_allocator = other.m_allocator;
            }
            reserve(other.m_capacity);

            m_size = other.m_size;

            if(m_size > 0)
            {
                std::memcpy(m_data, other.m_data, m_size);
            }
        }
        return *this;
    }

public:
    raw_buffer(raw_buffer&& other, const Allocator& allocator) noexcept(
        std::is_nothrow_copy_constructible<Allocator>::value
        )
        : m_allocator(allocator)
    {
        if_constexpr(allocator_traits::is_always_equal::value)
        {
            m_data     = std::exchange(other.m_data    , nullptr);
            m_size     = std::exchange(other.m_size    , 0);
            m_capacity = std::exchange(other.m_capacity, 0);
        }
        else if(m_allocator == other.m_allocator)
        {
            m_data     = std::exchange(other.m_data    , nullptr);
            m_size     = std::exchange(other.m_size    , 0);
            m_capacity = std::exchange(other.m_capacity, 0);
        }
        else
        {
            reserve(other.m_capacity);

            m_size = other.m_size;

            if(m_size > 0)
            {
                std::memcpy(m_data, other.m_data, m_size);
            }
            other.clear();
        }
    }
    raw_buffer(raw_buffer&& other) noexcept(move_constructor_is_nothrow)
        : m_allocator(std::move(other.m_allocator))
    {
        m_data     = std::exchange(other.m_data    , nullptr);
        m_size     = std::exchange(other.m_size    , 0);
        m_capacity = std::exchange(other.m_capacity, 0);
    }

    raw_buffer& operator=(raw_buffer&& other) noexcept(move_assignment_is_nothrow)
    {
        if(this != &other)
        {
            if_constexpr(allocator_traits::propagate_on_container_move_assignment::value)
            {
                m_allocator = std::move(other.m_allocator);

                m_data     = std::exchange(other.m_data    , nullptr);
                m_size     = std::exchange(other.m_size    , 0);
                m_capacity = std::exchange(other.m_capacity, 0);
            }
            else if_constexpr(allocator_traits::is_always_equal::value)
            {
                m_data     = std::exchange(other.m_data    , nullptr);
                m_size     = std::exchange(other.m_size    , 0);
                m_capacity = std::exchange(other.m_capacity, 0);
            }
            else if(m_allocator == other.m_allocator)
            {
                m_data     = std::exchange(other.m_data    , nullptr);
                m_size     = std::exchange(other.m_size    , 0);
                m_capacity = std::exchange(other.m_capacity, 0);
            }
            else
            {
                reserve(other.m_capacity);

                m_size = other.m_size;

                if(m_size > 0)
                {
                    std::memcpy(m_data, other.m_data, m_size);
                }
                other.clear();
            }
        }
        return *this;
    }

public:
    void swap(raw_buffer& other) noexcept(swap_is_nothrow)
    {
        if(this != &other)
        {
            if_constexpr(allocator_traits::propagate_on_container_swap::value)
            {
                std::swap(m_allocator, other.m_allocator);

                std::swap(m_data    , other.m_data);
                std::swap(m_size    , other.m_size);
                std::swap(m_capacity, other.m_capacity);
            }
            else if_constexpr(allocator_traits::is_always_equal::value)
            {
                std::swap(m_data    , other.m_data);
                std::swap(m_size    , other.m_size);
                std::swap(m_capacity, other.m_capacity);
            }
            else if(m_allocator == other.m_allocator)
            {
                std::swap(m_data    , other.m_data);
                std::swap(m_size    , other.m_size);
                std::swap(m_capacity, other.m_capacity);
            }
            else
            {
                raw_buffer buffer(other);
                other = std::move(*this);
                *this = std::move(buffer);
            }
        }
    }

public:
    Allocator get_allocator() const noexcept(std::is_nothrow_copy_constructible<Allocator>::value)
    {
        return Allocator(m_allocator);
    }

public:
    std::size_t size() const noexcept
    {
        return m_size;
    }
    std::size_t capacity() const noexcept
    {
        return m_capacity;
    }

public:
    void clear()
    {
        if(m_data != nullptr)
        {
            const std::size_t count = m_capacity / granularity;

            m_allocator.deallocate(reinterpret_cast<value_t*>(m_data), count);

            m_data     = nullptr;
            m_size     = 0;
            m_capacity = 0;
        }
    }
    void reserve(std::size_t capacity)
    {
        if(capacity <= m_capacity) return;

        const std::size_t count = ((capacity % granularity) == 0)
            ? (capacity / granularity + 0)
            : (capacity / granularity + 1)
            ;

        char* data = reinterpret_cast<char*>(m_allocator.allocate(count));

        if(m_data != nullptr)
        {
            std::memcpy(data, m_data, m_size);
            clear();
        }
        m_data     = data;
        m_capacity = count * granularity;
    }
    void resize(std::size_t size)
    {
        if(size > m_capacity)
        {
            reserve(size);
        }
        m_size = size;
    }

public:
    const void* data(size_t offset = 0) const noexcept
    {
        return (m_data + offset);
    }
    void* data(size_t offset = 0) noexcept
    {
        return (m_data + offset);
    }

public:
	template<typename T>
    const T* data(size_t offset = 0) const
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        return reinterpret_cast<const T*>(m_data + offset);
    }
	template<typename T>
    T* data(size_t offset = 0)
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        return reinterpret_cast<T*>(m_data + offset);
    }

	template<typename T>
    const T* ptr(size_t index) const
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        return reinterpret_cast<const T*>(m_data) + index;
    }
	template<typename T>
    T* ptr(size_t index)
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        return reinterpret_cast<T*>(m_data) + index;
    }
	template<typename T>
    const T& at(size_t index) const
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        return reinterpret_cast<const T*>(m_data)[index];
    }
	template<typename T>
    T& at(size_t index)
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        return reinterpret_cast<T*>(m_data)[index];
    }
};

}

namespace std
{
template<typename Allocator>
void swap(
    gkr::raw_buffer<Allocator>& lhs,
    gkr::raw_buffer<Allocator>& rhs
    )
    noexcept(gkr::raw_buffer<Allocator>::swap_is_nothrow)
{
    lhs.swap(rhs);
}
}
