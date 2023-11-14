#pragma once

#include <gkr/container/raw_buffer.h>
#include <gkr/net/socket.h>

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
        , m_socket(std::move(other.m_socket))
    {
    }
    udpSocketReceiver& operator=(udpSocketReceiver&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer_t>::value
        )
    {
        m_packet = std::move(other.m_packet);
        m_buffer = std::move(other.m_buffer);
        m_socket = std::move(other.m_socket);
        return *this;
    }

public:
    udpSocketReceiver(
        std::size_t maxPacketSize = 2*1024,
        std::size_t bufferInitialCapacity = 2*1024,
        bool useIPv6 = false
        );
    ~udpSocketReceiver();

public:
    bool startReceivingPackets(unsigned short port, unsigned waitPacketTime = 1000);
    void  stopReceivingPackets();

private:
    raw_buffer_t    m_packet;
    raw_buffer_t    m_buffer;
    net::socket     m_socket;
};

}
