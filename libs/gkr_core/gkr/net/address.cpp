#include <gkr/defs.hpp>
#include <gkr/net/address.hpp>

#include <gkr/diagnostics.hpp>

#ifdef _WIN32
#pragma warning(disable:4668)
#include <WinSock2.h>
#include <WS2tcpip.h>
#pragma warning(default:4668)
using sockaddr_inet = SOCKADDR_INET;
#define s6_addr16 s6_words
#else
#include <arpa/inet.h>
#include <netinet/in.h>
typedef union
{
    sockaddr_in    Ipv4;
    sockaddr_in6   Ipv6;
    unsigned short si_family;
} sockaddr_inet;
#endif

#include <cstring>
#include <cstdio>

namespace gkr
{
namespace net
{

int address::compare(const address& other) const noexcept
{
    static_assert(sizeof(m_addr) >= sizeof(sockaddr_inet), "The size of host address buffer must be larger");

    const sockaddr_inet&  thisSockAddr = *reinterpret_cast<const sockaddr_inet*>(&      m_addr);
    const sockaddr_inet& otherSockAddr = *reinterpret_cast<const sockaddr_inet*>(&other.m_addr);

    std::size_t sizeToCompare = sizeof(addr_t);

    switch(thisSockAddr.si_family)
    {
        case AF_INET : if(otherSockAddr.si_family == AF_INET ) sizeToCompare = sizeof(sockaddr_in ); break;
        case AF_INET6: if(otherSockAddr.si_family == AF_INET6) sizeToCompare = sizeof(sockaddr_in6); break;
    }
    return std::memcmp(&thisSockAddr, &otherSockAddr, sizeToCompare);
}

bool address::has_same_host(const address& other) const noexcept
{
    const sockaddr_inet&  thisSockAddr = *reinterpret_cast<const sockaddr_inet*>(&      m_addr);
    const sockaddr_inet& otherSockAddr = *reinterpret_cast<const sockaddr_inet*>(&other.m_addr);

    switch(thisSockAddr.si_family)
    {
        case AF_INET :
            if(otherSockAddr.si_family == AF_INET)
            {
                return (thisSockAddr.Ipv4.sin_addr.s_addr == otherSockAddr.Ipv4.sin_addr.s_addr);
            }
            break;
        case AF_INET6:
            if(otherSockAddr.si_family == AF_INET6)
            {
                for(std::size_t index = 0; index < 8; ++index)
                {
                    if(thisSockAddr.Ipv6.sin6_addr.s6_addr16[index] != otherSockAddr.Ipv6.sin6_addr.s6_addr16[index])
                    {
                        return false;
                    }
                }
                return true;
            }
            break;
    }
    return false;
}

bool address::reset(const char* host, unsigned short port)
{
    Check_Arg_NotNull(host, false);

    reset();

    return change_host(host) && change_port(port);
}

bool address::reset(bool ipv6, unsigned short port)
{
    reset();

    sockaddr_inet& sockAddr = *reinterpret_cast<sockaddr_inet*>(&m_addr);

    if(!ipv6)
    {
        sockAddr.Ipv4.sin_family      = AF_INET;
        sockAddr.Ipv4.sin_addr.s_addr = INADDR_ANY;
    }
    else
    {
        sockAddr.Ipv6.sin6_family = AF_INET6;
        sockAddr.Ipv6.sin6_addr   = IN6ADDR_ANY_INIT;
    }
    return change_port(port);
}

bool address::change_port(unsigned short port)
{
    sockaddr_inet& sockAddr = *reinterpret_cast<sockaddr_inet*>(&m_addr);

    switch(sockAddr.si_family)
    {
        case AF_INET : sockAddr.Ipv4.sin_port  = htons(port); break;
        case AF_INET6: sockAddr.Ipv6.sin6_port = htons(port); break;
        default: return false;
    }
    return true;
}
bool address::change_host(const char* host)
{
    Check_Arg_NotNull(host, false);

    sockaddr_inet& sockAddr = *reinterpret_cast<sockaddr_inet*>(&m_addr);

    if(1 == inet_pton(AF_INET, host, &sockAddr.Ipv4.sin_addr))
    {
        sockAddr.Ipv4.sin_family = AF_INET;
        return true;
    }
    if(1 == inet_pton(AF_INET6, host, &sockAddr.Ipv6.sin6_addr))
    {
        sockAddr.Ipv6.sin6_family = AF_INET6;
        return true;
    }
    sockAddr.si_family = AF_UNSPEC;
    return false;
}

bool address::is_ipv4() const noexcept
{
    const sockaddr_inet& sockAddr = *reinterpret_cast<const sockaddr_inet*>(&m_addr);

    return (sockAddr.si_family == AF_INET);
}

bool address::is_ipv6() const noexcept
{
    const sockaddr_inet& sockAddr = *reinterpret_cast<const sockaddr_inet*>(&m_addr);

    return (sockAddr.si_family == AF_INET6);
}

std::size_t address::size() const noexcept
{
    const sockaddr_inet& sockAddr = *reinterpret_cast<const sockaddr_inet*>(&m_addr);

    switch(sockAddr.si_family)
    {
        case AF_INET : return sizeof(sockaddr_in );
        case AF_INET6: return sizeof(sockaddr_in6);

        default: return 0;
    }
}

unsigned short address::port() const noexcept
{
    const sockaddr_inet& sockAddr = *reinterpret_cast<const sockaddr_inet*>(&m_addr);

    switch(sockAddr.si_family)
    {
        case AF_INET : return ntohs(sockAddr.Ipv4.sin_port );
        case AF_INET6: return ntohs(sockAddr.Ipv6.sin6_port);

        default: return 0;
    }
}

std::size_t address::host(char* buff, std::size_t cch) const
{
    constexpr std::size_t HOST_CCH = 64;
    char host[HOST_CCH];

    const sockaddr_inet& sockAddr = *reinterpret_cast<const sockaddr_inet*>(&m_addr);

    switch(sockAddr.si_family)
    {
        case AF_INET:
            if(nullptr == inet_ntop(AF_INET, &sockAddr.Ipv4.sin_addr, host, HOST_CCH))
            {
                return 0;
            }
            break;

        case AF_INET6:
            if(nullptr == inet_ntop(AF_INET6, &sockAddr.Ipv6.sin6_addr, host, HOST_CCH))
            {
                return 0;
            }
            break;

        default:
            return 0;
    }
    const std::size_t len = std::strlen(host);

    if(len >= cch) return (len + 1);

    Check_Arg_NotNull(buff, 0);

    std::strncpy(buff, host, cch);

    return len;
}

std::size_t address::addr(char* buff, std::size_t cch) const
{
    constexpr std::size_t ADDR_CCH = 72;
    char addr[ADDR_CCH];

    std::size_t len = 0;

    const sockaddr_inet& sockAddr = *reinterpret_cast<const sockaddr_inet*>(&m_addr);

    switch(sockAddr.si_family)
    {
        case AF_INET:
            if(nullptr != inet_ntop(AF_INET, &sockAddr.Ipv4.sin_addr, addr, ADDR_CCH))
            {
                const unsigned short port = ntohs(sockAddr.Ipv4.sin_port);

                len = std::strlen(addr);
                Assert_Check(len < ADDR_CCH);

                len += std::size_t(std::snprintf(addr + len, ADDR_CCH - len, ":%hu", port));
            }
            break;

        case AF_INET6:
            if(nullptr != inet_ntop(AF_INET6, &sockAddr.Ipv6.sin6_addr, addr + 1, ADDR_CCH - 1))
            {
                const unsigned short port = ntohs(sockAddr.Ipv6.sin6_port);

                *addr = '[';

                len = std::strlen(addr);
                Assert_Check(len < ADDR_CCH);

                len += std::size_t(std::snprintf(addr + len, ADDR_CCH - len, "]:%hu", port));
            }
            break;
    }
    if(len == 0)
    {
        if((cch > 0) && (buff != nullptr)) *buff = 0;
        return 0;
    }

    if(len >= cch) return (len + 1);

    Check_Arg_NotNull(buff, 0);

    std::strncpy(buff, addr, cch);

    return len;
}

}
}
