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

using namespace gkr::log;

static name_id_pair g_severities[] = {
    {"Fatal"  , SEVERITY_FATAL  },
    {"Error"  , SEVERITY_ERROR  },
    {"Warning", SEVERITY_WARNING},
    {"Info"   , SEVERITY_INFO   },
    {"Verbose", SEVERITY_VERBOSE},
    {nullptr  , 0               }
};
static name_id_pair g_facilities[] = {
    {"General", FACILITY_GENERAL},
    {"Network", FACILITY_NETWORK},
    {"FileSys", FACILITY_FILESYS},
    {"Synchro", FACILITY_SYNCHRO},
    {nullptr  , 0               }
};

//static logging s_logging(g_severities, g_facilities);

#include <thread>
#include <gkr/sys/thread_name.h>

static void bar()
{
    gkr::sys::set_current_thread_name("gkr-bar");
    logging::log_simple_message(false, SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread message");
}

int test_logging()
{
    logging::init(g_severities, g_facilities);

    logging::add_consumer(std::make_shared<console_consumer>());

    logging::log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First log message");

    std::thread t([] () { bar(); });
    t.join();

    logging::log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second log message");

    logging::done();

    return 0;
}
