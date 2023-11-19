#include <gkr/log/consumers/udpSocketConsumer.h>

#include <gkr/data/log_message.h>

#include <gkr/log/message.h>
#include <gkr/sys/process_name.h>
#include <gkr/net/lib.h>

#include <gkr/diagnostics.h>

#include <cstring>

extern "C" {

void* udpSocketCreateConsumerParam(
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize,
    unsigned bufferCapacity
    )
{
    return new udpSocketConsumer(remoteHost, remotePort, maxPacketSize, bufferCapacity);
}

int udpSocketInitLogging(void* param)
{
    if(param == nullptr) return false;

    udpSocketConsumer* consumer = static_cast<udpSocketConsumer*>(param);

    return consumer->init_logging();
}

void udpSocketDoneLogging(void* param)
{
    if(param == nullptr) return;

    udpSocketConsumer* consumer = static_cast<udpSocketConsumer*>(param);

    consumer->done_logging();

    delete consumer;
}

int udpSocketFilterLogMessage(void* param, const struct gkr_log_message* msg)
{
    udpSocketConsumer* consumer = static_cast<udpSocketConsumer*>(param);

    return consumer->filter_log_message(*msg);
}

void udpSocketConsumeLogMessage(void* param, const struct gkr_log_message* msg)
{
    udpSocketConsumer* consumer = static_cast<udpSocketConsumer*>(param);

    consumer->consume_log_message(*msg);
}

}

static_assert(udpSocketConsumer::MINIMUM_UDP_PACKET_SIZE > sizeof(gkr::data::split_packet_head), "Minimum size is not enough to deliver data");

udpSocketConsumer::udpSocketConsumer(
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize,
    unsigned bufferCapacity
    )
    : m_packet(maxPacketSize)
    , m_buffer(bufferCapacity)
    , m_processId(unsigned(gkr::sys::get_current_process_id()))
{
    m_packetId = (std::uint64_t(m_processId) << 32);

    Check_Arg_IsValid(maxPacketSize >= MINIMUM_UDP_PACKET_SIZE, );

    m_packet.resize(maxPacketSize);

    setRemoteAddress(remoteHost, remotePort);
}

udpSocketConsumer::~udpSocketConsumer()
{
    Assert_Check(!m_socket.is_open());
}

bool udpSocketConsumer::init_logging()
{
    Check_ValidState(m_packet.capacity() >= MINIMUM_UDP_PACKET_SIZE, false);

    if(!retrieveProcessName()) return false;
    if(!retrieveHostName()) return false;

    Check_ValidState(!m_socket.is_open(), false);
    Check_ValidState(m_remoteAddr.is_valid(), false);

    return m_socket.open_as_udp(m_remoteAddr.is_ipv6());
}

void udpSocketConsumer::done_logging()
{
    m_socket.close();
}

bool udpSocketConsumer::filter_log_message(const gkr::log::message& msg)
{
    return false;
}

void udpSocketConsumer::consume_log_message(const gkr::log::message& msg)
{
    constructData(msg);

    const gkr::data::log_message& messageData = m_buffer.as<gkr::data::log_message>();

    postData(reinterpret_cast<const char*>(&messageData), messageData.size);
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

    const int len = gkr::net::get_hostname(name, 256);

    Check_ValidState(len > 0, false);

    m_hostName.assign(name, unsigned(len));

    return true;
}

