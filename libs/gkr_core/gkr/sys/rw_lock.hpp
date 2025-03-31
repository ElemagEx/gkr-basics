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

private:
#ifdef _WIN32
    static constexpr unsigned SIZE_IN_BYTES = sizeof(void*);
#else
    static constexpr unsigned SIZE_IN_BYTES = 4 * sizeof(void*) + 24;
#endif

private:
    alignas(void*) char m_data[SIZE_IN_BYTES] = {0};
};

}
}
