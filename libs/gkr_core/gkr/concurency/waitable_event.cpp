#include <gkr/defs.hpp>
#include <gkr/concurency/waitable_event.hpp>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>
#include <winerror.h>

namespace gkr
{

waitable_event::~waitable_event()
{
}

bool waitable_event::handle_poll_data(unsigned long long value)
{
    return true;
}

void* waitable_event::create(bool manual_reset, bool create_fired)
{
    HANDLE hEvent = CreateEventA(nullptr, gkr_b2i(manual_reset), gkr_b2i(create_fired), nullptr);

    Check_Sys_Inspect(hEvent != nullptr, nullptr);

    return hEvent;
}

void waitable_event::fire()
{
    Check_ValidState(is_valid(), );

    DIAG_VAR(BOOL, res)
    SetEvent(handle());
    Check_Sys_Result(res, );
}

void waitable_event::reset()
{
    Check_ValidState(is_valid(), );

    DIAG_VAR(BOOL, res)
    ResetEvent(handle());
    Check_Sys_Result(res, );
}

void waitable_event::set_manual_reset(bool manual_reset)
{
    Assert_CheckMsg(wait_count() == 0, "There is an active thread that wait for this object");

    if(manual_reset != m_manual_reset)
    {
        waitable_event event(manual_reset);
        *this = std::move(event);
    }
}

}

#else

#include <unistd.h>
#include <sys/eventfd.h>

namespace gkr
{

waitable_event::~waitable_event()
{
}

bool waitable_event::handle_poll_data(unsigned long long value)
{
    if(m_manual_reset)
    {
        value = 1;
        DIAG_VAR(ssize_t, res)
        write(handle(), &value, sizeof(value));
        Check_Sys_Inspect((res != -1) || (errno == EAGAIN), false);
    }
    return true;
}

int waitable_event::create(bool manual_reset, bool create_fired)
{
    const unsigned initial_value = create_fired ? 1 : 0;

    int fd = eventfd(initial_value, EFD_NONBLOCK);

    Check_Sys_Result(fd, -1);

    return fd;
}

void waitable_event::fire()
{
    Check_ValidState(is_valid(), );

    eventfd_t value = 1;

    DIAG_VAR(ssize_t, res)
    write(handle(), &value, sizeof(value));

    Check_Sys_Inspect((res != -1) || (errno == EAGAIN), );
}

void waitable_event::reset()
{
    Check_ValidState(is_valid(), );

    eventfd_t value;

    DIAG_VAR(ssize_t, res)
    read(handle(), &value, sizeof(value));

    Check_Sys_Inspect((res != -1) || (errno == EAGAIN), );
}

void waitable_event::set_manual_reset(bool manual_reset)
{
    m_manual_reset = manual_reset;
}

}

#endif
