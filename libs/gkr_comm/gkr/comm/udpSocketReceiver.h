#pragma once

#include <gkr/api.h>

#include <gkr/container/raw_buffer.h>
#include <gkr/net/address.h>
#include <gkr/net/socket.h>

#include <vector>
#include <chrono>

namespace gkr
{

namespace net { struct split_packet_head; }

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
    GKR_COMM_API udpSocketReceiver(
        std::size_t maxPacketSize = 2*1024,
        bool useIPv6 = false
        );
    GKR_COMM_API ~udpSocketReceiver();

public:
    GKR_COMM_API bool setWaitPacketTimeout(unsigned waitPacketTimeout);

public:
    GKR_COMM_API bool startReceivingPackets(unsigned short port);
    GKR_COMM_API void  stopReceivingPackets();

public:
    GKR_COMM_API bool receivePacket();
    GKR_COMM_API bool getReadyPacketData(net::address& addr, const void*& data, std::size_t& size);

private:
    void handleUnsplittedPacket();
    void handlePartialPacket();

private:
    using time_point_t = std::chrono::time_point<std::chrono::steady_clock>;
    using packet_id_t  = unsigned long long;

    struct partial_packet_t
    {
        std::size_t     count    = 0;
        std::size_t     received = 0;
        packet_id_t     id       = 0;
        net::address    sender;
        raw_buffer_t    buffer;
        time_point_t    updated;

        void reset() { count = 0; }
    };

    partial_packet_t& findPartialPacket(const net::split_packet_head& packetHead, std::size_t& partialDataSize);

private:
    net::socket  m_socket;
    raw_buffer_t m_packet;
    raw_buffer_t m_buffer;

    net::address m_addr;
    std::size_t  m_offset {0};

    std::vector<partial_packet_t> m_partialPackets;
};

}
