#include "main.h"

#include <gkr/log/consumer.h>
#include <gkr/log/message.h>
#include <gkr/log/stamp.h>
#include <cstdio>
class console_consumer : public gkr::log::consumer
{
    virtual bool init_logging() override
    {
        return true;
    }
    virtual void done_logging() override
    {
    }
    virtual void consume_log_message(const gkr::log::message& msg) override
    {
        std::tm tm;
        unsigned ns;
        gkr::log::decompose_stamp(true, msg.stamp, tm, ns);

        using ulonglong = unsigned long long;

        std::printf("[%02i:%02i:%02i.%03u] (Thread (%llu): %s) | (Severity: %s) | (Facility: %s) - %s\n",
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec,
            (ns / 1000000U),
            ulonglong(msg.tid),
            msg.threadName,
            msg.severityName,
            msg.facilityName,
            msg.messageText
            );
    }
public:
    virtual ~console_consumer() override = default;
};

#if 1
#define SEVERITY_FATAL   0
#define SEVERITY_ERROR   1
#define SEVERITY_WARNING 2
#define SEVERITY_INFO    3
#define SEVERITY_VERBOSE 4

#define FACILITY_GENERAL 0
#define FACILITY_NETWORK 1
#define FACILITY_FILESYS 2
#define FACILITY_SYNCHRO 3

#include <gkr/log/logging.h>

static gkr::log::name_id_pair g_severities[] = {
    {"Fatal"  , SEVERITY_FATAL  },
    {"Error"  , SEVERITY_ERROR  },
    {"Warning", SEVERITY_WARNING},
    {"Info"   , SEVERITY_INFO   },
    {"Verbose", SEVERITY_VERBOSE},
    {nullptr  , 0               }
};
static gkr::log::name_id_pair g_facilities[] = {
    {"General", FACILITY_GENERAL},
    {"Network", FACILITY_NETWORK},
    {"FileSys", FACILITY_FILESYS},
    {"Synchro", FACILITY_SYNCHRO},
    {nullptr  , 0               }
};
#endif

#include <gkr/waitable_event.h>
#include <gkr/waitable_mutex.h>
#include <gkr/objects_waiter.h>
#include <gkr/waitable_semaphore.h>

#include <gkr/sys/thread_name.h>

#define TEST_CALL_CONV /*__cdecl*/

static int TEST_CALL_CONV actual_function_difference(int a, int b)
{
    return (a - b);
}

template<typename R, typename... Args>
static R execute_function_proxy(R (TEST_CALL_CONV *func)(Args...), void** params)
{
// Ignore clang and GNUC warnings
#if defined(__clang__)
#pragma clang diagnostic ignored "-Wunsequenced"
#elif defined(__GNUC__)
#pragma GCC diagnostic ignored "-Wsequence-point"
#endif

#if 1 // The correct way to call arguments with right-to-left placement in stack
    const std::size_t count = reinterpret_cast<std::size_t>(*params);

    params += count;

    R ret = (*func)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*params--)...);

#else // The correct way to call arguments with left-to-right placement in stack

    R ret = (*func)(*reinterpret_cast<typename std::remove_reference<Args>::type*>(*++params)...);

#endif
    return ret;
}

template<typename R, typename... Args>
static R execute_function_remote(Args... args)
{
    constexpr std::size_t count = sizeof...(args);

    void* params[count + 1] = {reinterpret_cast<void*>(count), static_cast<void*>(std::addressof(args))...};

    return execute_function_proxy<R>(actual_function_difference, params);
}

static bool is_right_to_left_args_in_stack()
{
    int result = execute_function_remote<int>(10, 1);

    return (result > 0);
}
#include <cstring>
int main()
{
    int n  =0;

    bool flag = is_right_to_left_args_in_stack();

    n += int(flag);

#if 1
    gkr::sys::set_current_thread_name("gkr-main-thread");

    gkr::log::logging::init(g_severities, g_facilities);

    gkr::log::logging::add_consumer(std::make_shared<console_consumer>());

    gkr::log::logging::log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First log message");
    gkr::log::logging::log_format_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second log message");
#endif

    gkr::waitable_mutex<> mutex;
    gkr::waitable_event<> event1, event2;
//  gkr::waitable_semaphore<> semaphore(1);

    event1.fire();
    event2.fire();

    gkr::objects_waiter waiter;

    auto wait_result = waiter.wait(mutex, event1, event2/*, semaphore*/);

    if(auto guard = gkr::guard_waitable_object(wait_result, 0, mutex))
    {
    }
    if(auto guard = gkr::guard_waitable_object(wait_result, 1, event1))
    {
    }
    if(auto guard = gkr::guard_waitable_object(wait_result, 2, event2))
    {
    }
//  if(auto guard = gkr::guard_waitable_object(wait_result, 3, semaphore); guard.wait_is_completed())
//  {
//  }

#if 0
    gkr::log::logging::done();
#endif
//  test_lockfree_queue();
//  test_waiters();
//  test_thread_worker();

    return n;
}
