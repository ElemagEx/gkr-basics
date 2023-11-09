#include "udpSocketConsumer.h"

#include <gkr/log/message.h>

#include <cstring>

#ifdef _WIN32
#pragma warning(disable:4668)
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma warning(default:4668)
#else
#endif

struct udp_log_message : gkr::log::message_head
{
};

namespace gkr
{
namespace log
{

udpSocketConsumer::udpSocketConsumer(udpSocketConsumer&& other) noexcept
    : m_processName(std::move(other.m_processName))
    , m_hostName   (std::move(other.m_hostName))
    , m_remoteAddr (other.m_remoteAddr)
{
    other.clearRemoteHostAddress();
}

udpSocketConsumer& udpSocketConsumer::operator=(udpSocketConsumer&& other) noexcept
{
    m_processName = std::move(other.m_processName);
    m_hostName    = std::move(other.m_hostName);

    m_remoteAddr = other.m_remoteAddr;

    other.clearRemoteHostAddress();

    return *this;
}

udpSocketConsumer::udpSocketConsumer(const char* addr, unsigned short port) : m_remoteAddr()
{
    

    setRemoteAddress(addr, port);
}

udpSocketConsumer::~udpSocketConsumer()
{
}

bool udpSocketConsumer::init_logging()
{
    SOCKADDR_INET& addr = *reinterpret_cast<SOCKADDR_INET*>(&m_remoteAddr);

    return (addr.si_family != AF_UNSPEC);
}

void udpSocketConsumer::done_logging()
{
}

bool udpSocketConsumer::filter_log_message(const message& msg)
{
    return false;
}

void udpSocketConsumer::consume_log_message(const message& msg)
{
}

bool udpSocketConsumer::setRemoteAddress(const char* addr, unsigned short port)
{
    static_assert(sizeof(m_remoteAddr) >= sizeof(SOCKADDR_INET), "The size of host address buffer must be larger");

    clearRemoteHostAddress();

    SOCKADDR_INET& sockAddr = *reinterpret_cast<SOCKADDR_INET*>(&m_remoteAddr);

    if(1 == inet_pton(AF_INET, addr, &sockAddr.Ipv4.sin_addr))
    {
        sockAddr.Ipv4.sin_family = AF_INET;
        sockAddr.Ipv4.sin_port   = htons(port);
        return true;
    }
    if(1 == inet_pton(AF_INET6, addr, &sockAddr.Ipv6.sin6_addr))
    {
        sockAddr.Ipv6.sin6_family = AF_INET6;
        sockAddr.Ipv6.sin6_port   = htons(port);
        return true;
    }
    return false;
}

void udpSocketConsumer::clearRemoteHostAddress() noexcept
{
    for(auto& byte : m_remoteAddr.bytes) byte = 0;
}

}
}
