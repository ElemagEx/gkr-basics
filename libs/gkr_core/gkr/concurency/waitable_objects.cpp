#include <gkr/defs.hpp>
#include <gkr/concurency/waitable_objects.h>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winerror.h>

namespace gkr
{
namespace impl
{

waitable_object::~waitable_object() noexcept(DIAG_NOEXCEPT)
{
    close();
}

void waitable_object::close() noexcept(DIAG_NOEXCEPT)
{
    if(is_valid())
    {
        DIAG_VAR(BOOL, res)
        CloseHandle(handle());
        m_handle = INVALID_OBJECT_HANDLE_VALUE;
        Check_Sys_Result(res, );
    }
}
bool waitable_object::wait(long long timeout_ns) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(is_valid(), false);

    DWORD timeout;

    switch(timeout_ns)
    {
        case  0: timeout = IGNORE  ; break;
        case -1: timeout = INFINITE; break;
        default:
            timeout = (timeout_ns < 0)
                ? IGNORE
                : DWORD(timeout_ns / 1000000);
            break;
    }
    DWORD waitResult = WaitForSingleObject(m_handle, timeout);

    if(waitResult == WAIT_TIMEOUT) return false;

    Check_Sys_Result(waitResult == WAIT_OBJECT_0, false);

    return true;
}

}

waitable_event::~waitable_event() noexcept(DIAG_NOEXCEPT)
{
}

bool waitable_event::handle_poll_data(unsigned long long value) noexcept(DIAG_NOEXCEPT)
{
    return true;
}

void* waitable_event::create(bool manual_reset, bool create_fired) noexcept(DIAG_NOEXCEPT)
{
    const BOOL initial_state = create_fired ? TRUE : FALSE;

    HANDLE hEvent = CreateEventA(nullptr, manual_reset?TRUE:FALSE, initial_state, nullptr);

    Check_Sys_Result(hEvent != nullptr, );

    return hEvent;
}

void waitable_event::fire() noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(is_valid(), );

    DIAG_VAR(BOOL, res)
    SetEvent(handle());
    Check_Sys_Result(res, );
}

void waitable_event::reset() noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(is_valid(), );

    DIAG_VAR(BOOL, res)
    ResetEvent(handle());
    Check_Sys_Result(res, );
}

void waitable_event::set_manual_reset(bool manual_reset) noexcept(DIAG_NOEXCEPT)
{
    if(manual_reset != m_manual_reset)
    {
        *this = std::move(waitable_event(manual_reset));
    }
}

waitable_semaphore::~waitable_semaphore() noexcept(DIAG_NOEXCEPT)
{
}

bool waitable_semaphore::handle_poll_data(unsigned long long value) noexcept(DIAG_NOEXCEPT)
{
    return true;
}

void* waitable_semaphore::create(unsigned max_count, unsigned init_count) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid((max_count > 0U) && (max_count < 0x7FFFFFFFU), nullptr);

    Check_Arg_IsValid(init_count <= max_count, nullptr);

    HANDLE hSemaphore = CreateSemaphoreA(nullptr, LONG(init_count), LONG(max_count), nullptr);

    Check_Sys_Result(hSemaphore != nullptr, );

    return hSemaphore;
}

bool waitable_semaphore::release(unsigned count) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(is_valid(), false);

    Check_Arg_IsValid((count > 0U) && (count < 0x7FFFFFFFU), );

    BOOL res = ReleaseSemaphore(handle(), LONG(count), nullptr);

    Check_Sys_Result(res || (GetLastError() == ERROR_TOO_MANY_POSTS), false);

    return !!res;
}

void waitable_semaphore::set_max_count(unsigned max_count) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid((max_count > 0U) && (max_count < 0x7FFFFFFFU), );

    if(max_count != m_max_count)
    {
        *this = std::move(waitable_semaphore(max_count));
    }
}

}

#else

#include <poll.h>
#include <unistd.h>
#include <sys/eventfd.h>

namespace gkr
{
namespace impl
{

static long long now()
{
    struct timespec ts;
    DIAG_VAR(int, res)
    clock_gettime(CLOCK_MONOTONIC, &ts);
    Check_Sys_Result(res != -1, false);
    long long ns = ts.tv_sec * 1000000000 + ts.tv_nsec;
    return ns;
}

waitable_object::~waitable_object() noexcept(DIAG_NOEXCEPT)
{
    close();
}

void waitable_object::close() noexcept(DIAG_NOEXCEPT)
{
    if(is_valid())
    {
        DIAG_VAR(int, res)
        ::close(m_handle);
        m_handle = INVALID_OBJECT_HANDLE_VALUE;
        Check_Sys_Result(res != -1, );
    }
}
bool waitable_object::wait(long long timeout_ns) noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(is_valid(), false);

    struct pollfd pfd {handle(), POLLIN, 0};

    unsigned long long value;

