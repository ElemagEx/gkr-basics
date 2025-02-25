#pragma once

#include <gkr/api.hpp>

#include <gkr/net/address.hpp>
#include <gkr/net/socket.hpp>

#include <utility>

namespace gkr
{
namespace comm
{

class tcp_socket_connection;

class tcp_socket_listener
{
    tcp_socket_listener           (const tcp_socket_listener&) noexcept = delete;
    tcp_socket_listener& operator=(const tcp_socket_listener&) noexcept = delete;

public:
    tcp_socket_listener(tcp_socket_listener&& other) noexcept
        : m_socket(std::move(other.m_socket))
    {
    }
    tcp_socket_listener& operator=(tcp_socket_listener&& other) noexcept
    {
        m_socket = std::move(other.m_socket);
        return *this;
    }

public:
    tcp_socket_listener() noexcept
    {
    }
    ~tcp_socket_listener()
    {
        stop_listening();
    }

public:
    bool is_listening() const
    {
        return m_socket.is_open();
    }

public:
    GKR_COMM_API bool start_listening(unsigned short listeniningPort, bool useIPv6 = false);
    GKR_COMM_API void  stop_listening();

public:
    GKR_COMM_API bool accept(net::address& addr, tcp_socket_connection& connection, unsigned timeout = 0);

private:
    net::socket m_socket;
};

}
}
