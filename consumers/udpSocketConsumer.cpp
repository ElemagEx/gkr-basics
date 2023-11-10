#include "udpSocketConsumer.h"

#include <gkr/log/message.h>
#include <gkr/sys/process_name.h>

#include <gkr/diag/diagnostics.h>

#include <cstring>

#ifdef _WIN32
#pragma warning(disable:4668)
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma warning(default:4668)
using sockaddr_inet = SOCKADDR_INET;
#else
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
typedef union
{
    sockaddr_in    Ipv4;
    sockaddr_in6   Ipv6;
    unsigned short si_family;
} sockaddr_inet;
#endif

namespace gkr
{
namespace log
{

struct message_packet_desc
{
    std::uint16_t packet_size;
    std::uint8_t  packet_count;
    std::uint8_t  packet_index;
};
struct message_packet_head : message_head
{
    std::int32_t  pid;
    std::uint16_t data_size;
    std::uint16_t _unused;
    std::uint16_t offset_to_host;
    std::uint16_t offset_to_process;
    std::uint16_t offset_to_thread;
    std::uint16_t offset_to_facility;
    std::uint16_t offset_to_severity;
    std::uint16_t offset_to_text;
};

udpSocketConsumer::udpSocketConsumer(udpSocketConsumer&& other) noexcept
    : m_processName(std::move(other.m_processName))
    , m_hostName   (std::move(other.m_hostName))
    , m_remoteAddr (other.m_remoteAddr)
    , m_pid        (other.m_pid)
{
    other.clearRemoteAddress();
}

udpSocketConsumer& udpSocketConsumer::operator=(udpSocketConsumer&& other) noexcept
{
    m_processName = std::move(other.m_processName);
    m_hostName    = std::move(other.m_hostName);

    m_remoteAddr = other.m_remoteAddr;
    m_pid        = other.m_pid;

    other.clearRemoteAddress();

    return *this;
}

udpSocketConsumer::udpSocketConsumer(const char* addr, unsigned short port, unsigned maxPacketSize, unsigned bufferSize)
    : m_remoteAddr()
//  , m_bufferSize(bufferSize)
    , m_maxPacketSize(maxPacketSize)
{
    setRemoteAddress(addr, port);
}

udpSocketConsumer::~udpSocketConsumer()
{
}

bool udpSocketConsumer::init_logging()
{
    sockaddr_inet& addr = *reinterpret_cast<sockaddr_inet*>(&m_remoteAddr);

    if(addr.si_family == AF_UNSPEC)
    {
        Check_FailureMsg("Remote address is invalid", false);
    }

    m_pid = gkr::sys::get_current_process_id();

    if(!retrieveProcessName() || !retrieveHostName()) return false;

    return true;
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
    static_assert(sizeof(m_remoteAddr) >= sizeof(sockaddr_inet), "The size of host address buffer must be larger");

    clearRemoteAddress();

    sockaddr_inet& sockAddr = *reinterpret_cast<sockaddr_inet*>(&m_remoteAddr);

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
    Check_Arg_Invalid(addr, false);
}

void udpSocketConsumer::clearRemoteAddress() noexcept
{
    for(auto& byte : m_remoteAddr.bytes) byte = 0;
}

bool udpSocketConsumer::retrieveProcessName()
{
    char name[256];

    const int len = gkr::sys::get_current_process_name(name, 256);

    Check_ValidState(len > 0, false);

    m_processName.assign(name, unsigned(len));

    return true;
}

bool udpSocketConsumer::retrieveHostName()
{
    char name[256];

    [[maybe_unused]] const int res = gethostname(name, 256);

    Check_ValidState(res == 0, false);

    m_hostName = name;

    return true;
}

}
}
