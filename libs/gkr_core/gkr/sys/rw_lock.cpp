#include <gkr/defs.hpp>
#include <gkr/sys/rw_lock.hpp>

#include <gkr/diagnostics.hpp>

template<unsigned> struct half_int_t;

template<> struct half_int_t<8> { using type = unsigned int  ; };
template<> struct half_int_t<4> { using type = unsigned short; };

using half_int = half_int_t<sizeof(void*)>::type;

union tls_t
{
    void* value;
    struct { half_int writer, reader; } ref_count;
};
static_assert(sizeof(tls_t) == sizeof(void*), "The tls_t must be with the same size as sizeof(void*)");

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace
{
static_assert(sizeof(RTL_SRWLOCK) == sizeof(gkr::sys::rw_lock::data_t), "The sizeof(rw_lock::data_t) must be equal to sizeof(RTL_SRWLOCK)");
static_assert(sizeof(DWORD      ) == sizeof(gkr::sys::rw_lock:: key_t), "The sizeof(rw_lock:: key_t) must be equal to sizeof(DWORD)");

inline RTL_SRWLOCK* get_srwlock(char* data)
{
    return reinterpret_cast<RTL_SRWLOCK*>(data);
}

inline void init_reader_writer_lock(char* data) noexcept
{
    InitializeSRWLock(get_srwlock(data));
}
inline void done_reader_writer_lock(char* data) noexcept
{
}
inline void writer_lock(char* data) noexcept
{
    AcquireSRWLockExclusive(get_srwlock(data));
}
inline void writer_unlock(char* data) noexcept
{
    ReleaseSRWLockExclusive(get_srwlock(data));
}
inline bool try_writer_lock(char* data) noexcept
{
    return TryAcquireSRWLockExclusive(get_srwlock(data));
}
inline void reader_lock(char* data) noexcept
{
    AcquireSRWLockShared(get_srwlock(data));
}
inline void reader_unlock(char* data) noexcept
{
    ReleaseSRWLockShared(get_srwlock(data));
}
inline bool try_reader_lock(char* data) noexcept
{
    return TryAcquireSRWLockShared(get_srwlock(data));
}
inline DWORD init_thread_value() noexcept
{
    return TlsAlloc();
}
inline void done_thread_value(DWORD key) noexcept
{
    TlsFree(key);
}
inline void* get_thread_value(DWORD key) noexcept
{
    return TlsGetValue(key);
}
inline void set_thread_value(DWORD key, void* value) noexcept
{
    TlsSetValue(key, value);
}
}

#else

#include <pthread.h>

namespace
{
static_assert(sizeof(pthread_rwlock_t) == sizeof(gkr::sys::rw_lock::data_t), "The sizeof(rw_lock::data_t) must be equal to sizeof(pthread_rwlock_t)");
static_assert(sizeof(pthread_key_t   ) == sizeof(gkr::sys::rw_lock:: key_t), "The sizeof(rw_lock:: key_t) must be equal to sizeof(pthread_key_t)");

inline pthread_rwlock_t* rw_lock_ptr(char* data)
{
    return reinterpret_cast<pthread_rwlock_t*>(data);
}

inline void init_reader_writer_lock(char* data) noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_init(rw_lock_ptr(data), nullptr);
    Check_Sys_Error(rc, );
}
inline void done_reader_writer_lock(char* data) noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_destroy(rw_lock_ptr(data));
    Check_Sys_Error(rc, );
}
inline void writer_lock(char* data) noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_wrlock(rw_lock_ptr(data));
    Check_Sys_Error(rc, );
}
inline void writer_unlock(char* data) noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_unlock(rw_lock_ptr(data));
    Check_Sys_Error(rc, );
}
inline bool try_writer_lock(char* data) noexcept
{
    int rc = pthread_rwlock_trywrlock(rw_lock_ptr(data));
    if(rc == EBUSY) return false;
    Check_Sys_Error(rc, false);
    return true;
}
inline void reader_lock(char* data) noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_rdlock(rw_lock_ptr(data));
    Check_Sys_Error(rc, );
}
inline void reader_unlock(char* data) noexcept
{
    DIAG_VAR(int, rc)
    pthread_rwlock_unlock(rw_lock_ptr(data));
    Check_Sys_Error(rc, );
}
inline bool try_reader_lock(char* data) noexcept
{
    int rc = pthread_rwlock_tryrdlock(rw_lock_ptr(data));
    if(rc == EBUSY) return false;
    Check_Sys_Error(rc, false);
    return true;
}
inline pthread_key_t init_thread_value() noexcept
{
    pthread_key_t key;
    DIAG_VAR(int, rc)
    pthread_key_create(&key, nullptr);
    Check_Sys_Error(rc, );
    return key;
}
inline void done_thread_value(pthread_key_t key) noexcept
{
    DIAG_VAR(int, rc)
    pthread_key_delete(key);
    Check_Sys_Error(rc, );
}
inline void* get_thread_value(pthread_key_t key) noexcept
{
    return pthread_getspecific(key);
}
inline void set_thread_value(pthread_key_t key, const void* value) noexcept
{
    DIAG_VAR(int, rc)
    pthread_setspecific(key, value);
    Check_Sys_Error(rc, );
}
}

