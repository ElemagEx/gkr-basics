#include <gkr/defs.hpp>
#include <gkr/concurency/waitable_object.hpp>

#include <gkr/stack_alloc.hpp>

namespace gkr
{

waitable_object::~waitable_object()
{
    close();
}
waitable_object& waitable_object::null() noexcept
{
    Assert_FailureMsg("Cannot have null waitable object");
    std::terminate();
}

}

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winerror.h>

namespace gkr
{

void waitable_object::close()
{
    if(is_valid())
    {
        DIAG_VAR(BOOL, res)
        CloseHandle(handle());
        m_handle = INVALID_OBJECT_HANDLE_VALUE;
        Check_Sys_Result(res, );
    }
}
bool waitable_object::wait(long long timeout_ns)
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


wait_result_t waitable_object::wait_many(long long timeout_ns, waitable_object** objects, std::size_t count)
{
    Check_Arg_NotNull(objects, WAIT_RESULT_ERROR);
    Check_Arg_IsValid((count > 0) && (count <= WAIT_MAX_OBJECTS), WAIT_RESULT_ERROR);

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

    GKR_STACK_ARRAY(HANDLE, handles, count);

    for(std::size_t index = 0; index < count; ++index)
    {
        Check_Arg_NotNull(objects[index], WAIT_RESULT_ERROR);
        handles[index] =  objects[index]->handle();
    }
    DWORD res = WaitForMultipleObjects(DWORD(count), handles, false, timeout);

    Check_Sys_Result(res != WAIT_FAILED, WAIT_RESULT_ERROR);

    if(res == WAIT_TIMEOUT)
    {
        return WAIT_RESULT_TIMEOUT;
    }

    wait_result_t wait_result = 0;

    if((res >= WAIT_ABANDONED_0) && (res < (WAIT_ABANDONED_0 + count)))
    {
        res -= WAIT_ABANDONED_0;
    }
    if(res < (WAIT_OBJECT_0 + count))
    {
    }

    wait_result |= wait_result_t(1) << res;

    for(std::size_t index = 0; index < count; ++index)
    {
        if(index == res) continue;

        if(objects[index]->try_consume())
        {
            wait_result |= wait_result_t(1) << index;
        }
    }
    return wait_result;
}

}

#else

#include <poll.h>
#include <unistd.h>
#include <sys/eventfd.h>

namespace gkr
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

void waitable_object::close()
{
    if(is_valid())
    {
        DIAG_VAR(int, res)
        ::close(m_handle);
        m_handle = INVALID_OBJECT_HANDLE_VALUE;
        Check_Sys_Result(res != -1, );
    }
}
bool waitable_object::wait(long long timeout_ns)
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
                if(timeout_ns < 0) return false;
                continue;
            }
            Check_Sys_Result(res != -1, false);

            return handle_poll_data(value);
        }
    }
}

wait_result_t wait_objects(long long timeout_ns, waitable_object** objects, std::size_t count)
{
    return 0;
}

}

#endif
