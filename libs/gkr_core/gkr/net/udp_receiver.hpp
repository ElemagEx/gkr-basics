#pragma once

#include <gkr/api.hpp>

#include <gkr/container/raw_buffer.hpp>
#include <gkr/net/address.hpp>
#include <gkr/net/socket.hpp>
#include <gkr/net/lib.hpp>

#include <vector>
#include <chrono>

namespace gkr
{
namespace data
{
    struct split_packet_head;
}
namespace net
{

class udp_receiver : private net::lib
{
    udp_receiver           (const udp_receiver&) noexcept = delete;
    udp_receiver& operator=(const udp_receiver&) noexcept = delete;

public:
    udp_receiver(udp_receiver&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer>::value
        )
        : m_socket(std::move(other.m_socket))
        , m_packet(std::move(other.m_packet))
    {
    }
    udp_receiver& operator=(udp_receiver&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer>::value
        )
    {
        m_socket = std::move(other.m_socket);
        m_packet = std::move(other.m_packet);
        return *this;
    }

public:
    GKR_INNER_API  udp_receiver(std::size_t maxPacketSize = 2*1024, bool useIPv6 = false);
    GKR_INNER_API ~udp_receiver();

public:
    GKR_INNER_API bool setWaitPacketTimeout(unsigned waitPacketTimeout);

public:
    GKR_INNER_API bool startReceivingPackets(unsigned short port);
    GKR_INNER_API void  stopReceivingPackets();

public:
    GKR_INNER_API bool receivePacket();
    GKR_INNER_API bool getReadyPacketData(net::address& addr, const void*& data, std::size_t& size);

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
        raw_buffer      buffer;
        time_point_t    updated;

        void reset() { count = 0; }
    };

    partial_packet_t& findPartialPacket(const data::split_packet_head& packetHead, std::size_t& partialDataSize);

private:
    net::socket  m_socket;
    raw_buffer   m_packet;
    raw_buffer   m_buffer;

    net::address m_addr;
    std::size_t  m_offset {0};

    std::vector<partial_packet_t> m_partialPackets;
};

}
}
