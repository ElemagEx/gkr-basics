#include <gkr/defs.hpp>
#include <gkr/concurency/waitable_object.hpp>

#include <gkr/stack_alloc.hpp>

#include <vector>
#include <exception>

namespace gkr
{

#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
class wait_count_keeper
{
    std::size_t* m_wait_count;

    void inc() noexcept;
    void dec() noexcept;

    wait_count_keeper           (const wait_count_keeper&) noexcept = delete;
    wait_count_keeper& operator=(const wait_count_keeper&) noexcept = delete;

public:
    wait_count_keeper           (wait_count_keeper&& other) noexcept : m_wait_count ( std::exchange(other.m_wait_count, nullptr)) {}
    wait_count_keeper& operator=(wait_count_keeper&& other) noexcept { m_wait_count = std::exchange(other.m_wait_count, nullptr); return *this; }

    wait_count_keeper(std::size_t* wait_count) noexcept : m_wait_count(wait_count)
    {
        inc();
    }
    ~wait_count_keeper() noexcept
    {
        if(m_wait_count != nullptr) dec();
    }
};
#endif

waitable_object::~waitable_object()
{
    close();
}
waitable_object& waitable_object::null_ref() noexcept
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

#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
void wait_count_keeper::inc() noexcept
{
    InterlockedIncrementSizeT(m_wait_count);
}
void wait_count_keeper::dec() noexcept
{
    InterlockedDecrementSizeT(m_wait_count);
}
#endif

void waitable_object::close()
{
    if(is_valid())
    {
        Assert_CheckMsg(wait_count() == 0, "There is an active thread that wait for this object");

        DIAG_VAR(BOOL, res)
        CloseHandle(handle());
        m_handle = INVALID_OBJECT_HANDLE_VALUE;
        Check_Sys_Result(res, );
    }
}
bool waitable_object::wait(long long timeout_ns)
{
#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
    wait_count_keeper keeper(&m_wait_count);
#endif

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

    Check_Sys_Inspect(waitResult == WAIT_OBJECT_0, false);

    return true;
}

wait_result_t waitable_object::wait_many(long long timeout_ns, waitable_object** objects, std::size_t count)
{
    Check_Arg_NotNull(objects, WAIT_RESULT_ERROR);
    Check_Arg_IsValid((count > 0) && (count <= WAIT_MAX_OBJECTS), WAIT_RESULT_ERROR);

#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
    std::vector<wait_count_keeper> keepers;
    for(std::size_t index = 0; index < count; ++index)
    {
        if(objects[index] != nullptr) keepers.emplace_back(&objects[index]->m_wait_count);
    }
#endif

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
        Check_Arg_NotNull(objects[index]            , WAIT_RESULT_ERROR);
        Check_Arg_IsValid(objects[index]->is_valid(), WAIT_RESULT_ERROR);
        handles[index] =  objects[index]->handle();
    }

    DWORD res = WaitForMultipleObjects(DWORD(count), handles, false, timeout);

    Check_Sys_Inspect(res != WAIT_FAILED, WAIT_RESULT_ERROR);

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

namespace gkr
{

static long long now()
{
    struct timespec ts;
    DIAG_VAR(int, res)
    clock_gettime(CLOCK_MONOTONIC, &ts);
    Check_Sys_Result(res, false);
    long long ns = ts.tv_sec * 1000000000 + ts.tv_nsec;
    return ns;
}

#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
void wait_count_keeper::inc() noexcept
{
    __atomic_add_fetch(m_wait_count, 1, __ATOMIC_SEQ_CST);
}
void wait_count_keeper::dec() noexcept
{
    __atomic_sub_fetch(m_wait_count, 1, __ATOMIC_SEQ_CST);
}
#endif

void waitable_object::close()
{
    if(is_valid())
    {
        Assert_CheckMsg(wait_count() == 0, "There is an active thread that wait for this object");

        DIAG_VAR(int, res)
        ::close(m_handle);
        m_handle = INVALID_OBJECT_HANDLE_VALUE;
        Check_Sys_Result(res, );
    }
}
bool waitable_object::wait(long long timeout_ns)
{
#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
    wait_count_keeper keeper(&m_wait_count);
#endif

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
            DIAG_VAR(int, poll_result)
            poll(&pfd, 1U, -1);

            Check_Sys_Inspect(poll_result == 1, false);

            Assert_CheckMsg(0 == (pfd.revents & POLLNVAL), "This waitable object is closed/destructed during wait");

            Check_ValidState(pfd.revents == POLLIN, false);

            const ssize_t res = read(pfd.fd, &value, sizeof(value));

            if((res == -1) && (errno == EAGAIN))
            {
                pfd.revents = 0;
                continue;
            }
            Check_Sys_Result(res, false);

            return handle_poll_data(value);
        }
    }
    else if(timeout_ns <= 0)
    {
        //
        // No wait - just check
        //
        {
            const int poll_result = poll(&pfd, 1U, 0);

            Check_Sys_Result(poll_result, false);

            if(poll_result == 0) return false;

            Assert_CheckMsg(0 == (pfd.revents & POLLNVAL), "This waitable object is closed/destructed during wait");

            Check_ValidState(pfd.revents == POLLIN, false);

            const ssize_t res = read(pfd.fd, &value, sizeof(value));

            if((res == -1) && (errno == EAGAIN)) return false;

            Check_Sys_Result(res, false);

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

            const int poll_result = ppoll(&pfd, 1U, &ts, nullptr);

            Check_Sys_Result(poll_result, false);

            if(poll_result == 0) return false;

            Assert_CheckMsg(0 == (pfd.revents & POLLNVAL), "This waitable object is closed/destructed during wait");

            Check_ValidState(pfd.revents == POLLIN, false);

            const ssize_t res = read(pfd.fd, &value, sizeof(value));

            if((res == -1) && (errno == EAGAIN))
            {
                timeout_ns -= (now() - start_time);
                if(timeout_ns < 0) return false;
                pfd.revents = 0;
                continue;
            }
            Check_Sys_Result(res, false);

            return handle_poll_data(value);
        }
    }
}

