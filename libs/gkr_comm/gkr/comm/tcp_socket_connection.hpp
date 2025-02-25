#pragma once

#include <gkr/api.hpp>

#include <gkr/net/address.hpp>
#include <gkr/net/socket.hpp>

#include <utility>

namespace gkr
{
namespace comm
{

class tcp_socket_connection
{
    tcp_socket_connection           (const tcp_socket_connection&) noexcept = delete;
    tcp_socket_connection& operator=(const tcp_socket_connection&) noexcept = delete;

public:
    tcp_socket_connection(tcp_socket_connection&& other) noexcept
        : m_socket(std::move(other.m_socket))
    {
    }
    tcp_socket_connection& operator=(tcp_socket_connection&& other) noexcept
    {
        m_socket = std::move(other.m_socket);
        return *this;
    }

public:
    tcp_socket_connection() noexcept
    {
    }
    ~tcp_socket_connection()
    {
        close();
    }

public:
    bool is_connected() const
    {
        return m_socket.is_open();
    }
    bool set_read_timeout(unsigned timeout)
    {
        return m_socket.set_receive_timeout(timeout);
    }
    bool set_write_timeout(unsigned timeout)
    {
        return m_socket.set_receive_timeout(timeout);
    }

private:
    friend class tcp_socket_listener;

    bool attach(net::socket&& socket);

public:
    GKR_COMM_API bool connect(const net::address& addr);

    GKR_COMM_API void close();

public:
    GKR_COMM_API std::size_t read_data(void* buffer, std::size_t size);

    GKR_COMM_API std::size_t write_data(const void* buffer, std::size_t size);

private:
    net::socket m_socket;
};

}
}
