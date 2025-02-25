#include <gkr/defs.hpp>
#include <gkr/comm/udp_socket_sender.hpp>

#include <gkr/data/split_packet.hpp>
#include <gkr/sys/process.hpp>

namespace gkr
{
namespace comm
{

udp_socket_sender::udp_socket_sender(std::size_t maxPacketSize)
{
    m_buffer.resize(maxPacketSize);

    m_packetId = packet_id_t(sys::get_current_process_id()) << 32;
}

udp_socket_sender::~udp_socket_sender()
{
    m_socket.close();
}

bool udp_socket_sender::start_sending_packets()
{
    if(!m_address.is_valid()) return false;

    return m_socket.open_as_udp(m_address.is_ipv6());
}

void udp_socket_sender::stop_sending_packets()
{
    m_socket.close();
}

bool udp_socket_sender::send_data(const char* data, std::size_t size)
{
    constexpr std::size_t DATA_OFFSET = sizeof(data::split_packet_head);

    const std::size_t maxPacketSize = m_buffer.size();
    Assert_Check(maxPacketSize > DATA_OFFSET);

    const std::size_t maxDataSizePerPacket = (maxPacketSize - DATA_OFFSET);

    const std::size_t count = ((size % maxDataSizePerPacket) == 0)
        ? ((size / maxDataSizePerPacket) + 0)
        : ((size / maxDataSizePerPacket) + 1)
        ;
    Assert_Check(count > 0);
    Assert_Check(count < 65536);

    data::split_packet_head& packetHead = m_buffer.as<data::split_packet_head>();

    packetHead.packet_id    = m_packetId++;
    packetHead.packet_count = std::uint16_t(count);
//  packetHead.packet_index = 0;
//  packetHead.packet_size  = 0;
    packetHead.data_offset  = DATA_OFFSET;
    packetHead.data_size    = std::uint32_t(size);
//  packetHead.data_sent    = 0;

    void* packetBody = m_buffer.data(DATA_OFFSET);

    std::size_t restDataSize = size;
    std::size_t sentDataSize = 0;

    for(std::size_t index = 0; index < count; ++index)
    {
        Assert_Check(restDataSize > 0);
        Assert_Check(sentDataSize < size);

        const std::size_t currDataSize = (restDataSize >= maxDataSizePerPacket)
            ?  maxDataSizePerPacket
            : restDataSize
            ;
        const std::size_t packetSize = currDataSize + DATA_OFFSET;

        packetHead.packet_index = std::uint16_t(index);
        packetHead.packet_size  = std::uint16_t(packetSize);
        packetHead.data_sent    = std::uint32_t(sentDataSize);

        std::memcpy(packetBody, data + sentDataSize, currDataSize);

        if(m_socket.send_to(m_address, &packetHead, packetSize) != packetSize)
        {
            return false;
        }
        restDataSize -= currDataSize;
        sentDataSize += currDataSize;
    }
    Assert_Check(restDataSize == 0);
    Assert_Check(sentDataSize == size);

    return true;
}

}
}
