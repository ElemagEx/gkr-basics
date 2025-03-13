#include <gkr/defs.hpp>
#include <gkr/concurency/waitable_semaphore.hpp>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winerror.h>

namespace gkr
{

waitable_semaphore::~waitable_semaphore()
{
}

bool waitable_semaphore::handle_poll_data(unsigned long long value)
{
    return true;
}

void* waitable_semaphore::create(unsigned max_count, unsigned init_count)
{
    Check_Arg_IsValid((max_count > 0U) && (max_count <= MAXIMUM_COUNT), nullptr);

    Check_Arg_IsValid(init_count <= max_count, nullptr);

    HANDLE hSemaphore = CreateSemaphoreA(nullptr, LONG(init_count), LONG(max_count), nullptr);

    Check_Sys_Result(hSemaphore != nullptr, nullptr);

    return hSemaphore;
}

bool waitable_semaphore::release(unsigned count)
{
    Check_ValidState(is_valid(), false);

    Check_Arg_IsValid((count > 0U) && (count < MAXIMUM_COUNT), false);

    BOOL res = ReleaseSemaphore(handle(), LONG(count), nullptr);

    Check_Sys_Result(res || (GetLastError() == ERROR_TOO_MANY_POSTS), false);

    return !!res;
}

void waitable_semaphore::set_max_count(unsigned max_count)
{
    Check_Arg_IsValid((max_count > 0U) && (max_count < MAXIMUM_COUNT), );

    Assert_CheckMsg(wait_count() == 0, "There is an active thread that wait for this object");

    if(max_count != m_max_count)
    {
        *this = std::move(waitable_semaphore(max_count));
    }
}

}

#else

#include <unistd.h>
#include <sys/eventfd.h>

namespace gkr
{

waitable_semaphore::~waitable_semaphore()
{
}

bool waitable_semaphore::handle_poll_data(unsigned long long value)
{
    return true;
}

int waitable_semaphore::create(unsigned max_count, unsigned init_count)
{
    Check_Arg_IsValid((max_count > 0U) && (max_count < MAXIMUM_COUNT), nullptr);

    Check_Arg_IsValid(init_count <= max_count, nullptr);

    int fd = eventfd(init_count, EFD_NONBLOCK);

    Check_Sys_Result(fd != -1, );

    return fd;
}

bool waitable_semaphore::release(unsigned count)
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

void waitable_semaphore::set_max_count(unsigned max_count)
{
    Check_Arg_IsValid((max_count > 0U) && (max_count < MAXIMUM_COUNT), );

    m_max_count = max_count;
}

}

#endif