wait_result_t waitable_object::wait_many(long long timeout_ns, waitable_object** objects, std::size_t count)
{
    Check_Arg_NotNull(objects, WAIT_RESULT_ERROR);
    Check_Arg_IsValid((count > 0) && (count <= WAIT_MAX_OBJECTS), WAIT_RESULT_ERROR);

#ifdef GKR_WAITABLE_OBJECT_KEEP_WAIT_COUNT
    std::vector<wait_count_keeper> keepers;
    for(std::size_t index = 0; index < count; ++index)
    {
        if(objects[index] != nullptr) keepers.emplace_back(&objects[index]->m_wait_count);
    }
#endif

    GKR_STACK_ARRAY(struct pollfd, pfds, count);

    for(std::size_t index = 0; index < count; ++index)
    {
        Check_Arg_NotNull(objects[index], WAIT_RESULT_ERROR);
        pfds[index].fd      = objects[index]->handle();
        pfds[index].events  = POLLIN;
        pfds[index].revents = 0;
    }

    wait_result_t wait_result = 0;
    unsigned long long value;

    if(timeout_ns == -1)
    {
        //
        // Infinite wait
        //
        for( ; ; )
        {
            DIAG_VAR(int, poll_result)
            poll(pfds, nfds_t(count), -1);

            Check_Sys_Result(poll_result, false);

            for(std::size_t index = 0; index < count; ++index)
            {
                const short int revents = std::exchange(pfds[index].revents, 0);

                Assert_CheckMsg(0 == (revents & POLLNVAL), "This waitable object is closed/destructed during wait");

                if(revents != POLLIN) continue;

                const ssize_t res = read(pfds[index].fd, &value, sizeof(value));

                if((res == -1) && (errno == EAGAIN)) continue;

                Check_Sys_Result(res, false);

                if(objects[index]->handle_poll_data(value))
                {
                    wait_result |= (wait_result_t(1) << index);
                }
            }
            if(wait_result == 0) continue;

            return wait_result;
        }
    }
    else if(timeout_ns <= 0)
    {
        //
        // No wait - just check
        //
        {
            const int poll_result = poll(pfds, nfds_t(count), 0);

            Check_Sys_Result(poll_result, false);

            if(poll_result == 0) return WAIT_RESULT_TIMEOUT;

            for(std::size_t index = 0; index < count; ++index)
            {
                const short int revents = pfds[index].revents;

                Assert_CheckMsg(0 == (revents & POLLNVAL), "This waitable object is closed/destructed during wait");

                if(revents != POLLIN) continue;

                const ssize_t res = read(pfds[index].fd, &value, sizeof(value));

                if((res == -1) && (errno == EAGAIN)) continue;

                Check_Sys_Result(res, false);

                if(objects[index]->handle_poll_data(value))
                {
                    wait_result |= (wait_result_t(1) << index);
                }
            }
            return wait_result;
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

            const int poll_result = ppoll(pfds, nfds_t(count), &ts, nullptr);

            Check_Sys_Result(poll_result, false);

            if(poll_result == 0) return WAIT_RESULT_TIMEOUT;

            for(std::size_t index = 0; index < count; ++index)
            {
                const short int revents = std::exchange(pfds[index].revents, 0);

                Assert_CheckMsg(0 == (revents & POLLNVAL), "This waitable object is closed/destructed during wait");

                if(revents != POLLIN) continue;

                const ssize_t res = read(pfds[index].fd, &value, sizeof(value));

                if((res == -1) && (errno == EAGAIN)) continue;

                Check_Sys_Result(res, false);

                if(objects[index]->handle_poll_data(value))
                {
                    wait_result |= (wait_result_t(1) << index);
                }
            }
            if(wait_result == 0)
            {
                timeout_ns -= (now() - start_time);
                if(timeout_ns < 0) return WAIT_RESULT_TIMEOUT;
                continue;
            }
            return wait_result;
        }
    }
}

}

#endif
