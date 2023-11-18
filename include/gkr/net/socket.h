#pragma once

#include <cstddef>

#ifndef GKR_NET_API
#define GKR_NET_API
#endif

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
        error_none    = 0,
        error_timeout = (1U <<  0),
        error_other   = (1U << 31)
    };

public:
    GKR_NET_API bool open_as_udp(bool ipv6 = false);
    GKR_NET_API bool reopen();
    GKR_NET_API void close();

public:
    GKR_NET_API bool is_ipv4() const;
    GKR_NET_API bool is_ipv6() const;

public:
    GKR_NET_API int family() const;
    GKR_NET_API int type() const;
    GKR_NET_API int protocol() const;

public:
    GKR_NET_API std::size_t get_send_buffer_size() const;
    GKR_NET_API bool        set_send_buffer_size(std::size_t size);

    GKR_NET_API std::size_t get_receive_buffer_size() const;
    GKR_NET_API bool        set_receive_buffer_size(std::size_t size);

public:
    GKR_NET_API bool set_send_timeout(unsigned timeout);
    GKR_NET_API bool set_receive_timeout(unsigned timeout);

public:
    GKR_NET_API bool bind(unsigned short port);

public:
    GKR_NET_API std::size_t send_to(const void* buff, std::size_t size, const address& addr);

    GKR_NET_API std::size_t receive_from(void* buff, std::size_t size, address& addr, unsigned* errors = nullptr);

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
