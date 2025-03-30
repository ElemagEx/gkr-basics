#pragma once

#include <gkr/api.hpp>

namespace gkr
{
namespace sys
{

class rw_lock
{
    rw_lock           (const rw_lock&) noexcept = delete;
    rw_lock& operator=(const rw_lock&) noexcept = delete;

public:
    GKR_INNER_API  rw_lock() noexcept;
    GKR_INNER_API ~rw_lock() noexcept;

public:
    rw_lock(rw_lock&& other) noexcept : m_handle(other.m_handle)
    {
        other.m_handle = nullptr;
    }
    rw_lock& operator=(rw_lock&& other) noexcept
    {
        if(this != &other)
        {
            m_handle = other.m_handle;
            other.m_handle = nullptr;
        }
        return *this;
    }

public:
    GKR_INNER_API void lock() noexcept;
    GKR_INNER_API void unlock() noexcept;
    GKR_INNER_API bool try_lock() noexcept;

    GKR_INNER_API void lock_shared() noexcept;
    GKR_INNER_API void unlock_shared() noexcept;
    GKR_INNER_API bool try_lock_shared() noexcept;

public:
    using native_handle_type = void*;

private:
    native_handle_type m_handle;
};

}
}
