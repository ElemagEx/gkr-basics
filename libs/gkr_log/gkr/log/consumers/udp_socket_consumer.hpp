#pragma once

#include <gkr/api.h>
#include <gkr/log/consumer.hpp>
#include <gkr/log/consumers/udp_socket_callbacks.h>

#include <string>

namespace gkr
{
namespace comm
{
class udp_socket_sender;
}
namespace log
{

class udp_socket_consumer : public consumer
{
    std::string m_hostName;
    std::string m_processName;

    comm::udp_socket_sender* m_sender;

    char*       m_bufferPtr;
    std::size_t m_bufferCap;

    unsigned m_processId {0};

public:
    static constexpr std::size_t OPTIMAL_UDP_PACKET_SIZE = 1400;
    static constexpr std::size_t MINIMUM_UDP_PACKET_SIZE = 80;

    GKR_LOG_API udp_socket_consumer(
        const char*    remoteHost,
        unsigned short remotePort,
        unsigned maxPacketSize  = OPTIMAL_UDP_PACKET_SIZE,
        unsigned bufferCapacity = 2*1024
        );
    GKR_LOG_API virtual ~udp_socket_consumer() override;

public:
    GKR_LOG_API virtual bool init_logging() override;
    GKR_LOG_API virtual void done_logging() override;

    GKR_LOG_API virtual bool  filter_log_message(const message& msg) override;
    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

private:
    bool retrieve_process_name();
    bool retrieve_host_name();

    bool construct_data(const message& msg);
};

}
}