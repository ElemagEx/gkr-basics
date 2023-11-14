#pragma once

#include <gkr/log/consumer.h>
#include <gkr/net/address.h>
#include <gkr/container/raw_buffer.h>

#include <string>

namespace gkr
{

class udpSocketConsumer : public log::consumer
{
    udpSocketConsumer() noexcept = delete;

    udpSocketConsumer           (const udpSocketConsumer&) noexcept = delete;
    udpSocketConsumer& operator=(const udpSocketConsumer&) noexcept = delete;

public:
    udpSocketConsumer(udpSocketConsumer&& other) noexcept(
        std::is_nothrow_move_constructible<raw_buffer_t>::value
        )
        : m_processName(std::move(other.m_processName))
        , m_hostName   (std::move(other.m_hostName))
        , m_packet     (std::move(other.m_packet))
        , m_buffer     (std::move(other.m_buffer))
        , m_remoteAddr (std::move(other.m_remoteAddr))
        , m_processId  (other.m_processId)
        , m_packetId   (std::exchange(other.m_packetId, 0))
        , m_socket     (std::exchange(other.m_socket  , INVALID_SOCKET_VALUE))
    {
    }
    udpSocketConsumer& operator=(udpSocketConsumer&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer_t>::value
        )
    {
        m_processName = std::move(other.m_processName);
        m_hostName    = std::move(other.m_hostName);
        m_packet      = std::move(other.m_packet);
        m_buffer      = std::move(other.m_buffer);
        m_remoteAddr  = std::move(other.m_remoteAddr);

        m_processId   = other.m_processId;

        m_packetId    = std::exchange(other.m_packetId, 0);
        m_socket      = std::exchange(other.m_socket  , INVALID_SOCKET_VALUE);
        return *this;
    }

public:
    static constexpr std::size_t OPTIMAL_UDP_PACKET_SIZE = 1500;
    static constexpr std::size_t MINIMUM_UDP_PACKET_SIZE = 240;

    udpSocketConsumer(
        const char*    remoteHost,
        unsigned short remotePort,
        std::size_t maxPacketSize = OPTIMAL_UDP_PACKET_SIZE,
        std::size_t bufferInitialCapacity = 2*1024
        );
    virtual ~udpSocketConsumer() override;

protected:
    virtual bool init_logging() override;
    virtual void done_logging() override;

    virtual bool filter_log_message(const log::message& msg) override;

    virtual void consume_log_message(const log::message& msg) override;

public:
    bool setRemoteAddress(const char* remoteHost, unsigned short remotePort)
    {
        return m_remoteAddr.reset(remoteHost, remotePort);
    }

private:
    bool retrieveProcessName();
    bool retrieveHostName();

    void constructData(const log::message& msg);
    void sendData(const char* data, std::size_t size);

    bool  openUdpSocket();
    void closeUdpSocket();

    void sendUdpPacket(const void* packet, std::size_t size);

private:
#ifdef _WIN32
    using socket_t = std::size_t;
#else
    using socket_t = int;
#endif
    static constexpr socket_t INVALID_SOCKET_VALUE = socket_t(-1);

private:
    std::string     m_processName;
    std::string     m_hostName;

    raw_buffer_t    m_packet;
    raw_buffer_t    m_buffer;

    net::address    m_remoteAddr;
    int             m_processId {0};
    std::uint64_t   m_packetId  {0};

    socket_t        m_socket {INVALID_SOCKET_VALUE};
};

}
