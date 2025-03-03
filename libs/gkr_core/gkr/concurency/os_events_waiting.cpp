#include <gkr/defs.hpp>
#include <gkr/concurency/os_events_waiting.hpp>

#ifdef _WIN32

#define WIN32_LEAN_AND_MEAN
#include <Windows.h>

namespace gkr
{

struct lock_guard
{
    lock_guard           (const lock_guard&) noexcept = delete;
    lock_guard& operator=(const lock_guard&) noexcept = delete;

    lock_guard           (lock_guard&&) noexcept = delete;
    lock_guard& operator=(lock_guard&&) noexcept = delete;

    CRITICAL_SECTION* m_cs;
    lock_guard(CRITICAL_SECTION* cs) noexcept : m_cs(cs) { EnterCriticalSection(m_cs); }
   ~lock_guard()                     noexcept            { LeaveCriticalSection(m_cs); }
};

template<unsigned SIZE>
inline CRITICAL_SECTION* critical_section(char (&buffer)[SIZE])
{
    static_assert(SIZE >= sizeof(CRITICAL_SECTION), "You must increase size of the buffer");
    return reinterpret_cast<CRITICAL_SECTION*>(buffer);
}

win_events_waiter::win_events_waiter(std::size_t flags) noexcept : events_waiter(flags)
{
    InitializeCriticalSection(critical_section(m_section));
}

win_events_waiter::~win_events_waiter() noexcept
{
    pop_events(0);
    DeleteCriticalSection(critical_section(m_section));
}

std::size_t win_events_waiter::events_count() const noexcept
{
    lock_guard guard(critical_section(m_section));

    return m_events.size();
}

bool win_events_waiter::pop_events(std::size_t min_count) noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(critical_section(m_section));

    Check_Arg_IsValid(min_count <= m_events.size(), false);

    Check_ValidState(m_waiting_threads == 0, false);

    for(std::size_t count = (m_events.size() - min_count); count > 0; --count)
    {
        CloseHandle(m_events.back());
        m_events.pop_back();
    }
    return true;
}

bool win_events_waiter::remove_all_events() noexcept(DIAG_NOEXCEPT)
{
    return pop_events(0);
}

bool win_events_waiter::add_event(bool manual_reset, bool initial_state, std::size_t& index) noexcept(false)
{
    lock_guard guard(critical_section(m_section));

    index = m_events.size();

    Check_ValidState(m_waiting_threads == 0, false);
    Check_ValidState(index < WAIT_MAX_OBJECTS, false);
    Check_ValidState(index < MAXIMUM_WAIT_OBJECTS, false);

    Check_ValidState(!flag_is_set(Flag_ForbidMultipleEventsBind) || (index == 0), false);

    m_events.push_back(CreateEventA(nullptr, BOOL(manual_reset), BOOL(initial_state), nullptr));
    Assert_NotNullPtr(m_events.back());

    return true;
}

bool win_events_waiter::fire_event(std::size_t index) noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(critical_section(m_section));

    Check_Arg_IsValid(index < m_events.size(), false);

    SetEvent(m_events[index]);
    return true;
}
bool win_events_waiter::reset_event(std::size_t index) noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(critical_section(m_section));

    Check_Arg_IsValid(index < m_events.size(), false);

    ResetEvent(m_events[index]);
    return true;
}

wait_result_t win_events_waiter::check_single_event(std::size_t index) noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(critical_section(m_section));

    Check_Arg_IsValid(index < m_events.size(), WAIT_RESULT_ERROR);

    return check_event(index);
}

wait_result_t win_events_waiter::wait_single_event(std::size_t index, long long nsec) noexcept(DIAG_NOEXCEPT)
{
    const unsigned timeout = (nsec < 0)
        ? INFINITE
        : unsigned((nsec + 999999) / 1000000)
        ;
    return wait_event(index, timeout);
}

wait_result_t win_events_waiter::check_all_events() noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(critical_section(m_section));

    Check_Arg_IsValid(0 < m_events.size(), WAIT_RESULT_ERROR);

    return check_events(MAXIMUM_WAIT_OBJECTS);
}

