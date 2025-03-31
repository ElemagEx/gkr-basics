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

inline RTL_SRWLOCK* rw_lock_ptr(char* data)
{
    return reinterpret_cast<RTL_SRWLOCK*>(data);
}

rw_lock::rw_lock() noexcept
{
    static_assert(sizeof(RTL_SRWLOCK) == sizeof(m_data), "The m_data must be with the same size as sizeof(RTL_SRWLOCK)");

    InitializeSRWLock(rw_lock_ptr(m_data));
}

rw_lock::~rw_lock() noexcept
{
}

void rw_lock::lock() noexcept
{
    AcquireSRWLockExclusive(rw_lock_ptr(m_data));
}

void rw_lock::unlock() noexcept
{
    ReleaseSRWLockExclusive(rw_lock_ptr(m_data));
}

bool rw_lock::try_lock() noexcept
{
    return TryAcquireSRWLockExclusive(rw_lock_ptr(m_data));
}

void rw_lock::lock_shared() noexcept
{
    AcquireSRWLockShared(rw_lock_ptr(m_data));
}

void rw_lock::unlock_shared() noexcept
{
    ReleaseSRWLockShared(rw_lock_ptr(m_data));
}

bool rw_lock::try_lock_shared() noexcept
{
    return TryAcquireSRWLockShared(rw_lock_ptr(m_data));
}

}
}

#else

#include <pthread.h>

namespace gkr
{
namespace sys
{

inline pthread_rwlock_t* rw_lock_ptr(char* data)
{
    return reinterpret_cast<pthread_rwlock_t*>(data);
}

rw_lock::rw_lock() noexcept
{
    static_assert(sizeof(pthread_rwlock_t) == sizeof(m_data), "The m_data must be with the same size as sizeof(pthread_rwlock_t)");

    DIAG_VAR(int, rc)
    pthread_rwlock_init(rw_lock_ptr(m_data), nullptr);
    Check_Sys_Error(rc, );
}

rw_lock::~rw_lock() noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_destroy(rw_lock_ptr(m_data));
    Check_Sys_Error(rc, );
}

void rw_lock::lock() noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_wrlock(rw_lock_ptr(m_data));
    Check_Sys_Error(rc, );
}

void rw_lock::unlock() noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_unlock(rw_lock_ptr(m_data));
    Check_Sys_Error(rc, );
}

bool rw_lock::try_lock() noexcept
{
    int rc = pthread_rwlock_trywrlock(rw_lock_ptr(m_data));
    if(rc == EBUSY) return false;
    Check_Sys_Error(rc, false);
    return true;
}

void rw_lock::lock_shared() noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_rdlock(rw_lock_ptr(m_data));
    Check_Sys_Error(rc, );
}

void rw_lock::unlock_shared() noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_unlock(rw_lock_ptr(m_data));
    Check_Sys_Error(rc, );
}

bool rw_lock::try_lock_shared() noexcept
{
    int rc = pthread_rwlock_tryrdlock(rw_lock_ptr(m_data));
    if(rc == EBUSY) return false;
    Check_Sys_Error(rc, false);
    return true;
}

}
}

#endif
