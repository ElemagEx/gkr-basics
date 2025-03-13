#pragma once

#include <memory>
#include <cstddef>
#include <cstring>
#include <utility>
#include <type_traits>

#ifndef GKR_RAW_BUFFER_SINGLE_HEADER

#include <gkr/diagnostics.hpp>

#ifndef __cpp_lib_exchange_function
#include <gkr/cpp/lib_exchange_function.hpp>
#endif
#ifndef __cpp_lib_is_swappable
#include <gkr/cpp/lib_is_swappable.hpp>
#endif

#else

#ifndef __cpp_lib_is_swappable
namespace std
{
template<typename T>
struct is_nothrow_swappable
{
    static constexpr bool value =
        std::is_nothrow_move_constructible<T>::value &&
        std::is_nothrow_move_assignable<T>::value;
};
}
#endif
#ifndef __cpp_lib_exchange_function
namespace std
{
template<class T, class U=T>
T exchange(T& obj, U&& new_value) noexcept(
    std::is_nothrow_move_constructible<T>::value &&
    std::is_nothrow_assignable<T&, U>::value
    )
{
    T old_value = std::move(obj);
    obj = std::forward<U>(new_value);
    return old_value;
}
}
#endif

#include <cassert>

#ifndef DIAG_NOEXCEPT
#define DIAG_NOEXCEPT true
#endif
#ifndef Assert_NotNullPtr
#define Assert_NotNullPtr(cond) assert(cond)
#endif
#ifndef Check_Arg_IsValid
#define Check_Arg_IsValid(check, ...) if(!(check)) return __VA_ARGS__
#endif

#endif

#ifndef if_constexpr
#ifdef __cpp_if_constexpr
#define if_constexpr if constexpr
#else
#define if_constexpr if
#endif
#endif
#ifndef gkr_attr_no_unique_address
#if __has_cpp_attribute(no_unique_address)
#define gkr_attr_no_unique_address [[no_unique_address]]
#else
#define gkr_attr_no_unique_address
#endif
#endif

namespace gkr
{

template<typename Allocator>
class basic_raw_buffer
{
    using allocator_traits = std::allocator_traits<Allocator>;

    using allocator_value_t = typename allocator_traits::value_type;

    static constexpr std::size_t granularity = (sizeof(allocator_value_t) >= alignof(allocator_value_t))
        ?  sizeof(allocator_value_t)
        : alignof(allocator_value_t)
        ;
public:
    static constexpr std::size_t alignment = alignof(allocator_value_t);

    static constexpr bool move_constructor_is_nothrow = (
        std::is_nothrow_move_constructible<Allocator>::value
        );
    static constexpr bool move_assignment_is_nothrow = (
        allocator_traits::is_always_equal::value
        ||
        (allocator_traits::propagate_on_container_move_assignment::value && std::is_nothrow_move_constructible<Allocator>::value)
        );
    static constexpr bool swap_is_nothrow = (
        allocator_traits::is_always_equal::value
        ||
        (allocator_traits::propagate_on_container_swap::value && std::is_nothrow_swappable<Allocator>::value)
        );

private:
    char*       m_data     = nullptr;
    std::size_t m_size     = 0;
    std::size_t m_capacity = 0;

