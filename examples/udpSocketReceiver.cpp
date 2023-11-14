#include "udpSocketReceiver.h"

#include <gkr/net/address.h>

namespace gkr
{

udpSocketReceiver::udpSocketReceiver(
    std::size_t maxPacketSize,
    std::size_t bufferInitialCapacity,
    bool useIPv6
    )
{
    m_packet.resize(maxPacketSize);
    m_buffer.reserve(bufferInitialCapacity);

    m_socket.open_as_udp(useIPv6);
}

udpSocketReceiver::~udpSocketReceiver()
{
    m_socket.close();
}

bool udpSocketReceiver::startReceivingPackets(unsigned short port, unsigned waitPacketTime)
{
    return m_socket.set_receive_timeout(waitPacketTime) && m_socket.bind(port);
}

void udpSocketReceiver::stopReceivingPackets()
{
    m_socket.reopen();
}

}
