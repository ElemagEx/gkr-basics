#include "main.h"

#include <gkr/log/logging.h>
#include <gkr/log/consumer.h>
#include <gkr/log/entry.h>
#include <gkr/sys/thread_name.h>
#include <gkr/misc/stamp.h>

#define SEVERITY_FATAL   0
#define SEVERITY_ERROR   1
#define SEVERITY_WARNING 2
#define SEVERITY_INFO    3
#define SEVERITY_VERBOSE 4

#define FACILITY_GENERAL 0
#define FACILITY_NETWORK 1
#define FACILITY_FILESYS 2
#define FACILITY_SYNCRO  3

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
    {nullptr  , 0               }
};

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
        gkr::misc::decompose_stamp(true, entry.head.stamp, tm, ns);

        using ulonglong = unsigned long long;
        printf("[%02i:%02i:%02i.%03u] (Thread (%llu): %s) | (Severity: %s) | (Facility: %s) - %s\n",
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec,
            (ns / 1000000U),
            ulonglong(entry.head.tid),
            entry.threadName,
            entry.severityName,
            entry.facilityName,
            entry.messageText
            );
    }
public:
    virtual ~console_consumer() override = default;
};

int main()
{
    gkr::sys::set_current_thread_name("gkr-main-thread");

    gkr::log::logging::init(g_severities, g_facilities);

    gkr::log::logging::add_consumer(std::make_shared<console_consumer>());

    gkr::log::logging::set_facility({"Synchronization", FACILITY_SYNCRO});

    gkr::log::logging::log_simple_message(true, SEVERITY_VERBOSE, FACILITY_SYNCRO, "First log message");
    gkr::log::logging::log_format_message(true, SEVERITY_VERBOSE, FACILITY_SYNCRO, "Second log message %i", 10);

    int n = int(sizeof(void*));

    gkr::log::logging::done();

//  test_lockfree_queue();
//  test_waiters();
//  test_thread_worker();

    return n;
}
