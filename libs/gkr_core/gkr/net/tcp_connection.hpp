#pragma once

#include <gkr/api.hpp>

#include <gkr/net/address.hpp>
#include <gkr/net/socket.hpp>

#include <utility>

namespace gkr
{
namespace net
{

class tcp_connection
{
    tcp_connection           (const tcp_connection&) noexcept = delete;
    tcp_connection& operator=(const tcp_connection&) noexcept = delete;

public:
    tcp_connection(tcp_connection&& other) noexcept
        : m_socket(std::move(other.m_socket))
    {
    }
    tcp_connection& operator=(tcp_connection&& other) noexcept
    {
        m_socket = std::move(other.m_socket);
        return *this;
    }

public:
    tcp_connection() noexcept
    {
    }
    ~tcp_connection()
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
    friend class tcp_listener;

    bool attach(net::socket&& socket);

public:
    GKR_INNER_API bool connect(const net::address& addr);

    GKR_INNER_API void close();

public:
    GKR_INNER_API std::size_t read_data(void* buffer, std::size_t size);

    GKR_INNER_API std::size_t write_data(const void* buffer, std::size_t size);

private:
    net::socket m_socket;
};

}
}
