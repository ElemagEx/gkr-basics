#include <plog/Log.h>
#include <plog/Formatters/MessageOnlyFormatter.h>
#include <plog/Initializers/ConsoleInitializer.h>

#include <gkr/log/logging.hpp>

#include <gkr/log/consumer.hpp>
#include <gkr/stamp.hpp>

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

        std::printf("[%02i:%02i:%02i.%03d] (Thread (%lld): %s) | (Severity: %s) | (Facility: %s) - %s\n",
            tm.tm_hour,
            tm.tm_min,
            tm.tm_sec,
            ns / 1000000,
            msg.tid,
            msg.threadName,
            msg.severityName,
            msg.facilityName,
            msg.messageText
            );
    }
public:
    virtual ~console_consumer() override = default;
};

#include <gkr/log/logging.h>
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

constexpr gkr::log::name_id_pair g_severities_infos[] = {
    {"Fatal"  , SEVERITY_FATAL  },
    {"Error"  , SEVERITY_ERROR  },
    {"Warning", SEVERITY_WARNING},
    {"Info"   , SEVERITY_INFO   },
    {"Verbose", SEVERITY_VERBOSE},
    {nullptr  , 0               }
};
constexpr gkr::log::name_id_pair g_facilities_infos[] = {
    {"General", FACILITY_GENERAL},
    {"Network", FACILITY_NETWORK},
    {"FileSys", FACILITY_FILESYS},
    {"Synchro", FACILITY_SYNCHRO},
    {nullptr  , 0               }
};
#endif
#include <thread>
#if 0
TEST_CASE("logging.logger. main")
{
    gkr_log_init(g_severities_infos, g_facilities_infos, 16, 1023/*63*/);

    gkr_log_add_consumer(std::make_shared<console_consumer>());

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First log message");

    std::thread t([] () {
        gkr_log_set_this_thread_name("gkr-bar");
        gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread message");
    });
    t.join();

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second log message");

//  gkr_log_format_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Hello {}! How are you", "world");

    gkr::log::ostream(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO)
        << "abcdefghijklmnopqrstuvwxyz" << " + "
        << "ABCDEFGHIJKLMNOPQRSTUVWXYZ" << " + "
        << "0123456789";

    gkr_log_done();
}
#endif
TEST_CASE("logging.imposter.plog. main")
{
    //plog::init(); // 0 args
    //plog::init(plog::debug); // 1 args
    //static plog::ConsoleAppender<plog::MessageOnlyFormatter> appender;
    //plog::init(plog::debug, &appender); // 2 args

    //plog::init<plog::FuncMessageFormatter>(plog::debug, plog::streamStdOut);


    PLOGV << "Hello log!";
    PLOG_DEBUG << "Hello log!";
    PLOG(plog::info) << "Hello log!";
    
    LOGW << "Hello log!";
    LOG_ERROR << "Hello log!";
    LOG(plog::fatal) << "Hello log!";

    PLOG_NONE << "Hello log!";

    //TODO:Decrement logging refCount
}
