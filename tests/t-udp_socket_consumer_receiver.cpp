#include <udpSocketConsumer.h>
#include <udpSocketReceiver.h>
#include <udpMessagePacket.h>

#include <gkr/stamp.h>
#include <gkr/net/lib.h>
#include <gkr/net/address.h>
#include <gkr/log/logging.h>
#include <gkr/concurency/worker_thread.h>

#include <catch2/catch_test_macros.hpp>

#include <thread>
#include <iostream>

#define SEVERITY_FATAL   0
#define SEVERITY_ERROR   1
#define SEVERITY_WARNING 2
#define SEVERITY_INFO    3
#define SEVERITY_VERBOSE 4

#define FACILITY_GENERAL 0
#define FACILITY_NETWORK 1
#define FACILITY_FILESYS 2
#define FACILITY_SYNCHRO 3

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
static gkr::log::logging logging(g_severities, g_facilities);
static gkr::net::lib     networking;

constexpr unsigned short UDP_COMM_PORT = 12345;

class receiver : public gkr::worker_thread
{
    virtual const char* get_name() noexcept override
    {
        return "udp-receiver";
    }
    virtual std::chrono::nanoseconds get_wait_timeout() noexcept override
    {
        return std::chrono::nanoseconds::zero();
    }
    virtual void bind_events(gkr::events_waiter& waiter) noexcept(DIAG_NOEXCEPT) override
    {
    }
    virtual bool on_start() override
    {
        return m_receiver.setWaitPacketTimeout(1000) && m_receiver.startReceivingPackets(UDP_COMM_PORT);
    }
    virtual void on_finish() override
    {
        m_receiver.stopReceivingPackets();
    }
    virtual void on_action(action_id_t action, void* param, void* result) override
    {
    }
    virtual void on_wait_timeout() override
    {
        if(m_receiver.receivePacket())
        {
            gkr::net::address addr;
            const void* data;
            std::size_t size;
            while(m_receiver.getReadyPacketData(addr, data, size))
            {
                const gkr::log::message_data& msg = *static_cast<const gkr::log::message_data*>(data);

                if(msg.head.signature == gkr::log::SIGNITURE_LOG_MSG)
                {
                    std::tm tm;
                    unsigned ns;
                    gkr_stamp_decompose(true, msg.info.stamp, &tm, ns);

                    const char* base = static_cast<const char*>(data);

                    char host[80];
                    addr.host(host);

                    using ulonglong = unsigned long long;
                    char str[256];
                    std::snprintf(
                        str,
                        256,
                        "[%02i:%02i:%02i.%03u] (Host: %s [%s])(Process: %s [%u])(Thread: %s [%llu])(Severity: %s)(Facility: %s) - ",
                        tm.tm_hour,
                        tm.tm_min,
                        tm.tm_sec,
                        (ns / 1000000U),
                        (base + msg.desc.offset_to_host),
                        host,
                        (base + msg.desc.offset_to_process),
                        msg.desc.pid,
                        (base + msg.desc.offset_to_thread),
                        ulonglong(msg.info.tid),
                        (base + msg.desc.offset_to_severity),
                        (base + msg.desc.offset_to_facility)
                        );
                    std::cout << str << (base + msg.desc.offset_to_text) << std::endl;
                }
                if(--m_count == 0) quit();
            }
        }
    }
    virtual void on_wait_success(gkr::wait_result_t wait_result) override
    {
    }
    virtual bool on_exception(except_method_t method, const std::exception* e) noexcept override
    {
        return false;
    }

private:
    gkr::udpSocketReceiver m_receiver;
    unsigned m_count;

public:
    receiver(unsigned maxMsgCount) : m_receiver(), m_count(maxMsgCount)
    {
    }
    virtual ~receiver() override
    {
        join(true);
    }
};

TEST_CASE("logging.consumer.udp_socket. Simple packet")
{
    receiver recv(3);
    REQUIRE(recv.run());

    using namespace gkr::log;

    gkr_log_del_all_consumers();
    gkr_log_add_consumer(std::make_shared<gkr::udpSocketConsumer>("127.0.0.1", UDP_COMM_PORT));

    gkr_log_set_this_thread_name("thread-0");

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First simple log message");

    std::thread t([] () {
        gkr_log_set_this_thread_name("thread-1");
        gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread simple log message");
    });
    t.join();

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second simple log message");

    gkr_log_done();
}

TEST_CASE("logging.consumer.udp_socket. Splitted packet")
{
    receiver recv(3);
    REQUIRE(recv.run());

    gkr_log_add_consumer(std::make_shared<gkr::udpSocketConsumer>("127.0.0.1", UDP_COMM_PORT, 128));

    gkr_log_set_this_thread_name("thread-0");

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First splitted log message");

    std::thread t([] () {
        gkr_log_set_this_thread_name("thread-1");
        gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread splitted log message");
    });
    t.join();

    gkr_log_simple_message(false, SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second splitted log message");

    gkr_log_done();
}