wait_result_t win_events_waiter::wait_all_events(long long nsec) noexcept(DIAG_NOEXCEPT)
{
    const unsigned timeout = (nsec < 0)
        ? INFINITE
        : unsigned((nsec + 999999) / 1000000)
        ;
    return wait_events(timeout);
}

bool win_events_waiter::this_thread_can_wait() const noexcept
{
    return !flag_is_set(Flag_ForbidMultipleThreadsWait) || (m_waiting_threads == 0);
}

wait_result_t win_events_waiter::check_events(std::size_t preset_index) const noexcept
{
    wait_result_t result = 0;

    for(std::size_t index = 0; index < m_events.size(); ++index)
    {
        if((index == preset_index) || (WAIT_OBJECT_0 == WaitForSingleObject(m_events[index], IGNORE)))
        {
            result |= (wait_result_t(1U) << index);
        }
    }
    return result;
}

wait_result_t win_events_waiter::wait_events(unsigned timeout) noexcept(DIAG_NOEXCEPT)
{
    HANDLE* handles;
    std::size_t count;
    {
        lock_guard guard(critical_section(m_section));

        Check_ValidState(this_thread_can_wait(), WAIT_RESULT_ERROR);

        Check_ValidState(m_events.size() > 0, WAIT_RESULT_ERROR);

        count   = m_events.size();
        handles = m_events.data();

        ++m_waiting_threads;
    }
    const DWORD signaled_object_index = WaitForMultipleObjects(DWORD(count), handles, FALSE, timeout);

    lock_guard guard(critical_section(m_section));

    --m_waiting_threads;

    Check_ValidState(signaled_object_index != WAIT_FAILED, WAIT_RESULT_ERROR);

    if(signaled_object_index == WAIT_TIMEOUT) return WAIT_RESULT_TIMEOUT;

    Check_ValidState(signaled_object_index < m_events.size(), WAIT_RESULT_ERROR);

    return check_events(signaled_object_index);
}

wait_result_t win_events_waiter::check_event(std::size_t index) const noexcept
{
    const wait_result_t result = (WAIT_OBJECT_0 != WaitForSingleObject(m_events[index], IGNORE))
        ? WAIT_RESULT_TIMEOUT
        : (wait_result_t(1U) << index)
        ;
    return result;
}

wait_result_t win_events_waiter::wait_event(std::size_t index, unsigned timeout) noexcept(DIAG_NOEXCEPT)
{
    HANDLE handle;
    {
        lock_guard guard(critical_section(m_section));

        Check_ValidState(this_thread_can_wait(), WAIT_RESULT_ERROR);

        Check_ValidState(m_events.size() > index, WAIT_RESULT_ERROR);

        Check_ValidState(flag_is_set(Flag_AllowPartialEventsWait) || (m_events.size() == 1), WAIT_RESULT_ERROR);

        handle = m_events[index];

        ++m_waiting_threads;
    }
    const DWORD signaled_object_index = WaitForSingleObject(handle, timeout);

    lock_guard guard(critical_section(m_section));

    --m_waiting_threads;

    Check_ValidState(signaled_object_index != WAIT_FAILED, WAIT_RESULT_ERROR);

    if(signaled_object_index == WAIT_TIMEOUT) return WAIT_RESULT_TIMEOUT;

    Check_ValidState(signaled_object_index == WAIT_OBJECT_0, WAIT_RESULT_ERROR);

    const wait_result_t result = (wait_result_t(1U) << index);
    return result;
}

}

#else

#include <time.h>
#include <poll.h>
#include <unistd.h>
#include <pthread.h>
#include <sys/eventfd.h>
#include <gkr/stack_alloc.hpp>

namespace gkr
{

struct lock_guard
{
    lock_guard           (const lock_guard&) noexcept = delete;
    lock_guard& operator=(const lock_guard&) noexcept = delete;

    lock_guard           (lock_guard&&) noexcept = delete;
    lock_guard& operator=(lock_guard&&) noexcept = delete;