void udpSocketConsumer::constructData(const gkr::log::message& msg)
{
    const std::size_t nameLenThread   = std::strlen(msg.threadName);
    const std::size_t nameLenFacility = std::strlen(msg.facilityName);
    const std::size_t nameLenSeverity = std::strlen(msg.severityName);

    const std::size_t dataSize =
        sizeof(gkr::data::log_message)
        + m_hostName   .size() + 1
        + m_processName.size() + 1
        + nameLenThread   + 1
        + nameLenFacility + 1
        + nameLenSeverity + 1
        + msg.messageLen  + 1
        ;
    m_buffer.resize(dataSize);

    gkr::data::log_message& messageData = m_buffer.as<gkr::data::log_message>();

    messageData.signature = gkr::data::SIGNITURE_LOG_MESSAGE;
    messageData.size      = std::uint32_t(dataSize);

    messageData.stamp    = msg.stamp;
    messageData.tid      = msg.tid;
    messageData.pid      = m_processId;
    messageData.severity = msg.severity;
    messageData.facility = msg.facility;

    std::size_t offsetToStr = sizeof(gkr::data::log_message);

    messageData.offset_to_host     = std::uint16_t(offsetToStr); offsetToStr += m_hostName   .size() + 1;
    messageData.offset_to_process  = std::uint16_t(offsetToStr); offsetToStr += m_processName.size() + 1;
    messageData.offset_to_thread   = std::uint16_t(offsetToStr); offsetToStr += nameLenThread   + 1;
    messageData.offset_to_facility = std::uint16_t(offsetToStr); offsetToStr += nameLenFacility + 1;
    messageData.offset_to_severity = std::uint16_t(offsetToStr); offsetToStr += nameLenSeverity + 1;
    messageData.offset_to_text     = std::uint16_t(offsetToStr); offsetToStr += msg.messageLen  + 1;

    Assert_Check(offsetToStr == dataSize);

    char* strBase = reinterpret_cast<char*>(&messageData);

    std::strncpy(strBase + messageData.offset_to_host    , m_hostName   .c_str(), m_hostName   .size() + 1); 
    std::strncpy(strBase + messageData.offset_to_process , m_processName.c_str(), m_processName.size() + 1);
    std::strncpy(strBase + messageData.offset_to_thread  , msg.threadName       , nameLenThread   + 1);
    std::strncpy(strBase + messageData.offset_to_facility, msg.facilityName     , nameLenFacility + 1);
    std::strncpy(strBase + messageData.offset_to_severity, msg.severityName     , nameLenSeverity + 1);
    std::strncpy(strBase + messageData.offset_to_text    , msg.messageText      , msg.messageLen  + 1);
}

void udpSocketConsumer::postData(const char* data, std::size_t size)
{
    constexpr std::size_t DATA_OFFSET = sizeof(gkr::data::split_packet_head);

    const std::size_t maxPacketSize = m_packet.size();
    Assert_Check(maxPacketSize > DATA_OFFSET);

    const std::size_t maxDataSizePerPacket = (maxPacketSize - DATA_OFFSET);

    const std::size_t count = ((size % maxDataSizePerPacket) == 0)
        ? ((size / maxDataSizePerPacket) + 0)
        : ((size / maxDataSizePerPacket) + 1)
        ;
    Assert_Check(count > 0);
    Assert_Check(count < 65536);

    gkr::data::split_packet_head& packetHead = m_packet.as<gkr::data::split_packet_head>();

    packetHead.packet_id    = m_packetId++;
    packetHead.packet_count = std::uint16_t(count);
//  packetHead.packet_index = 0;
//  packetHead.packet_size  = 0;
    packetHead.data_offset  = DATA_OFFSET;
    packetHead.data_size    = std::uint32_t(size);
//  packetHead.data_sent    = 0;

    char* buff = m_packet.data<char>(DATA_OFFSET);

    std::size_t restDataSize = size;
    std::size_t sentDataSize = 0;

    for(std::size_t index = 0; index < count; ++index)
    {
        Assert_Check(restDataSize > 0);
        Assert_Check(sentDataSize < size);

        const std::size_t curDataSize = (restDataSize >= maxDataSizePerPacket)
            ?  maxDataSizePerPacket
            : restDataSize
            ;
        const std::size_t packetSize = curDataSize + DATA_OFFSET;

        packetHead.packet_index = std::uint16_t(index);
        packetHead.packet_size  = std::uint16_t(packetSize);
        packetHead.data_sent    = std::uint32_t(sentDataSize);

        std::memcpy(buff, data + sentDataSize, curDataSize);

        m_socket.send_to(&packetHead, packetSize, m_remoteAddr);

        restDataSize -= curDataSize;
        sentDataSize += curDataSize;
    }
    Assert_Check(restDataSize == 0);
    Assert_Check(sentDataSize == size);
}
