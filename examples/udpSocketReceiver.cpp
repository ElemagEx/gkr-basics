#include "udpSocketReceiver.h"

#include "udpMessagePacket.h"

namespace gkr
{

udpSocketReceiver::udpSocketReceiver(
    std::size_t maxPacketSize,
    bool useIPv6
    )
{
    m_packet.reserve(maxPacketSize);

    m_socket.open_as_udp(useIPv6);
}

udpSocketReceiver::~udpSocketReceiver()
{
    m_socket.close();
}

bool udpSocketReceiver::setWaitPacketTimeout(unsigned waitPacketTimeout)
{
    return m_socket.set_receive_timeout(waitPacketTimeout);
}

bool udpSocketReceiver::startReceivingPackets(unsigned short port)
{
    return m_socket.bind(port);
}

void udpSocketReceiver::stopReceivingPackets()
{
    m_socket.reopen();
}

bool udpSocketReceiver::receivePacket()
{
    m_offset = 0;
    m_packet.set_size(0);

    unsigned errors = net::socket::error_timeout;

    const std::size_t received = m_socket.receive_from(m_packet.data(), m_packet.capacity(), m_addr, &errors);

    if(received == 0) return false;

    Assert_Check(received <= m_packet.capacity());

    const net::split_packet_head& packet_head = m_packet.as<const net::split_packet_head>();

    if(received != packet_head.packet_size)
    {
        //LOG
        return false;
    }
    if(packet_head.data_offset < sizeof(net::split_packet_head))
    {
        //LOG
        return false;
    }

    if((packet_head.packet_count == 1) && (packet_head.packet_index == 0))
    {
        m_offset = packet_head.data_offset;
        m_packet.set_size(received);

        return handleUnsplittedPacket();
    }
    else
    {
        //LOG
        return false;
    }
}

bool udpSocketReceiver::getPacketData(net::address& addr, const void*& data, std::size_t& size)
{
    Check_ValidState(m_offset > 0, false);
    Check_ValidState(m_offset < m_packet.size(), false);

    data = m_packet.data(m_offset);
    size = m_packet.size() - m_offset;

    addr = m_addr;
    return true;
}

bool udpSocketReceiver::handleUnsplittedPacket()
{
    const net::split_packet_head& packet_head = m_packet.as<const net::split_packet_head>();
    const net::packet_data_head&  packet_data = m_packet.as<const net::packet_data_head>(packet_head.data_offset);

    Assert_Check(m_offset == packet_head.data_offset);

    const std::size_t size = (packet_head.packet_size - packet_head.data_offset);

    if((size != packet_head.data_size) || (0 != packet_head.data_sent))
    {
        //LOG
        return false;
    }
    if(size != packet_data.size)
    {
        //LOG
        return false;
    }
    return true;
}

}