    pthread_mutex_t* m_mutex;
    lock_guard(pthread_mutex_t* mutex) noexcept : m_mutex(mutex) { pthread_mutex_lock  (m_mutex); }
   ~lock_guard()                       noexcept                  { pthread_mutex_unlock(m_mutex); }
};

template<unsigned SIZE>
inline pthread_mutex_t* pthread_mutex(char (&buffer)[SIZE])
{
    static_assert(SIZE >= sizeof(pthread_mutex_t), "You must increase size of the buffer");
    return reinterpret_cast<pthread_mutex_t*>(buffer);
}

linux_events_waiter::linux_events_waiter(std::size_t flags) noexcept : events_waiter(flags)
{
    pthread_mutex_init(pthread_mutex(m_mutex), nullptr);
}

linux_events_waiter::~linux_events_waiter() noexcept
{
    pop_events(0);
    pthread_mutex_destroy(pthread_mutex(m_mutex));
}

std::size_t linux_events_waiter::events_count() const noexcept
{
    lock_guard guard(pthread_mutex(m_mutex));

    return m_events.size();
}

bool linux_events_waiter::pop_events(std::size_t min_count) noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(pthread_mutex(m_mutex));

    Check_Arg_IsValid(min_count <= m_events.size(), false);

    Check_ValidState(m_waiting_threads == 0, false);

    for(std::size_t count = (m_events.size() - min_count); count > 0; --count)
    {
        close(m_events.back());
        m_events.pop_back();
    }
    return true;
}

bool linux_events_waiter::remove_all_events() noexcept(DIAG_NOEXCEPT)
{
    return pop_events(0);
}

bool linux_events_waiter::add_event(bool manual_reset, bool initial_state, std::size_t& index) noexcept(false)
{
    lock_guard guard(pthread_mutex(m_mutex));

    index = m_events.size();

    Check_ValidState(m_waiting_threads == 0, false);
    Check_ValidState(index < WAIT_MAX_OBJECTS, false);

    Check_ValidState(!flag_is_set(Flag_ForbidMultipleEventsBind) || (index == 0), false);

    const unsigned value = initial_state ? 1U : 0;
    const int      flags = EFD_NONBLOCK;

    m_events.push_back(eventfd(value, flags));
    Assert_Check(m_events.back() != -1);

    if(manual_reset) m_auto_mask |= (std::size_t(1) << index);

    return true;
}

bool linux_events_waiter::fire_event(std::size_t index) noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(pthread_mutex(m_mutex));

    Check_Arg_IsValid(index < m_events.size(), false);

    const unsigned long long value = 1;
    const ssize_t res = write(m_events[index], &value, sizeof(value));
    return (res > 0);
}

bool linux_events_waiter::reset_event(std::size_t index) noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(pthread_mutex(m_mutex));

    Check_Arg_IsValid(index < m_events.size(), false);

    unsigned long long value = 0;
    const ssize_t res = read(m_events[index], &value, sizeof(value));
    return (res > 0);
}

wait_result_t linux_events_waiter::check_single_event(std::size_t index) noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(pthread_mutex(m_mutex));

    Check_Arg_IsValid(index < m_events.size(), WAIT_RESULT_ERROR);

    return check_event(index);
}

wait_result_t linux_events_waiter::wait_single_event(std::size_t index, long long nsec) noexcept(DIAG_NOEXCEPT)
{
    return wait_event(index, nsec);
}

wait_result_t linux_events_waiter::check_all_events() noexcept(DIAG_NOEXCEPT)
{
    lock_guard guard(pthread_mutex(m_mutex));

    Check_Arg_IsValid(0 < m_events.size(), WAIT_RESULT_ERROR);

    return check_events();
}

wait_result_t linux_events_waiter::wait_all_events(long long nsec) noexcept(DIAG_NOEXCEPT)
{
    return wait_events(nsec);
}

bool linux_events_waiter::this_thread_can_wait() const noexcept
{
    return !flag_is_set(Flag_ForbidMultipleThreadsWait) || (m_waiting_threads == 0);
}

