#include <gkr/defs.hpp>
#include <gkr/sys/rw_lock.hpp>

#include <gkr/diagnostics.hpp>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace gkr
{
namespace sys
{

inline RTL_SRWLOCK* srw_lock_ptr(rw_lock::native_handle_type& handle)
{
    return reinterpret_cast<RTL_SRWLOCK*>(&handle);
}

rw_lock::rw_lock() noexcept : m_handle(nullptr)
{
    static_assert(sizeof(RTL_SRWLOCK) == sizeof(m_handle), "Check it");

    InitializeSRWLock(srw_lock_ptr(m_handle));
}

rw_lock::~rw_lock() noexcept
{
}

void rw_lock::lock() noexcept
{
    AcquireSRWLockExclusive(srw_lock_ptr(m_handle));
}

void rw_lock::unlock() noexcept
{
    ReleaseSRWLockExclusive(srw_lock_ptr(m_handle));
}

bool rw_lock::try_lock() noexcept
{
    return TryAcquireSRWLockExclusive(srw_lock_ptr(m_handle));
}

void rw_lock::lock_shared() noexcept
{
    AcquireSRWLockShared(srw_lock_ptr(m_handle));
}

void rw_lock::unlock_shared() noexcept
{
    ReleaseSRWLockShared(srw_lock_ptr(m_handle));
}

bool rw_lock::try_lock_shared() noexcept
{
    return TryAcquireSRWLockShared(srw_lock_ptr(m_handle));
}

}
}

#else

#include <pthread.h>

namespace gkr
{
namespace sys
{

}
}

#endif
