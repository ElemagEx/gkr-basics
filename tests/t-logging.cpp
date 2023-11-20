#include <gkr/log/consumer.h>
#include <gkr/log/message.h>
#include <gkr/stamp.h>

#include <catch2/catch_test_macros.hpp>

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
    virtual bool filter_log_message(const gkr::log::message& msg) override
    {
        return false;
    }
    virtual void consume_log_message(const gkr::log::message& msg) override
    {
        struct tm tm;
        int ns = gkr::stamp_decompose(true, msg.stamp, tm);

        using ulonglong = unsigned long long;

        std::printf("[%02i:%02i:%02i.%03d] (Thread (%llu): %s) | (Severity: %s) | (Facility: %s) - %s\n",
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec,
            ns / 1000000,
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

constexpr gkr::log::name_id_pair g_severities[] = {
    {"Fatal"  , SEVERITY_FATAL  },
    {"Error"  , SEVERITY_ERROR  },
    {"Warning", SEVERITY_WARNING},
    {"Info"   , SEVERITY_INFO   },
    {"Verbose", SEVERITY_VERBOSE},
    {nullptr  , 0               }
};
constexpr gkr::log::name_id_pair g_facilities[] = {
    {"General", FACILITY_GENERAL},
    {"Network", FACILITY_NETWORK},
    {"FileSys", FACILITY_FILESYS},
    {"Synchro", FACILITY_SYNCHRO},
    {nullptr  , 0               }
};

#include <thread>

TEST_CASE("logging.logger. main")
{
    gkr_log_init(g_severities, g_facilities, 16, 1024);

    gkr_log_add_consumer(std::make_shared<console_consumer>());

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First log message");

    std::thread t([] () {
        gkr_log_set_this_thread_name("gkr-bar");
        gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread message");
    });
    t.join();

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second log message");

    gkr_log_done();
}
