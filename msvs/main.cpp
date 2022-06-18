#include "main.h"

#include <chrono>
#include <ctime>
#include <thread>

#include <forward_list>

class A
{
    int a;
    A() : a(5)
    {
    }
public:
    int get() { return a; }
    static A create()
    {
        return A();
    }
};

static bool foo(const A&)
{
    return true;
}

static bool bar(int n)
{
    return ((n > 4) || !foo(A::create()));
}

#include <gkr/log/logging.h>
#include <gkr/log/consumer.h>
#include <gkr/log/entry.h>

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
    }
public:
    virtual ~console_consumer() override = default;
};

int main()
{
    gkr::log::logging::init(g_severities, g_facilities);

    gkr::log::logging::add_consumer(std::make_shared<console_consumer>());

    gkr::log::logging::set_facility({"Synchronization", FACILITY_SYNCRO});

    int n = int(sizeof(void*));

    n += int(sizeof(std::thread::id));

    bool f = bar(n/1);

    n += f ? 10 : 1;

    gkr::log::logging::done();

#if 0
    int64_t stamp;
    std::tm tm;

    size_t a = sizeof(std::thread::id);

    {
        auto now = std::chrono::system_clock::now();

        auto dur = now.time_since_epoch();

        auto ns  = std::chrono::duration_cast<std::chrono::nanoseconds>(dur);

        stamp = ns.count();

        std::time_t time1 = std::chrono::system_clock::to_time_t(now);
        std::time_t time2 = time1 * 1000000000;

#ifdef _MSC_VER
        gmtime_s(&tm, &time1);
#else
        gmtime_r(&time1, &tm);
#endif

        int64_t nanoseconds = (stamp - time2);

        ++n;
    }

    {
        auto ns  = std::chrono::nanoseconds(stamp);

        auto dur = std::chrono::duration_cast<std::chrono::system_clock::duration>(ns);

        auto now = std::chrono::system_clock::time_point(dur);

        std::time_t time1 = std::chrono::system_clock::to_time_t(now);
        std::time_t time2 = time1 * 1000000000;

#ifdef _MSC_VER
        gmtime_s(&tm, &time1);
#else
        gmtime_r(&time1, &tm);
#endif

        int64_t nanoseconds = (stamp - time2);

        ++n;
    }

#endif


//  test_lockfree_queue();
//  test_waiters();
//  test_thread_worker();

    return n;
}
