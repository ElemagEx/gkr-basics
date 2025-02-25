#pragma once

#include <gkr/api.hpp>

#include <cstddef>

namespace gkr
{
namespace net
{

class address;

class socket
{
    socket           (const socket&) noexcept = delete;
    socket& operator=(const socket&) noexcept = delete;

public:
    socket(socket&& other) noexcept : m_socket(other.m_socket)
    {
        other.m_socket = INVALID_SOCKET_VALUE;
    }
    socket& operator=(socket&& other) noexcept
    {
        m_socket = other.m_socket;
        other.m_socket = INVALID_SOCKET_VALUE;
        return *this;
    }

    socket() noexcept = default;

    ~socket()
    {
        close();
    }

public:
    bool is_open() const
    {
        return (m_socket != INVALID_SOCKET_VALUE);
    }

public:
    enum : unsigned
    {
        error_none         = 0,
        error_timeout      = (1U <<  0),
        error_conn_refused = (1U <<  1),
        error_conn_aborted = (1U <<  2),
        error_other        = (1U << 31)
    };

public:
    GKR_INNER_API bool open_as_tcp(bool ipv6 = false);
    GKR_INNER_API bool open_as_udp(bool ipv6 = false);
    GKR_INNER_API bool reopen();
    GKR_INNER_API void close();

    GKR_INNER_API bool connect(const address& addr, unsigned* errors = nullptr);

public:
    GKR_INNER_API bool is_ipv4() const;
    GKR_INNER_API bool is_ipv6() const;

public:
    GKR_INNER_API int family() const;
    GKR_INNER_API int type() const;
    GKR_INNER_API int protocol() const;

public:
    GKR_INNER_API std::size_t get_send_buffer_size() const;
    GKR_INNER_API bool        set_send_buffer_size(std::size_t size);

    GKR_INNER_API std::size_t get_receive_buffer_size() const;
    GKR_INNER_API bool        set_receive_buffer_size(std::size_t size);

public:
    GKR_INNER_API bool set_send_timeout(unsigned timeout);
    GKR_INNER_API bool set_receive_timeout(unsigned timeout);

public:
    GKR_INNER_API bool bind(unsigned short port);

    GKR_INNER_API bool listen();

public:
    GKR_INNER_API bool accept(address& addr, socket& conn, unsigned timeout = 0, unsigned* errors = nullptr);

public:

public:
    GKR_INNER_API std::size_t send_to(const address& addr, const void* buff, std::size_t size, unsigned* errors = nullptr);

    GKR_INNER_API std::size_t receive_from(address& addr, void* buff, std::size_t size, unsigned* errors = nullptr);

    GKR_INNER_API std::size_t write(const void* buff, std::size_t size, unsigned* errors = nullptr);

    GKR_INNER_API std::size_t read(void* buff, std::size_t size, unsigned* errors = nullptr);

private:
#ifdef _WIN32
    using socket_t = std::size_t;
#else
    using socket_t = int;
#endif
    static constexpr socket_t INVALID_SOCKET_VALUE = socket_t(-1);

    socket_t m_socket = INVALID_SOCKET_VALUE;
};

}
}