    gkr_attr_no_unique_address Allocator m_allocator;

public:
    basic_raw_buffer(std::size_t capacity, const Allocator& allocator = Allocator())
        : m_allocator(allocator)
    {
        reserve(capacity);
    }
    basic_raw_buffer(const Allocator& allocator = Allocator()) noexcept(
        std::is_nothrow_copy_constructible<Allocator>::value
        )
        : m_allocator(allocator)
    {
    }
    ~basic_raw_buffer()
    {
        clear();
    }

public:
    basic_raw_buffer(const basic_raw_buffer& other, const Allocator& allocator)
        : m_allocator(allocator)
    {
        copy_data(other.m_data, other.m_size, other.m_capacity);
    }
    basic_raw_buffer(const basic_raw_buffer& other)
        : m_allocator(allocator_traits::select_on_container_copy_construction(other.m_allocator))
    {
        copy_data(other.m_data, other.m_size, other.m_capacity);
    }
    basic_raw_buffer& operator=(const basic_raw_buffer& other)
    {
        if(this != &other)
        {
            clear();
            if_constexpr(allocator_traits::propagate_on_container_copy_assignment::value)
            {
                m_allocator = other.m_allocator;
            }
            copy_data(other.m_data, other.m_size, other.m_capacity);
        }
        return *this;
    }

public:
    basic_raw_buffer(basic_raw_buffer&& other, const Allocator& allocator) noexcept(
        std::is_nothrow_copy_constructible<Allocator>::value && allocator_traits::is_always_equal::value
        )
        : m_allocator(allocator)
    {
        if_constexpr(!allocator_traits::is_always_equal::value)
        {
            if(m_allocator != other.m_allocator)
            {
                other.relocate_data(m_data, m_size, m_capacity, m_allocator);
                return;
            }
        }
        m_data     = std::exchange(other.m_data    , nullptr);
        m_size     = std::exchange(other.m_size    , 0);
        m_capacity = std::exchange(other.m_capacity, 0);
    }
    basic_raw_buffer(basic_raw_buffer&& other) noexcept(move_constructor_is_nothrow)
        : m_allocator(std::move(other.m_allocator))
    {
        m_data     = std::exchange(other.m_data    , nullptr);
        m_size     = std::exchange(other.m_size    , 0U);
        m_capacity = std::exchange(other.m_capacity, 0U);
    }
    basic_raw_buffer& operator=(basic_raw_buffer&& other) noexcept(move_assignment_is_nothrow)
    {
        if(this != &other)
        {
            if_constexpr(allocator_traits::propagate_on_container_move_assignment::value)
            {
                m_allocator = std::move(other.m_allocator);
            }
            else if_constexpr(!allocator_traits::is_always_equal::value)
            {
                if(m_allocator != other.m_allocator)
                {
                    clear();
                    other.relocate_data(m_data, m_size, m_capacity, m_allocator);
                    return *this;
                }
            }
            m_data     = std::exchange(other.m_data    , nullptr);
            m_size     = std::exchange(other.m_size    , 0U);
            m_capacity = std::exchange(other.m_capacity, 0U);
        }
        return *this;
    }

public:
    void swap(basic_raw_buffer& other) noexcept(swap_is_nothrow)
    {
        if(this != &other)
        {
            if_constexpr(allocator_traits::propagate_on_container_swap::value)
            {
                std::swap(m_allocator, other.m_allocator);
            }
            else if_constexpr(!allocator_traits::is_always_equal::value)
            {
                if(m_allocator != other.m_allocator)
                {
                    char* data;
                    std::size_t size;
                    std::size_t capacity;

                    this->relocate_data(  data,   size,   capacity, other.m_allocator);
                    other.relocate_data(m_data, m_size, m_capacity,       m_allocator);

                    other.m_data     = data;
                    other.m_size     = size;
                    other.m_capacity = capacity;
                    return;
                }
            }
            std::swap(m_data    , other.m_data);
            std::swap(m_size    , other.m_size);
            std::swap(m_capacity, other.m_capacity);
        }
    }

private:
    void copy_data(const char* data, std::size_t size, std::size_t capacity, Allocator& allocator)
    {
        reserve(capacity);

        m_size = size;

        std::memcpy(m_data, data, m_size);
    }
    void relocate_data(char*& data, std::size_t& size, std::size_t& capacity, Allocator& allocator)
    {
        if(m_capacity == 0)
        {
            data     = nullptr;
            size     = 0;
            capacity = 0;
        }
        else
        {
            const std::size_t count = ((m_capacity % granularity) == 0)
                ? (m_capacity / granularity + 0)
                : (m_capacity / granularity + 1)
                ;
            data     = reinterpret_cast<char*>(allocator.allocate(count));
            size     = m_size;
            capacity = count * granularity;

            std::memcpy(data, m_data, m_size);
            clear();
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
        if(m_data == nullptr) return;

        const std::size_t count = ((m_capacity % granularity) == 0)
            ? (m_capacity / granularity + 0)
            : (m_capacity / granularity + 1)
            ;
        m_allocator.deallocate(reinterpret_cast<allocator_value_t*>(m_data), count);

        m_data     = nullptr;
        m_size     = 0;
        m_capacity = 0;
    }
    void reserve(std::size_t capacity)
    {
        if(capacity <= m_capacity) return;

        const std::size_t size  = m_size;
        const std::size_t count = ((capacity % granularity) == 0)
            ? (capacity / granularity + 0)
            : (capacity / granularity + 1)
            ;
        char* data = reinterpret_cast<char*>(m_allocator.allocate(count));

        std::memcpy(data, m_data, m_size);
        clear();

        m_data     = data;
        m_size     = size;
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
    bool change_size(std::size_t size) noexcept(DIAG_NOEXCEPT)
    {
        Check_Arg_IsValid(size <= m_capacity, false);
        m_size = size;
        return true;
    }

public:
    const void* data(std::size_t offset = 0) const noexcept
    {
        return (m_data + offset);
    }
    void* data(std::size_t offset = 0) noexcept
    {
        return (m_data + offset);
    }

public:
    template<typename T>
    const T* data(std::size_t offset = 0) const noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check((std::size_t(m_data + offset) % alignof(T)) == 0);
        return reinterpret_cast<const T*>(m_data + offset);
    }
    template<typename T>
    T* data(std::size_t offset = 0) noexcept(DIAG_NOEXCEPT)
    {
        Assert_Check((std::size_t(m_data + offset) % alignof(T)) == 0);
        return reinterpret_cast<T*>(m_data + offset);
    }

    template<typename T>
    const T& as(std::size_t offset = 0) const noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_data);
        Assert_Check((std::size_t(m_data + offset) % alignof(T)) == 0);
        return *reinterpret_cast<const T*>(m_data + offset);
    }
    template<typename T>
    T& as(std::size_t offset = 0) noexcept(DIAG_NOEXCEPT)
    {
        Assert_NotNullPtr(m_data);
        Assert_Check((std::size_t(m_data + offset) % alignof(T)) == 0);
        return *reinterpret_cast<T*>(m_data + offset);
    }

    template<typename T>
    const T* ptr(std::size_t index) const noexcept
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        return reinterpret_cast<const T*>(m_data) + index;
    }
    template<typename T>
    T* ptr(std::size_t index) noexcept
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        return reinterpret_cast<T*>(m_data) + index;
    }
    template<typename T>
    const T& at(std::size_t index) const noexcept(DIAG_NOEXCEPT)
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        Assert_NotNullPtr(m_data);
        return reinterpret_cast<const T*>(m_data)[index];
    }
    template<typename T>
    T& at(std::size_t index) noexcept(DIAG_NOEXCEPT)
    {
        static_assert(alignof(T) <= alignment, "Alignment mismatch");
        Assert_NotNullPtr(m_data);
        return reinterpret_cast<T*>(m_data)[index];
    }
};

using raw_buffer = basic_raw_buffer<std::allocator<std::max_align_t>>;

}

namespace std
{
template<typename Allocator>
void swap(
    gkr::basic_raw_buffer<Allocator>& lhs,
    gkr::basic_raw_buffer<Allocator>& rhs
    )
    noexcept(gkr::basic_raw_buffer<Allocator>::swap_is_nothrow)
{
    lhs.swap(rhs);
}
}
