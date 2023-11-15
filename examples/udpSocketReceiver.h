#pragma once

#include <gkr/container/raw_buffer.h>
#include <gkr/net/address.h>
#include <gkr/net/socket.h>

namespace gkr
{

class udpSocketReceiver
{
    udpSocketReceiver           (const udpSocketReceiver&) noexcept = delete;
    udpSocketReceiver& operator=(const udpSocketReceiver&) noexcept = delete;

public:
    udpSocketReceiver(udpSocketReceiver&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer_t>::value
        )
        : m_socket(std::move(other.m_socket))
        , m_packet(std::move(other.m_packet))
    {
    }
    udpSocketReceiver& operator=(udpSocketReceiver&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer_t>::value
        )
    {
        m_socket = std::move(other.m_socket);
        m_packet = std::move(other.m_packet);
        return *this;
    }

public:
    udpSocketReceiver(
        std::size_t maxPacketSize = 2*1024,
        bool useIPv6 = false
        );
    ~udpSocketReceiver();

public:
    bool setWaitPacketTimeout(unsigned waitPacketTimeout);

public:
    bool startReceivingPackets(unsigned short port);
    void  stopReceivingPackets();

public:
    bool receivePacket();
    bool getPacketData(net::address& addr, const void*& data, std::size_t& size);

private:
    bool handleUnsplittedPacket();

private:
    net::socket  m_socket;
    raw_buffer_t m_packet;

    raw_buffer_t m_buffer;
    net::address m_addr;
    std::size_t  m_offset {0};
};

}
