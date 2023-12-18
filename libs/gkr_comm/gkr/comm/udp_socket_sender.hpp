#pragma once

#include <gkr/comm/api.h>

#include <gkr/container/raw_buffer.hpp>
#include <gkr/net/address.hpp>
#include <gkr/net/socket.hpp>

namespace gkr
{
namespace comm
{

class udp_socket_sender
{
    udp_socket_sender           (const udp_socket_sender&) noexcept = delete;
    udp_socket_sender& operator=(const udp_socket_sender&) noexcept = delete;

public:
    udp_socket_sender(udp_socket_sender&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer_t>::value
        )
        : m_packetId(std::exchange(other.m_packetId, 0U))
        , m_address (std::move(other.m_address))
        , m_socket  (std::move(other.m_socket ))
        , m_buffer  (std::move(other.m_buffer ))
    {
    }
    udp_socket_sender& operator=(udp_socket_sender&& other) noexcept(
        std::is_nothrow_move_assignable<raw_buffer_t>::value
        )
    {
        m_packetId = std::exchange(other.m_packetId, 0U);
        m_address  = std::move(other.m_address);
        m_socket   = std::move(other.m_socket );
        m_buffer   = std::move(other.m_buffer );
        return *this;
    }

public:
    static constexpr std::size_t MINIMUM_UDP_PACKET_SIZE = 80;

    GKR_COMM_API  udp_socket_sender(std::size_t maxPacketSize = 2*1024);
    GKR_COMM_API ~udp_socket_sender();

public:
    GKR_COMM_API bool start_sending_packets();
    GKR_COMM_API void  stop_sending_packets();

public:
    GKR_COMM_API bool send_data(const char* data, std::size_t size);

public:
    bool change_remote_address(const char* remoteHost, unsigned short remotePort)
    {
        return m_address.reset(remoteHost, remotePort);
    }

public:
    const net::address& remote_address() const
    {
        return m_address;
    }
    std::size_t max_packet_size() const
    {
        return m_buffer.capacity();
    }
    bool is_started() const
    {
        return m_socket.is_open();
    }

private:
    using packet_id_t = unsigned long long;

    packet_id_t m_packetId {0};

    gkr::net::address m_address;
    gkr::net::socket  m_socket;
    gkr::raw_buffer_t m_buffer;
};

}
}
