#include <gkr/defs.hpp>
#include <gkr/comm/udp_socket_receiver.hpp>

#include <gkr/data/split_packet.hpp>

namespace gkr
{
namespace comm
{

udp_socket_receiver::udp_socket_receiver(
    std::size_t maxPacketSize,
    bool useIPv6
    )
{
    m_packet.reserve(maxPacketSize);

    m_socket.open_as_udp(useIPv6);
}

udp_socket_receiver::~udp_socket_receiver()
{
    m_socket.close();
}

bool udp_socket_receiver::setWaitPacketTimeout(unsigned waitPacketTimeout)
{
    return m_socket.set_receive_timeout(waitPacketTimeout);
}

bool udp_socket_receiver::startReceivingPackets(unsigned short port)
{
    return m_socket.bind(port);
}

void udp_socket_receiver::stopReceivingPackets()
{
    m_socket.reopen();
}

bool udp_socket_receiver::receivePacket()
{
    m_offset = 0;
    m_packet.change_size(0);

    unsigned errors = net::socket::error_timeout;

    const std::size_t received = m_socket.receive_from(m_packet.data(), m_packet.capacity(), m_addr, &errors);

    if(received == 0) return (errors != 0);

    Assert_Check(received <= m_packet.capacity());

    m_packet.change_size(received);

    const data::split_packet_head& packetHead = m_packet.as<const data::split_packet_head>();

    if(received != packetHead.packet_size)
    {
        //TODO:replace with log
        Check_Recovery("Invalid packet header");
    }
    else if(packetHead.data_offset < sizeof(data::split_packet_head))
    {
        //TODO:replace with log
        Check_Recovery("Invalid packet header");
    }
    else if(packetHead.packet_index >= packetHead.packet_count)
    {
        //TODO:replace with log
        Check_Recovery("Invalid packet header");
    }
    else if((packetHead.packet_index == 0) && (packetHead.packet_count == 1))
    {
        handleUnsplittedPacket();
    }
    else
    {
        handlePartialPacket();
    }
    return true;
}

bool udp_socket_receiver::getReadyPacketData(net::address& addr, const void*& data, std::size_t& size)
{
    if(m_offset > 0)
    {
        Check_ValidState(m_offset < m_packet.size(), false);

        addr = m_addr;
        data = m_packet.data(m_offset);
        size = m_packet.size() - m_offset;

        m_addr  .reset();
        m_packet.change_size(0);
        m_offset = 0;
        return true;
    }
    if(m_buffer.size() > 0)
    {
        addr = m_addr;
        data = m_buffer.data(m_offset);
        size = m_buffer.size() - m_offset;

        //todo
        m_buffer.change_size(0);
        return true;
    }
    return false;
}

void udp_socket_receiver::handleUnsplittedPacket()
{
    const data::split_packet_head&      packetHead = m_packet.as<const data::split_packet_head>();
    const data::split_packet_data_head& packetData = m_packet.as<const data::split_packet_data_head>(packetHead.data_offset);

    const std::size_t data_size = std::size_t(packetHead.packet_size) - std::size_t(packetHead.data_offset);

    if((data_size != packetHead.data_size) || (0 != packetHead.data_sent))
    {
        //TODO:replace with log
        Check_Recovery("Invalid packet header");
    }
    else if(data_size != packetData.size)
    {
        //TODO:replace with log
        Check_Recovery("Invalid packet header");
    }
    else
    {
        m_offset = packetHead.data_offset;
    }
}

void udp_socket_receiver::handlePartialPacket()
{
    const data::split_packet_head& packetHead = m_packet.as<const data::split_packet_head>();

    std::size_t partialDataSize;
    partial_packet_t& partialPacket = findPartialPacket(packetHead, partialDataSize);

    if(partialPacket.count == 0) return;

    partialPacket.updated = std::chrono::steady_clock::now();

    std::memcpy(partialPacket.buffer.data(packetHead.data_sent), m_packet.data(packetHead.data_offset), partialDataSize);

    if(++partialPacket.received == partialPacket.count)
    {
        std::swap(m_buffer, partialPacket.buffer);
        partialPacket.reset();
    }
}

udp_socket_receiver::partial_packet_t& udp_socket_receiver::findPartialPacket(const data::split_packet_head& packetHead, std::size_t& partialDataSize)
{
    partialDataSize = std::size_t(packetHead.packet_size) - std::size_t(packetHead.data_offset);
    Assert_Check(packetHead.packet_count > 0);

    partial_packet_t* unused = nullptr;

    for(partial_packet_t& partialPacket : m_partialPackets)
    {
        if(partialPacket.count == 0)
        {
            unused = &partialPacket;
        }
        else if((packetHead.packet_id == partialPacket.id) && (m_addr == partialPacket.sender))
        {
            if(partialPacket.count != packetHead.packet_count)
            {
                //TODO:replace with log
                Check_Recovery("Invalid packet header");
                partialPacket.reset();
            }
            else if(partialPacket.buffer.size() != packetHead.data_size)
            {
                //TODO:replace with log
                Check_Recovery("Invalid packet header");
                partialPacket.reset();
            }
            else if(partialPacket.buffer.size() < (packetHead.data_sent + partialDataSize))
            {
                //TODO:replace with log
                Check_Recovery("Invalid packet header");
                partialPacket.reset();
            }
            return partialPacket;
        }
    }
    partial_packet_t& partialPacket = (unused != nullptr)
        ? *unused
        : ((void)m_partialPackets.emplace_back(), m_partialPackets.back())
        ;
    partialPacket.count    = packetHead.packet_count;
    partialPacket.received = 0;
    partialPacket.id       = packetHead.packet_id;
    partialPacket.sender   = m_addr;

    partialPacket.buffer.resize(packetHead.data_size);
    return partialPacket;
}

}
}
