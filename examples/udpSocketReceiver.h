#pragma once

#include <gkr/container/raw_buffer.h>

namespace gkr
{

class udpSocketReceiver
{
    udpSocketReceiver() noexcept = delete;

    udpSocketReceiver           (const udpSocketReceiver&) noexcept = delete;
    udpSocketReceiver& operator=(const udpSocketReceiver&) noexcept = delete;

    udpSocketReceiver(udpSocketReceiver&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer_t>::value
        )
        : m_packet(std::move(other.m_packet))
        , m_buffer(std::move(other.m_buffer))
    {
    }
    udpSocketReceiver& operator=(udpSocketReceiver&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer_t>::value
        )
    {
        m_packet = std::move(other.m_packet);
        m_buffer = std::move(other.m_buffer);
        return *this;
    }

public:
    udpSocketReceiver(
        unsigned short port,
        std::size_t maxPacketSize = 2*1024,
        std::size_t bufferInitialCapacity = 2*1024,
        bool useIPv6 = false
        );
    ~udpSocketReceiver();

public:
    bool startReceivingPackets();
    void  stopReceivingPackets();

private:
    bool  openUdpSocket();
    void closeUdpSocket();

private:
#ifdef _WIN32
    using socket_t = std::size_t;
#else
    using socket_t = int;
#endif
    static constexpr socket_t INVALID_SOCKET_VALUE = socket_t(-1);

private:
    raw_buffer_t    m_packet;
    raw_buffer_t    m_buffer;

    socket_t        m_socket  {INVALID_SOCKET_VALUE};
    unsigned short  m_port    {0};
    bool            m_useIPv6 {false};
};

}
