#pragma once

#include <gkr/container/lockfree_queue.hpp>

#ifdef _WIN32
#ifndef _WINBASE_
#ifndef _APISETHANDLE_
extern "C" __declspec(dllimport) int __stdcall CloseHandle(void*);
#endif
#ifndef _SYNCHAPI_H_
extern "C" __declspec(dllimport) int __stdcall SetEvent(void*);
extern "C" __declspec(dllimport) int __stdcall ResetEvent(void*);
extern "C" __declspec(dllimport) void* __stdcall CreateEventA(void*, int, int, const char*);
extern "C" __declspec(dllimport) unsigned __stdcall WaitForSingleObject(void*, unsigned);
#endif
#endif
#else
#include <poll.h>
#include <unistd.h>
#include <sys/eventfd.h>
#endif

namespace gkr
{
namespace impl
{

class os_wait_support
{
    os_wait_support           (const os_wait_support&) noexcept = delete;
    os_wait_support& operator=(const os_wait_support&) noexcept = delete;

private:
#ifdef _WIN32
    using handle_t = void*;

    static void fire(handle_t event)
    {
        ::SetEvent(SetEvent);
    }
    static void reset(handle_t event)
    {
        ::ResetEvent(event);
    }
    static bool wait(handle_t event, long long timeout_ns)
    {
        const unsigned timeout = (timeout_ns == -1)
            ? unsigned(-1)
            : ((timeout_ns <= 0) ? 0 : unsigned(timeout_ns / 1000000))
            ;
        return (0 == ::WaitForSingleObject(event, timeout));
    }
#else
    using handle_t = int;

    static void fire(handle_t event)
    {
        unsigned long long value = 1;
        ::write(event, &value, sizeof(value));
    }
    static void reset(handle_t event)
    {
        unsigned long long value;
        ::read(event, &value, sizeof(value));
    }
    static bool wait(handle_t event, long long timeout_ns)
    {
        const unsigned timeout = (timeout_ns == -1)
            ? unsigned(-1)
            : ((timeout_ns <= 0) ? 0 : unsigned(timeout_ns / 1000000))
            ;
        struct pollfd pfd {event, POLLIN, 0};

        if(0 == ::poll(&pfd, 1U, timeout)) return false;

        unsigned long long value;
        const ssize_t res = ::read(pfd.fd, &value, sizeof(value));

        if(res == -1) return false;

        value = 1;
        ::write(pfd.fd, &value, sizeof(value));

        return true;
    }
#endif

private:
    std::atomic<std::size_t> m_busy_count {0};
    std::atomic<std::size_t> m_free_count {0};

    handle_t m_has_space_event;
    handle_t m_has_items_event;

protected:
#ifdef _WIN32
    os_wait_support() noexcept
    {
        m_has_space_event = ::CreateEventA(nullptr, 1, 0, nullptr);
        m_has_items_event = ::CreateEventA(nullptr, 1, 0, nullptr);
    }
    ~os_wait_support() noexcept
    {
        ::CloseHandle(m_has_items_event);
        ::CloseHandle(m_has_space_event);
    }
#else
    linux_wait_support() noexcept
    {
        m_has_space_event = ::eventfd(1, EFD_NONBLOCK);
        m_has_items_event = ::eventfd(1, EFD_NONBLOCK);
    }
    ~linux_wait_support() noexcept
    {
        ::close(m_has_items_event);
        ::close(m_has_space_event);
    }
#endif

    os_wait_support(os_wait_support&& other) noexcept
        : m_busy_count(other.m_busy_count.exchange(0))
        , m_free_count(other.m_free_count.exchange(0))
        , m_has_space_event(std::move(other.m_has_space_event))
        , m_has_items_event(std::move(other.m_has_items_event))
    {
    }
    os_wait_support& operator=(os_wait_support&& other) noexcept
    {
        m_busy_count = other.m_busy_count.exchange(0);
        m_free_count = other.m_free_count.exchange(0);

        m_has_space_event = std::move(other.m_has_space_event);
        m_has_items_event = std::move(other.m_has_items_event);
        return *this;
    }

    void swap(os_wait_support& other) noexcept
    {
        m_busy_count = other.m_busy_count.exchange(m_busy_count);
        m_free_count = other.m_free_count.exchange(m_free_count);

        m_has_space_event.swap(other.m_has_space_event);
        m_has_items_event.swap(other.m_has_items_event);
    }

protected:
    void reset(std::size_t capacity) noexcept(DIAG_NOEXCEPT)
    {
        m_busy_count = 0;
        m_free_count = capacity;

        if(m_free_count > 0)
        {
            fire(m_has_space_event);
        }
        else
        {
            reset(m_has_space_event);
        }
        reset(m_has_items_event);
    }
    void reserve(std::size_t capacity) noexcept(DIAG_NOEXCEPT)
    {
        m_free_count = capacity - m_busy_count;

        if(m_free_count > 0)
        {
            fire(m_has_space_event);
        }
        else
        {
            reset(m_has_space_event);
        }
    }

protected:
    void notify_producer_ownership_fail() noexcept(DIAG_NOEXCEPT)
    {
        reset(m_has_space_event);
        fire(m_has_items_event);
    }
    void notify_producer_ownership_start() noexcept(DIAG_NOEXCEPT)
    {
        if(m_free_count-- == 1)
        {
            reset(m_has_space_event);
        }
    }
    void notify_producer_ownership_finish() noexcept(DIAG_NOEXCEPT)
    {
        if(m_busy_count++ == 0)
        {
            fire(m_has_items_event);
        }
    }
    void notify_producer_ownership_cancel() noexcept(DIAG_NOEXCEPT)
    {
        if(m_free_count++ == 0)
        {
            fire(m_has_space_event);
        }
    }

protected:
    void notify_consumer_ownership_fail() noexcept(DIAG_NOEXCEPT)
    {
        fire(m_has_space_event);
        reset(m_has_items_event);
    }
    void notify_consumer_ownership_start() noexcept(DIAG_NOEXCEPT)
    {
        if(m_busy_count-- == 1)
        {
            reset(m_has_items_event);
        }
    }
    void notify_consumer_ownership_finish() noexcept(DIAG_NOEXCEPT)
    {
        if(m_free_count++ == 0)
        {
            fire(m_has_space_event);
        }
    }
    void notify_consumer_ownership_cancel() noexcept(DIAG_NOEXCEPT)
    {
        if(m_busy_count++ == 0)
        {
            fire(m_has_items_event);
        }
    }

protected:
    bool producer_wait(long long timeout_ns) noexcept(DIAG_NOEXCEPT)
    {
        return wait(m_has_space_event, timeout_ns);
    }
    bool consumer_wait(long long timeout_ns) noexcept(DIAG_NOEXCEPT)
    {
        return wait(m_has_items_event, timeout_ns);
    }

public:
    handle_t get_producer_waitable_object() noexcept
    {
        return m_has_space_event;
    }
    handle_t get_consumer_waitable_object() noexcept
    {
        return m_has_items_event;
    }
};

}

template<
    typename T,
    bool MultipleConsumersMultipleProducersSupport=false,
    bool Pausable=false,
    typename Allocator=std::allocator<impl::queue_allocator_value_type<T>>
    >
using os_waitable_lockfree_queue = lockfree_queue<T, MultipleConsumersMultipleProducersSupport, Pausable, Allocator, impl::os_wait_support>;
}
