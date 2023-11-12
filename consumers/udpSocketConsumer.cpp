#include "udpSocketConsumer.h"
#include "udpMessagePacket.h"

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
inline int closesocket(int fd)
{
	return close(fd);
}
#endif

namespace gkr
{
namespace log
{

udpSocketConsumer::udpSocketConsumer(
	const char* addr,
	unsigned short port,
	std::size_t maxPacketSize,
	std::size_t bufferInitialCapacity
	)
    : m_packet(maxPacketSize)
    , m_buffer(bufferInitialCapacity)
    , m_processId(gkr::sys::get_current_process_id())
{
    Check_Arg_IsValid(maxPacketSize >= MINIMUM_UDP_PACKET_SIZE, );

    m_packet.resize(maxPacketSize);

    setRemoteAddress(addr, port);
}

udpSocketConsumer::~udpSocketConsumer()
{
    Assert_Check(m_socket == INVALID_SOCKET_VALUE);
}

bool udpSocketConsumer::init_logging()
{
    Check_ValidState(m_packet.capacity() >= MINIMUM_UDP_PACKET_SIZE, false);

    if(!retrieveProcessName()) return false;
    if(!retrieveHostName()) return false;

    return openUdpSocket();
}

void udpSocketConsumer::done_logging()
{
    closeUdpSocket();
}

bool udpSocketConsumer::filter_log_message(const message& msg)
{
    return false;
}

void udpSocketConsumer::consume_log_message(const message& msg)
{
    constructData(msg);
    sendData();
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

    m_hostName.assign(name);

    return true;
}

void udpSocketConsumer::constructData(const message& msg)
{
    const std::size_t nameLenThread   = std::strlen(msg.threadName);
    const std::size_t nameLenFacility = std::strlen(msg.facilityName);
    const std::size_t nameLenSeverity = std::strlen(msg.severityName);

    const std::size_t dataSize =
        sizeof(message_data)
        + m_hostName   .size() + 1
        + m_processName.size() + 1
        + nameLenThread   + 1
        + nameLenFacility + 1
        + nameLenSeverity + 1
        + msg.messageLen  + 1
        ;
    m_buffer.resize(dataSize);

    message_data& messageData = m_buffer.as<message_data>();

    messageData.head.signature = SIGNITURE_LOG_MSG;
    messageData.head.size      = std::uint32_t(dataSize);

    messageData.info = static_cast<const message_info&>(msg);

    messageData.desc.pid = std::uint32_t(m_processId);

    std::size_t offsetToStr = sizeof(message_data);

    messageData.desc.offset_to_host     = std::uint16_t(offsetToStr); offsetToStr += m_hostName   .size() + 1;
    messageData.desc.offset_to_process  = std::uint16_t(offsetToStr); offsetToStr += m_processName.size() + 1;
    messageData.desc.offset_to_thread   = std::uint16_t(offsetToStr); offsetToStr += nameLenThread   + 1;
    messageData.desc.offset_to_facility = std::uint16_t(offsetToStr); offsetToStr += nameLenFacility + 1;
    messageData.desc.offset_to_severity = std::uint16_t(offsetToStr); offsetToStr += nameLenSeverity + 1;
    messageData.desc.offset_to_text     = std::uint16_t(offsetToStr); offsetToStr += msg.messageLen  + 1;

    Assert_Check(offsetToStr == dataSize);

    char* strBase = reinterpret_cast<char*>(&messageData);

    std::strncpy(strBase + messageData.desc.offset_to_host    , m_hostName   .c_str(), m_hostName   .size() + 1); 
    std::strncpy(strBase + messageData.desc.offset_to_process , m_processName.c_str(), m_processName.size() + 1);
    std::strncpy(strBase + messageData.desc.offset_to_thread  , msg.threadName       , nameLenThread   + 1);
    std::strncpy(strBase + messageData.desc.offset_to_facility, msg.facilityName     , nameLenFacility + 1);
    std::strncpy(strBase + messageData.desc.offset_to_severity, msg.severityName     , nameLenSeverity + 1);
    std::strncpy(strBase + messageData.desc.offset_to_text    , msg.messageText      , msg.messageLen  + 1);
}

void udpSocketConsumer::sendData()
{
    constexpr std::size_t DATA_OFFSET = sizeof(net::split_packet_head);

    const std::size_t maxPacketSize = m_packet.size();
    Assert_Check(maxPacketSize > DATA_OFFSET);

    const std::size_t allDataSize          = m_buffer.as<message_data>(DATA_OFFSET).head.size;
    const std::size_t maxDataSizePerPacket = (maxPacketSize - DATA_OFFSET);

    const std::size_t count       = ((allDataSize % maxDataSizePerPacket) == 0)
        ? ((allDataSize / maxDataSizePerPacket) + 0)
        : ((allDataSize / maxDataSizePerPacket) + 1)
        ;
    Assert_Check(count > 0);
    Assert_Check(count < 65536);

    net::split_packet_head& packetHead = m_packet.as<net::split_packet_head>();

    packetHead.packet_id    = m_packetId++;
    packetHead.packet_count = std::uint16_t(count);
//  packetHead.packet_index = 0;
//  packetHead.packet_size  = 0;
    packetHead.data_offset  = DATA_OFFSET;
//  packetHead.data_size    = 0;
//  packetHead.data_offset  = 0;

    const
    char* data = m_buffer.data<char>();
    char* buff = m_packet.data<char>(DATA_OFFSET);

    std::size_t restDataSize = allDataSize;
    std::size_t sentDataSize = 0;

    for(std::size_t index = 0; index < count; ++index)
    {
        Assert_Check(restDataSize > 0);
        Assert_Check(sentDataSize < allDataSize);

        const std::size_t curDataSize = (restDataSize >= maxDataSizePerPacket)
            ?  maxDataSizePerPacket
            : restDataSize
            ;
        const std::size_t packetSize = curDataSize + DATA_OFFSET;

        packetHead.packet_index = std::uint16_t(index);
        packetHead.packet_size  = std::uint16_t(packetSize);
        packetHead.data_size    = std::uint32_t( allDataSize);
        packetHead.data_sent    = std::uint32_t(sentDataSize);

        std::memcpy(buff, data + sentDataSize, curDataSize);

        sendUdpPacket(&packetHead, packetSize);

        restDataSize -= curDataSize;
        sentDataSize += curDataSize;
    }
    Assert_Check(restDataSize == 0);
    Assert_Check(sentDataSize == allDataSize);
}

bool udpSocketConsumer::openUdpSocket()
{
    Check_ValidState(m_socket == INVALID_SOCKET_VALUE, false);

    sockaddr_inet& sockAddr = *reinterpret_cast<sockaddr_inet*>(&m_remoteAddr);

    Check_ValidState(sockAddr.si_family != AF_UNSPEC);

    m_socket = socket(sockAddr.si_family, SOCK_DGRAM, IPPROTO_UDP);

    return (m_socket != INVALID_SOCKET_VALUE);
}

void udpSocketConsumer::closeUdpSocket()
{
    if(m_socket == INVALID_SOCKET_VALUE) return;

    closesocket(m_socket);

    m_socket = INVALID_SOCKET_VALUE;
}

void udpSocketConsumer::sendUdpPacket(const void* packet, std::size_t size)
{
    const sockaddr_inet& sockAddr = *reinterpret_cast<sockaddr_inet*>(&m_remoteAddr);

    socklen_t tolen;
    switch(sockAddr.si_family)
    {
        case AF_INET : tolen = sizeof(sockaddr_in ); break;
        case AF_INET6: tolen = sizeof(sockaddr_in6); break;

        default: Check_Failure();
    }

    sendto(m_socket, static_cast<const char*>(packet), socklen_t(size), 0, reinterpret_cast<const sockaddr*>(&sockAddr), tolen);
}

}
}
