#pragma once

#include <gkr/capi/log/consumers/udp_consumer.h>

#include <gkr/api.hpp>
#include <gkr/log/consumer.hpp>

#include <string>

namespace gkr
{
namespace net
{
class udp_sender;
}
namespace log
{

class udp_consumer : public consumer
{
    std::string m_host_name;
    std::string m_proc_name;

    net::udp_sender* m_sender;

    char*       m_buf = nullptr;
    std::size_t m_cap = 0;

    unsigned m_process_id = 0;

public:
    static constexpr std::size_t OPTIMAL_UDP_PACKET_SIZE = 1400;
    static constexpr std::size_t MINIMUM_UDP_PACKET_SIZE = 80;

    GKR_LOG_API udp_consumer(
        const char*    remoteHost,
        unsigned short remotePort,
        unsigned maxPacketSize = OPTIMAL_UDP_PACKET_SIZE
        );
    GKR_LOG_API virtual ~udp_consumer() override;

public:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

protected:
    GKR_LOG_API virtual const char* compose_output(const message& msg, unsigned* len, int flags);

private:
    void update_buffer(std::size_t size);

    bool construct_data(const message& msg);
};

}
}
