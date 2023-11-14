#include "udpSocketReceiver.h"

#include <gkr/net/address.h>

#ifdef _WIN32
#pragma warning(disable:4668)
#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma warning(default:4668)
using sockaddr_inet = SOCKADDR_INET;
#else
#include <unistd.h>
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

udpSocketReceiver::udpSocketReceiver(
    unsigned short port,
    std::size_t maxPacketSize,
    std::size_t bufferInitialCapacity,
    bool useIPv6
    )
{
    m_packet.resize(maxPacketSize);
    m_buffer.reserve(bufferInitialCapacity);
}

udpSocketReceiver::~udpSocketReceiver() = default;

bool udpSocketReceiver::startReceivingPackets()
{
    return openUdpSocket();
}

void udpSocketReceiver::stopReceivingPackets()
{
    return closeUdpSocket();
}

bool udpSocketReceiver::openUdpSocket()
{
    Check_ValidState(m_socket == INVALID_SOCKET_VALUE, false);

    net::address addr(m_useIPv6, m_port);

    m_socket = socket(addr.family(), SOCK_DGRAM, IPPROTO_UDP);

    if(m_socket == INVALID_SOCKET_VALUE) return false;

    bind(m_socket, (sockaddr*)&addr, sizeof(addr)), NET_ERR_ADDRINUSE)
}

void udpSocketReceiver::closeUdpSocket()
{
    if(m_socket == INVALID_SOCKET_VALUE) return;

    closesocket(m_socket);

    m_socket = INVALID_SOCKET_VALUE;
}

}
