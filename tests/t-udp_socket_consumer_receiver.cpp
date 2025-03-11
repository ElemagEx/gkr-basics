#include <gkr/log/consumers/udp_socket_consumer.hpp>
#include <gkr/log/consumer.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/defs/generic.hpp>

#include <gkr/testing/log_defs.hpp>

#include <gkr/stamp.hpp>
#include <gkr/comm/udp_socket_receiver.hpp>
#include <gkr/data/log_message.hpp>

#include <gkr/net/address.hpp>
#include <gkr/concurency/worker_thread.hpp>

#include <catch2/catch_test_macros.hpp>

#include <thread>
#include <iostream>

#define COMMON_SEVERITIES_INFOS LOG_SEVERITIES_INFOS
constexpr gkr::log::name_id_pair g_severities_infos[] = COMMON_SEVERITIES_INFOS;
constexpr gkr::log::name_id_pair g_facilities_infos[] = COMMON_FACILITIES_INFOS;

static gkr::log::logging logging(nullptr, 0, 0, g_severities_infos, g_facilities_infos);

constexpr unsigned short UDP_COMM_PORT = 12345;

class receiver : public gkr::worker_thread
{
    virtual const char* get_name() noexcept override
    {
        return "udp-receiver";
    }
    virtual long long get_wait_timeout_ns() noexcept override
    {
        return 0;
    }
    virtual std::size_t get_waitable_objects_count() noexcept override
    {
        return 0;
    }
    virtual gkr::waitable_object& get_waitable_object(std::size_t index) noexcept override
    {
        return gkr::waitable_object::null();
    }
    virtual bool on_start() override
    {
        return m_receiver.setWaitPacketTimeout(1000) && m_receiver.startReceivingPackets(UDP_COMM_PORT);
    }
    virtual void on_finish() override
    {
        m_receiver.stopReceivingPackets();
    }
    virtual void on_cross_action(action_id_t action, void* param, void* result) override
    {
        Check_Failure();
    }
    virtual void on_queue_action(action_id_t action, void* data) override
    {
        Check_Failure();
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
                const gkr::data::log_message& msg = *static_cast<const gkr::data::log_message*>(data);

                if(msg.signature == gkr::data::SIGNITURE_LOG_MESSAGE)
                {
                    struct tm tm;
                    int ns = gkr::stamp_decompose(true, msg.stamp, tm);

                    const char* base = static_cast<const char*>(data);

                    char host[80];
                    addr.host(host);

                    //using longlong = long long;
                    char str[256];
                    std::snprintf(
                        str,
                        256,
                        "[%02i:%02i:%02i.%03d] (Host: %s [%s])(Process: %s [%u])(Thread: %s [%jd])(Severity: %s)(Facility: %s) - ",
                        tm.tm_hour,
                        tm.tm_min,
                        tm.tm_sec,
                        ns / 1000000,
                        base + msg.offset_to_host,
                        host,
                        base + msg.offset_to_process,
                        msg.pid,
                        base + msg.offset_to_thread,
                        msg.tid,
                        base + msg.offset_to_severity,
                        base + msg.offset_to_facility
                        );
                    std::cout << str << (base + msg.offset_to_text) << std::endl;
                }
                if(--m_count == 0) quit();
            }
        }
    }
    virtual void on_wait_success(std::size_t index) override
    {
        Check_Failure();
    }
    virtual bool on_exception(except_method_t method, const std::exception* e) noexcept override
    {
        return false;
    }

private:
    gkr::comm::udp_socket_receiver m_receiver;
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

    REQUIRE(gkr_log_del_all_consumers(nullptr));
    REQUIRE(gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::udp_socket_consumer>("127.0.0.1", UDP_COMM_PORT)));

    REQUIRE(gkr_log_set_current_thread_name("thread-0"));

    REQUIRE(gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First simple log message"));

    std::thread t([] () {
        gkr_log_set_current_thread_name("thread-1");
        gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread simple log message");
    });
    t.join();

    REQUIRE(gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second simple log message"));

    recv.join(false);
}

TEST_CASE("logging.consumer.udp_socket. Splitted packet")
{
    receiver recv(3);
    REQUIRE(recv.run());

    REQUIRE(gkr_log_del_all_consumers(nullptr));
    REQUIRE(gkr_log_add_consumer(nullptr, std::make_shared<gkr::log::udp_socket_consumer>("127.0.0.1", UDP_COMM_PORT, 128)));

    REQUIRE(gkr_log_set_current_thread_name("thread-A"));

    REQUIRE(gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_SYNCHRO, "First splitted log message"));

    std::thread t([] () {
        gkr_log_set_current_thread_name("thread-B");
        gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_FILESYS, "Other thread splitted log message");
    });
    t.join();

    REQUIRE(gkr_log_simple_message(nullptr, LOG_SEVERITY_VERBOSE, FACILITY_SYNCHRO, "Second splitted log message"));

    recv.join(false);
}
