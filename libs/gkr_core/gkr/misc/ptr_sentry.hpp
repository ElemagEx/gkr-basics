#pragma once

#include <gkr/diagnostics.hpp>

namespace gkr
{
namespace misc
{

template<typename T=void, bool no_except=true>
class ptr_sentry
{
public:
    typedef void (*PFN)(T*) noexcept(no_except);

    ptr_sentry(const ptr_sentry* sentry) noexcept = delete;
    ptr_sentry& operator=(const ptr_sentry&) noexcept = delete;

    ptr_sentry(PFN pfn) noexcept(DIAG_NOEXCEPT) : m_ptr(nullptr), m_pfn(pfn)
    {
        Assert_NotNullPtr(pfn);
    }
    ptr_sentry(T* ptr, PFN pfn) noexcept(DIAG_NOEXCEPT) : m_ptr(ptr), m_pfn(pfn)
    {
        Assert_NotNullPtr(pfn);
    }
    ~ptr_sentry() noexcept(no_except)
    {
        reset();
    }
    ptr_sentry(ptr_sentry&& other) noexcept : m_ptr(other.m_ptr), m_pfn(other.m_pfn)
    {
        other.m_ptr = nullptr;
    }
    ptr_sentry& operator=(ptr_sentry&& other) noexcept(no_except)
    {
        if(&other == this) return *this;

        reset();

        m_ptr = other.ptr;
        m_pfn = other.pfn;

        other.m_ptr = nullptr;
        return *this;
    }

    T* get() const noexcept
    {
        return m_ptr;
    }
    template<typename U>
    U* get() const noexcept
    {
        return static_cast<U*>(m_ptr);
    }

    T* detach() noexcept
    {
        T* ptr = m_ptr;
        m_ptr = nullptr;
        return ptr;
    }
    template<typename U>
    U* detach() noexcept
    {
        U* ptr = static_cast<U*>(m_ptr);
        m_ptr = nullptr;
        return ptr;
    }

    void set(void* ptr) noexcept(no_except)
    {
        reset();

        m_ptr = ptr;
    }
    void reset() noexcept(no_except)
    {
        if(m_ptr == nullptr) return;
        (*m_pfn)(m_ptr);
        m_ptr = nullptr;
    }

private:
    T*  m_ptr;
    PFN m_pfn;
};

}
}
