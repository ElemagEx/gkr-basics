#pragma once

#include <gkr/api.hpp>

namespace gkr
{
namespace sys
{

class rw_lock
{
    rw_lock(      rw_lock&&) noexcept = delete;
    rw_lock(const rw_lock& ) noexcept = delete;

    rw_lock& operator=(      rw_lock&&) noexcept = delete;
    rw_lock& operator=(const rw_lock& ) noexcept = delete;

public:
    GKR_INNER_API  rw_lock() noexcept;
    GKR_INNER_API ~rw_lock() noexcept;

public:
    GKR_INNER_API void lock() noexcept;
    GKR_INNER_API void unlock() noexcept;
    GKR_INNER_API bool try_lock() noexcept;

    GKR_INNER_API void lock_shared() noexcept;
    GKR_INNER_API void unlock_shared() noexcept;
    GKR_INNER_API bool try_lock_shared() noexcept;

public:
#ifdef _WIN32
    static constexpr unsigned SIZE_IN_BYTES = sizeof(void*);
#else
    static constexpr unsigned SIZE_IN_BYTES = 4 * sizeof(void*) + 24;
#endif
    using data_t = char[SIZE_IN_BYTES];
    using  key_t = unsigned;

private:
    alignas(void*)
    data_t m_data = {0};
};

class recursive_rw_lock
{
    recursive_rw_lock(      recursive_rw_lock&&) noexcept = delete;
    recursive_rw_lock(const recursive_rw_lock& ) noexcept = delete;

    recursive_rw_lock& operator=(      recursive_rw_lock&&) noexcept = delete;
    recursive_rw_lock& operator=(const recursive_rw_lock& ) noexcept = delete;

public:
    GKR_INNER_API  recursive_rw_lock() noexcept;
    GKR_INNER_API ~recursive_rw_lock() noexcept;

public:
    GKR_INNER_API void lock() noexcept;
    GKR_INNER_API void unlock() noexcept;
    GKR_INNER_API bool try_lock() noexcept;

    GKR_INNER_API void lock_shared() noexcept;
    GKR_INNER_API void unlock_shared() noexcept;
    GKR_INNER_API bool try_lock_shared() noexcept;

private:
    alignas(void*)
    rw_lock::data_t m_data = {0};
    rw_lock::key_t  m_key  = 0;
};

}
}
