#include "main.h"

#include <gkr/log/logging.h>
#include <gkr/log/consumer.h>
#include <gkr/log/entry.h>
#include <gkr/sys/thread_name.h>
#include <gkr/misc/stamp.h>
#if 0
#define SEVERITY_FATAL   0
#define SEVERITY_ERROR   1
#define SEVERITY_WARNING 2
#define SEVERITY_INFO    3
#define SEVERITY_VERBOSE 4

#define FACILITY_GENERAL 0
#define FACILITY_NETWORK 1
#define FACILITY_FILESYS 2
#define FACILITY_SYNCHRO 3

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
    virtual void consume_log_message(const gkr::log::entry_info& entry) override
    {
        std::tm tm;
        unsigned ns;
        gkr::misc::decompose_stamp(true, entry.stamp, tm, ns);

        using ulonglong = unsigned long long;

        std::printf("[%02i:%02i:%02i.%03u] (Thread (%llu): %s) | (Severity: %s) | (Facility: %s) - %s\n",
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec,
            (ns / 1000000U),
            ulonglong(entry.tid),
            entry.threadName,
            entry.severityName,
            entry.facilityName,
            entry.messageText
            );
    }
public:
    virtual ~console_consumer() override = default;
};

#include <gkr/waitable_event.h>
#include <gkr/waitable_mutex.h>
#include <gkr/objects_waiter.h>
#include <gkr/waitable_semaphore.h>

int main()
{
    int n  =0;
#if 0
    gkr::sys::set_current_thread_name("gkr-main-thread");

    gkr::log::logging::init(g_severities, g_facilities);

    gkr::log::logging::add_consumer(std::make_shared<console_consumer>());

    gkr::log::logging::log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCRO, "First log message");
    gkr::log::logging::log_format_message(false, SEVERITY_VERBOSE, FACILITY_SYNCRO, "Second log message %i", 10);
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
