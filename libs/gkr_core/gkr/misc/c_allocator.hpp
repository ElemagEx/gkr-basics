#pragma once

#include <memory>
#include <type_traits>

#ifndef gkr_attr_nodiscard
#if __has_cpp_attribute(no_unique_address)
#define gkr_attr_nodiscard [[nodiscard]]
#else
#define gkr_attr_nodiscard
#endif
#endif

namespace gkr
{
namespace misc
{

template<typename T>
class c_allocator
{
public:
    using propagate_on_container_copy_assignment = std::true_type;
    using propagate_on_container_move_assignment = std::true_type;
    using propagate_on_container_swap            = std::true_type;

    using value_type = T;

    using size_type       = std::size_t;
    using difference_type = std::ptrdiff_t;

private:
    void* (*m_alloc_proc)(std::size_t);
    void  (*m_free_proc )(void*);

public:
    bool invalid() const noexcept
    {
        return ((m_alloc_proc == nullptr) || (m_free_proc == nullptr));
    }
public:
    c_allocator() noexcept
        : m_alloc_proc(nullptr)
        , m_free_proc (nullptr)
    {
    }
    c_allocator(void* (*alloc_proc)(std::size_t), void (*free_proc)(void*)) noexcept
        : m_alloc_proc(alloc_proc)
        , m_free_proc (free_proc)
    {
    }
    ~c_allocator() noexcept
    {
    }

public:
    c_allocator(c_allocator&& other) noexcept : m_alloc_proc(other.m_alloc_proc), m_free_proc(other.m_free_proc)
    {
    }
    c_allocator(const c_allocator& other) noexcept : m_alloc_proc(other.m_alloc_proc), m_free_proc(other.m_free_proc)

    {
    }
    template<class U>
    c_allocator(const c_allocator<U>& other) noexcept : m_alloc_proc(other.m_alloc_proc), m_free_proc(other.m_free_proc)
    {
    }

public:
    void swap(c_allocator& other) noexcept
    {
        std::swap(m_alloc_proc, other.alloc_proc);
        std::swap(m_free_proc , other.free_proc );
    }

public:
    c_allocator& operator=(c_allocator&& other) noexcept
    {
        m_alloc_proc = other.alloc_proc;
        m_free_proc  = other.free_proc ;
        return *this;
    }
    c_allocator& operator=(const c_allocator& other) noexcept
    {
        m_alloc_proc = other.alloc_proc;
        m_free_proc  = other.free_proc ;
        return *this;
    }

public:
    bool operator==(const c_allocator& other) const noexcept
    {
        return (m_alloc_proc == other.alloc_proc) && (m_free_proc == other.free_proc);
    }
    bool operator!=(const c_allocator& other) const noexcept
    {
        return (m_alloc_proc != other.alloc_proc) || (m_free_proc != other.free_proc);
    }

public:
#ifdef __cpp_lib_allocate_at_least
    gkr_attr_nodiscard
    std::allocation_result<T*, std::size_t> allocate_at_least(std::size_t n) noexcept(false)
    {
        T* ptr = invalid() ? nullptr : static_cast<T*>((*m_alloc_proc)(sizeof(T) * n));

        if(ptr == nullptr) throw std::bad_alloc();

        return {ptr, n};
    }
#endif
    gkr_attr_nodiscard
    T* allocate(std::size_t n) noexcept(false)
    {
        T* ptr = invalid() ? nullptr : static_cast<T*>((*m_alloc_proc)(sizeof(T) * n));

        if(ptr == nullptr) throw std::bad_alloc();

        return ptr;
    }
    void deallocate(T* ptr, std::size_t n) noexcept
    {
        if(invalid()) return;

        (*m_free_proc)(ptr);
    }
};

}
}