wait_result_t linux_events_waiter::check_events() const noexcept
{
    wait_result_t result = 0;

    for(std::size_t index = 0; index < m_events.size(); ++index)
    {
        result |= check_event(index);
    }
    return result;
}

wait_result_t linux_events_waiter::wait_events(long long nsec) noexcept(DIAG_NOEXCEPT)
{
    int* eventfds;
    std::size_t count;
    {
        lock_guard guard(pthread_mutex(m_mutex));

        Check_ValidState(this_thread_can_wait(), WAIT_RESULT_ERROR);

        Check_ValidState(m_events.size() > 0, WAIT_RESULT_ERROR);

        count    = m_events.size();
        eventfds = m_events.data();

        ++m_waiting_threads;
    }
    GKR_STACK_ARRAY(pollfd, pfds, count);
    for(std::size_t index = 0; index < count; ++index)
    {
        pfds[index].fd      = eventfds[index];
        pfds[index].events  = POLLIN;
        pfds[index].revents = 0;
    }
    struct timespec ts {0, 0};
    struct timespec* pts = nullptr;
    if(nsec >= 0)
    {
        ts.tv_sec  = nsec / 1000000000;
        ts.tv_nsec = nsec % 1000000000;
        pts = &ts;
    }
    const int poll_ret = ppoll(pfds, nfds_t(count), pts, nullptr);

    lock_guard guard(pthread_mutex(m_mutex));

    --m_waiting_threads;

    Check_ValidState(poll_ret >= 0, WAIT_RESULT_ERROR);

    if(poll_ret == 0) return WAIT_RESULT_TIMEOUT;

    wait_result_t result = 0;

    for(std::size_t index = 0; index < count; ++index)
    {
        if((pfds[index].revents & POLLIN) != 0)
        {
            result |= check_event(index);
        }
    }
    return result;
}

wait_result_t linux_events_waiter::check_event(std::size_t index) const noexcept
{
    const std::size_t mask = (std::size_t(1) << index);

    if((m_auto_mask & mask) == 0)
    {
        unsigned long long value = 0;
        return (-1 == read(m_events[index], &value, sizeof(value)))
            ? WAIT_RESULT_TIMEOUT
            : mask;
    }
    else
    {
        pollfd pfds {m_events[index], POLLIN, 0};

        const int poll_ret = poll(&pfds, 1, 0);
        Check_ValidState(poll_ret >= 0, WAIT_RESULT_ERROR);
        return (poll_ret == 0)
            ? WAIT_RESULT_TIMEOUT
            : mask;
    }
}

wait_result_t linux_events_waiter::wait_event(std::size_t index, long long nsec) noexcept(DIAG_NOEXCEPT)
{
    const std::size_t mask = (std::size_t(1) << index);

    pollfd pfds {0, POLLIN, 0};
    {
        lock_guard guard(pthread_mutex(m_mutex));

        Check_ValidState(this_thread_can_wait(), WAIT_RESULT_ERROR);

        Check_ValidState(m_events.size() > index, WAIT_RESULT_ERROR);

        Check_ValidState(flag_is_set(Flag_AllowPartialEventsWait) || (m_events.size() == 1), WAIT_RESULT_ERROR);

        pfds.fd = m_events[index];

        ++m_waiting_threads;
    }
    struct timespec ts {0, 0};
    struct timespec* pts = nullptr;
    if(nsec >= 0)
    {
        ts.tv_sec  = nsec / 1000000000;
        ts.tv_nsec = nsec % 1000000000;
        pts = &ts;
    }
    const int poll_ret = ppoll(&pfds, 1, pts, nullptr);

    lock_guard guard(pthread_mutex(m_mutex));

    --m_waiting_threads;

    Check_ValidState(poll_ret >= 0, WAIT_RESULT_ERROR);

    if(poll_ret == 0) return WAIT_RESULT_TIMEOUT;

    if((m_auto_mask & mask) != 0) return mask;

    unsigned long long value = 0;
    return (-1 == read(pfds.fd, &value, sizeof(value)))
        ? WAIT_RESULT_TIMEOUT
        : mask;
}

}

#endif