    if(timeout_ns == -1)
    {
        //
        // Infinite wait
        //
        for( ; ; )
        {
            int poll_result = poll(&pfd, 1U, -1);

            Check_Sys_Result(poll_result != -1, false);

            const ssize_t res = read(handle(), &value, sizeof(value));

            if((res == -1) && (errno == EAGAIN)) continue;

            Check_Sys_Result(res != -1, false);

            return handle_poll_data(value);
        }
    }
    else if(timeout_ns <= 0)
    {
        //
        // No wait - just check
        //
        {
            int poll_result = poll(&pfd, 1U, 0);

            Check_Sys_Result(poll_result != -1, false);

            if(poll_result == 0) return false;

            const ssize_t res = read(handle(), &value, sizeof(value));

            if((res == -1) && (errno == EAGAIN)) return false;

            Check_Sys_Result(res != -1, false);

            return handle_poll_data(value);
        }
    }
    else
    {
        //
        // Timeout wait
        //
        for(long long start_time = now(); ; )
        {
            struct timespec ts {timeout_ns / 1000000000, timeout_ns % 1000000000};

            int poll_result = ppoll(&pfd, 1U, &ts, nullptr);

            Check_Sys_Result(poll_result != -1, false);

            if(poll_result == 0) return false;

            const ssize_t res = read(handle(), &value, sizeof(value));

            if((res == -1) && (errno == EAGAIN))
            {
                timeout_ns -= (now() - start_time);
                if(timeout_ns <= 0) return false;
                continue;
            }
            Check_Sys_Result(res != -1, false);

            return handle_poll_data(value);
        }
    }
}

}

waitable_event::~waitable_event() noexcept(DIAG_NOEXCEPT)
{
}

bool waitable_event::handle_poll_data(unsigned long long value) noexcept(DIAG_NOEXCEPT)
{
    if(m_manual_reset)
    {
        DIAG_VAR(ssize_t, res)
        write(handle(), &value, sizeof(value));
        Check_Sys_Result((res != -1) || (errno == EAGAIN), );
    }
    return true;
}

int waitable_event::create(bool manual_reset, bool create_fired) noexcept(DIAG_NOEXCEPT)
{
    const unsigned initial_value = create_fired ? 1 : 0;

    int fd = eventfd(initial_value, EFD_NONBLOCK);

    Check_Sys_Result(fd != -1, );

    return fd;
}

void waitable_event::fire() noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(is_valid(), );

    eventfd_t value = 1;

    DIAG_VAR(ssize_t, res)
    write(handle(), &value, sizeof(value));

    Check_Sys_Result((res != -1) || (errno == EAGAIN), );
}

void waitable_event::reset() noexcept(DIAG_NOEXCEPT)
{
    Check_ValidState(is_valid(), );

    eventfd_t value;

    DIAG_VAR(ssize_t, res)
    read(handle(), &value, sizeof(value));

    Check_Sys_Result((res != -1) || (errno == EAGAIN), );
}

void waitable_event::set_manual_reset(bool manual_reset) noexcept(DIAG_NOEXCEPT)
{
    m_manual_reset = manual_reset;
}

waitable_semaphore::~waitable_semaphore() noexcept(DIAG_NOEXCEPT)
{
}

bool waitable_semaphore::handle_poll_data(unsigned long long value) noexcept(DIAG_NOEXCEPT)
{
    return true;
}

int waitable_semaphore::create(unsigned max_count, unsigned init_count) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid((max_count > 0U) && (max_count < 0x7FFFFFFFU), nullptr);

    Check_Arg_IsValid(init_count <= max_count, nullptr);

    int fd = eventfd(init_count, EFD_NONBLOCK);

    Check_Sys_Result(fd != -1, );

    return fd;
}

bool waitable_semaphore::release(unsigned count) noexcept(DIAG_NOEXCEPT)
{
    const eventfd_t max_count = m_max_count;

    Check_ValidState(is_valid(), false);

    Check_Arg_IsValid((count > 0U) && (count < max_count), );

    eventfd_t value;
    const ssize_t len = read(handle(), &value, sizeof(value));

    Check_Sys_Result((len != -1) || (errno == EAGAIN), false);

    bool released;

    if(len == -1)
    {
        released = true;
        value = count;
    }
    else
    {
        if(value > max_count) value = max_count;
        released = (value < max_count);
        value = (max_count <= (value + count))
            ? max_count
            : (value + count);
    }

    DIAG_VAR(ssize_t, res)
    write(handle(), &value, sizeof(value));
    Check_Sys_Result((res != -1) || (errno == EAGAIN), false);
 
    return released;
}

void waitable_semaphore::set_max_count(unsigned max_count) noexcept(DIAG_NOEXCEPT)
{
    Check_Arg_IsValid((max_count > 0U) && (max_count < 0x7FFFFFFFU), );

    m_max_count = max_count;
}

}

#endif
