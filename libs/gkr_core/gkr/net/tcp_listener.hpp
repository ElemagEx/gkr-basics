#pragma once

#include <gkr/api.hpp>

#include <gkr/net/address.hpp>
#include <gkr/net/socket.hpp>

#include <utility>

namespace gkr
{
namespace net
{

class tcp_connection;

class tcp_listener
{
    tcp_listener           (const tcp_listener&) noexcept = delete;
    tcp_listener& operator=(const tcp_listener&) noexcept = delete;

public:
    tcp_listener(tcp_listener&& other) noexcept
        : m_socket(std::move(other.m_socket))
    {
    }
    tcp_listener& operator=(tcp_listener&& other) noexcept
    {
        m_socket = std::move(other.m_socket);
        return *this;
    }

public:
    tcp_listener() noexcept
    {
    }
    ~tcp_listener()
    {
        stop_listening();
    }

public:
    bool is_listening() const
    {
        return m_socket.is_open();
    }

public:
    GKR_INNER_API bool start_listening(unsigned short listeniningPort, bool useIPv6 = false);
    GKR_INNER_API void  stop_listening();

public:
    GKR_INNER_API bool accept(net::address& addr, tcp_connection& connection, unsigned timeout = 0);

private:
    net::socket m_socket;
};

}
}
