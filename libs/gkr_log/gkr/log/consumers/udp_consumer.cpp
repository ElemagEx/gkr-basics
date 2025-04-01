#include <gkr/defs.hpp>
#include <gkr/log/consumers/udp_consumer.hpp>

#include <gkr/net/udp_sender.hpp>

#include <gkr/data/log_message.hpp>
#include <gkr/log/logging.hpp>
#include <gkr/log/c_consumer.hpp>

#include <gkr/sys/process.hpp>
#include <gkr/net/lib.hpp>

#include <gkr/diagnostics.hpp>

#include <cstring>

extern "C"
{

int gkr_log_add_udp_consumer(
    void* channel,
    void* param,
    const gkr_log_udp_consumer_callbacks* callbacks,
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize
    )
{
    const struct gkr_log_consumer_opt_callbacks* opt_callbacks = (callbacks == nullptr) ? nullptr : &callbacks->opt_callbacks;

    std::shared_ptr<gkr::log::consumer> consumer(new gkr::log::c_consumer<gkr::log::udp_consumer>(param, opt_callbacks, remoteHost, remotePort, maxPacketSize));

    return gkr_log_add_consumer(channel, consumer);
}

}

namespace gkr
{
namespace log
{

static_assert(udp_consumer::MINIMUM_UDP_PACKET_SIZE > sizeof(data::split_packet_head), "Minimum size is not enough to deliver data");

udp_consumer::udp_consumer(
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize
    )
    : m_sender(new net::udp_sender(maxPacketSize))
    , m_process_id(sys::get_current_process_id())
{
    update_buffer(std::size_t(gkr_log_get_max_message_chars()) + 257);

    Check_Arg_IsValid(maxPacketSize >= MINIMUM_UDP_PACKET_SIZE, );

    m_sender->change_remote_address(remoteHost, remotePort);
}

udp_consumer::~udp_consumer()
{
    update_buffer(0);

    if(m_sender != nullptr) delete m_sender;
}

bool udp_consumer::init_logging()
{
    Check_ValidState(m_sender->max_packet_size() >= MINIMUM_UDP_PACKET_SIZE, false);

    m_proc_name = sys::get_current_process_name();
    m_host_name = net::get_hostname();

    Check_ValidState( m_sender->remote_address().is_valid(), false);
    Check_ValidState(!m_sender->is_started(), false);

    return m_sender->start_sending_packets();
}

void udp_consumer::done_logging()
{
    m_sender->stop_sending_packets();
}

bool udp_consumer::filter_log_message(const message& msg)
{
    return false;
}

void udp_consumer::consume_log_message(const message& msg)
{
    if(construct_data(msg))
    {
        const data::log_message& messageData = *reinterpret_cast<data::log_message*>(m_buf);

        if(!m_sender->send_data(m_buf, messageData.size))
        {
            //TODO:LOG
        }
    }
}

const char* udp_consumer::compose_output(const message& msg, unsigned* len, int flags)
{
    return nullptr;
}

void udp_consumer::update_buffer(std::size_t size)
{
    if((size > 0) && (size <= m_cap)) return;

    if(m_buf != nullptr) delete [] m_buf;

    m_buf = (size == 0)
        ? nullptr
        : new char[size]
        ;
}

bool udp_consumer::construct_data(const message& msg)
{
    const std::size_t nameLenFunc     = std::strlen(msg.sourceFunc);
    const std::size_t nameLenFile     = std::strlen(msg.sourceFile);
    const std::size_t nameLenChannel  = std::strlen(msg.channelName);
    const std::size_t nameLenThread   = std::strlen(msg.threadName);
    const std::size_t nameLenFacility = std::strlen(msg.facilityName);
    const std::size_t nameLenSeverity = std::strlen(msg.severityName);

    const std::size_t dataSize =
        sizeof(data::log_message)
        + msg.messageLen     + 1
        + m_host_name.size() + 1
        + m_proc_name.size() + 1
        + nameLenFunc        + 1
        + nameLenFile        + 1
        + nameLenChannel     + 1
        + nameLenThread      + 1
        + nameLenSeverity    + 1
        + nameLenFacility    + 1
        ;
    update_buffer(dataSize);

    data::log_message& messageData = *reinterpret_cast<data::log_message*>(m_buf);

    messageData.signature = data::SIGNITURE_LOG_MESSAGE;
    messageData.size      = std::uint32_t(dataSize);

    messageData.stamp     = msg.stamp;
    messageData.tid       = msg.tid;
    messageData.pid       = m_process_id;
    messageData.severity  = msg.severity;
    messageData.facility  = msg.facility;
    messageData._reserved = 0;
    messageData.sized_ver = sizeof(data::log_message);

    std::size_t offsetToStr = sizeof(data::log_message);

    messageData.offset_to_text     = std::uint16_t(offsetToStr); offsetToStr += msg.messageLen     + 1;
    messageData.offset_to_host     = std::uint16_t(offsetToStr); offsetToStr += m_host_name.size() + 1;
    messageData.offset_to_process  = std::uint16_t(offsetToStr); offsetToStr += m_proc_name.size() + 1;
    messageData.offset_to_func     = std::uint16_t(offsetToStr); offsetToStr += nameLenFunc        + 1;
    messageData.offset_to_file     = std::uint16_t(offsetToStr); offsetToStr += nameLenFile        + 1;
    messageData.offset_to_thread   = std::uint16_t(offsetToStr); offsetToStr += nameLenChannel     + 1;
    messageData.offset_to_thread   = std::uint16_t(offsetToStr); offsetToStr += nameLenThread      + 1;
    messageData.offset_to_severity = std::uint16_t(offsetToStr); offsetToStr += nameLenSeverity    + 1;
    messageData.offset_to_facility = std::uint16_t(offsetToStr); offsetToStr += nameLenFacility    + 1;

    Assert_Check(offsetToStr == dataSize);

    char* strBase = reinterpret_cast<char*>(&messageData);

    std::strncpy(strBase + messageData.offset_to_text    , msg.messageText    , msg.messageLen     + 1);
    std::strncpy(strBase + messageData.offset_to_host    , m_host_name.c_str(), m_host_name.size() + 1); 
    std::strncpy(strBase + messageData.offset_to_process , m_proc_name.c_str(), m_proc_name.size() + 1);
    std::strncpy(strBase + messageData.offset_to_func    , msg.threadName     , nameLenFunc        + 1);
    std::strncpy(strBase + messageData.offset_to_file    , msg.threadName     , nameLenFile        + 1);
    std::strncpy(strBase + messageData.offset_to_thread  , msg.threadName     , nameLenThread      + 1);
    std::strncpy(strBase + messageData.offset_to_severity, msg.severityName   , nameLenSeverity    + 1);
    std::strncpy(strBase + messageData.offset_to_facility, msg.facilityName   , nameLenFacility    + 1);

    return true;
}

}
}
