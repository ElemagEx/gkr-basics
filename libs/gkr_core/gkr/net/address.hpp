#pragma once

#include <gkr/api.hpp>

#include <cstddef>

namespace gkr
{
namespace net
{

class address
{
    struct addr_t { alignas(4) unsigned char bytes[32] = {0}; };

    addr_t m_addr {};

public:
    address() noexcept = default;
   ~address() noexcept = default;

    address(address&& other) noexcept : m_addr(other.m_addr)
    {
        other.reset();
    }
    address& operator=(address&& other) noexcept
    {
        m_addr = other.m_addr;
        other.reset();
        return *this;
    }

    address(const address& other) noexcept : m_addr(other.m_addr)
    {
    }
    address& operator=(const address& other) noexcept
    {
        m_addr = other.m_addr;
        return *this;
    }

    address(const char* host, unsigned short port)
    {
        reset(host, port);
    }
    address(bool ipv6, unsigned short port)
    {
        reset(ipv6, port);
    }

public:
    bool operator==(const address& other) const noexcept
    {
        return (0 == compare(other));
    }
    bool operator!=(const address& other) const noexcept
    {
        return (0 != compare(other));
    }

    bool operator<(const address& other) const noexcept
    {
        return (0 > compare(other));
    }

public:
    void reset() noexcept
    {
        m_addr = addr_t();
    }

public:
    int family() const noexcept
    {
        return *reinterpret_cast<const unsigned short*>(&m_addr);
    }
    bool is_valid() const noexcept
    {
        return (is_ipv4() || is_ipv6());
    }
    const void* data() const noexcept
    {
        return &m_addr;
    }
    void* data() noexcept
    {
        return &m_addr;
    }
    constexpr std::size_t capacity() const noexcept
    {
        return sizeof(m_addr);
    }

public:
    GKR_INNER_API int compare(const address& other) const noexcept;

    GKR_INNER_API bool has_same_host(const address& other) const noexcept;

public:
    GKR_INNER_API bool reset(const char* host, unsigned short port);
    GKR_INNER_API bool reset(bool ipv6, unsigned short port);

    GKR_INNER_API bool change_port(unsigned short port);
    GKR_INNER_API bool change_host(const char* host);

public:
    GKR_INNER_API bool is_ipv4() const noexcept;
    GKR_INNER_API bool is_ipv6() const noexcept;

    GKR_INNER_API std::size_t size() const noexcept;

    GKR_INNER_API unsigned short port() const noexcept;

    GKR_INNER_API std::size_t host(char* buff, std::size_t cch) const;
    GKR_INNER_API std::size_t addr(char* buff, std::size_t cch) const;

public:
    template<std::size_t CCH>
    size_t host(char (&buff)[CCH]) const
    {
        return host(buff, CCH);
    }
    template<std::size_t CCH>
    size_t addr(char (&buff)[CCH]) const
    {
        return addr(buff, CCH);
    }
};

}
}