#endif

namespace gkr
{
namespace sys
{ 
rw_lock::rw_lock() noexcept
{
    init_reader_writer_lock(m_data);
}

rw_lock::~rw_lock() noexcept
{
    done_reader_writer_lock(m_data);
}

void rw_lock::lock() noexcept
{
    writer_lock(m_data);
}

void rw_lock::unlock() noexcept
{
    writer_unlock(m_data);
}

bool rw_lock::try_lock() noexcept
{
    return try_writer_lock(m_data);
}

void rw_lock::lock_shared() noexcept
{
    reader_lock(m_data);
}

void rw_lock::unlock_shared() noexcept
{
    reader_unlock(m_data);
}

bool rw_lock::try_lock_shared() noexcept
{
    return try_reader_lock(m_data);
}


recursive_rw_lock::recursive_rw_lock() noexcept : m_key(init_thread_value())
{
    tls_t tls {nullptr};
    set_thread_value(m_key, tls.value);
    init_reader_writer_lock(m_data);
}

recursive_rw_lock::~recursive_rw_lock() noexcept
{
    done_reader_writer_lock(m_data);
    done_thread_value(m_key);
}

void recursive_rw_lock::lock() noexcept
{
    tls_t tls {get_thread_value(m_key)};

    Assert_Check(tls.ref_count.reader == 0);

    if(tls.ref_count.writer == 0)
    {
        writer_lock(m_data);
    }

    ++tls.ref_count.writer;
    set_thread_value(m_key, tls.value);
}

void recursive_rw_lock::unlock() noexcept
{
    tls_t tls {get_thread_value(m_key)};

    Assert_Check(tls.ref_count.reader == 0);
    Assert_Check(tls.ref_count.writer >= 1);

    if(tls.ref_count.writer == 1)
    {
        writer_unlock(m_data);
    }

    --tls.ref_count.writer;
    set_thread_value(m_key, tls.value);
}

bool recursive_rw_lock::try_lock() noexcept
{
    tls_t tls {get_thread_value(m_key)};

    Assert_Check(tls.ref_count.reader == 0);

    if((tls.ref_count.writer == 0) && !try_writer_lock(m_data)) return false;

    ++tls.ref_count.writer;
    set_thread_value(m_key, tls.value);
    return true;
}

void recursive_rw_lock::lock_shared() noexcept
{
    tls_t tls {get_thread_value(m_key)};

    if((tls.ref_count.reader == 0) && (tls.ref_count.writer == 0))
    {
        reader_lock(m_data);
    }

    ++tls.ref_count.reader;
    set_thread_value(m_key, tls.value);
}

void recursive_rw_lock::unlock_shared() noexcept
{
    tls_t tls {get_thread_value(m_key)};

    Assert_Check(tls.ref_count.reader >= 1);

    if((tls.ref_count.reader == 1) && (tls.ref_count.writer == 0))
    {
        reader_unlock(m_data);
    }

    --tls.ref_count.reader;
    set_thread_value(m_key, tls.value);
}

bool recursive_rw_lock::try_lock_shared() noexcept
{
    tls_t tls {get_thread_value(m_key)};

    if((tls.ref_count.reader == 0) && (tls.ref_count.writer == 0) && !try_reader_lock(m_data)) return false;

    ++tls.ref_count.reader;
    set_thread_value(m_key, tls.value);
    return true;
}

}
}
