#pragma once

#include <gkr/log/consumer.h>
#include <gkr/log/api.h>

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API void* udpSocketCreateConsumerParam(
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize,
    unsigned bufferCapacity
    );

GKR_LOG_API int udpSocketInitLogging(void* param);

GKR_LOG_API void udpSocketDoneLogging(void* param);

GKR_LOG_API int udpSocketFilterLogMessage(void* param, const struct gkr_log_message* msg);

GKR_LOG_API void udpSocketConsumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}

#include <gkr/net/socket.h>
#include <gkr/net/address.h>
#include <gkr/container/raw_buffer.h>

#include <string>

namespace gkr
{
namespace log
{

class udp_socket_consumer : public consumer
{
    udp_socket_consumer() noexcept = delete;

    udp_socket_consumer           (const udp_socket_consumer&) noexcept = delete;
    udp_socket_consumer& operator=(const udp_socket_consumer&) noexcept = delete;

public:
    udp_socket_consumer(udp_socket_consumer&& other) noexcept(
        std::is_nothrow_move_constructible<gkr::raw_buffer_t>::value
        )
        : m_processName(std::move(other.m_processName))
        , m_hostName   (std::move(other.m_hostName))
        , m_packet     (std::move(other.m_packet))
        , m_buffer     (std::move(other.m_buffer))
        , m_socket     (std::move(other.m_socket))
        , m_remoteAddr (std::move(other.m_remoteAddr))
        , m_processId  (std::exchange(other.m_processId, 0))
        , m_packetId   (std::exchange(other.m_packetId , 0))
    {
    }
    udp_socket_consumer& operator=(udp_socket_consumer&& other) noexcept(
        std::is_nothrow_move_assignable<gkr::raw_buffer_t>::value
        )
    {
        m_processName = std::move(other.m_processName);
        m_hostName    = std::move(other.m_hostName);
        m_packet      = std::move(other.m_packet);
        m_buffer      = std::move(other.m_buffer);
        m_socket      = std::move(other.m_socket);
        m_remoteAddr  = std::move(other.m_remoteAddr);

        m_processId   = std::exchange(other.m_processId, 0);
        m_packetId    = std::exchange(other.m_packetId , 0);
        return *this;
    }

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

    GKR_LOG_API virtual bool filter_log_message(const message& msg) override;

    GKR_LOG_API virtual void consume_log_message(const message& msg) override;

public:
    bool set_remote_address(const char* remoteHost, unsigned short remotePort)
    {
        return m_remoteAddr.reset(remoteHost, remotePort);
    }

private:
    bool retrieve_process_name();
    bool retrieve_host_name();

    void constructData(const message& msg);
    void postData(const char* data, std::size_t size);

private:
    std::string m_processName;
    std::string m_hostName;

    gkr::raw_buffer_t m_packet;
    gkr::raw_buffer_t m_buffer;

    gkr::net::socket  m_socket;
    gkr::net::address m_remoteAddr;

    unsigned      m_processId {0};
    std::uint64_t m_packetId  {0};
};

}
}

#endif