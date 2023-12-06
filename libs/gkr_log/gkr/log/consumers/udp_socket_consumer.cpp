#include "udp_socket_consumer.hpp"

#include <gkr/comm/udp_socket_sender.hpp>

#include <gkr/data/log_message.hpp>
#include <gkr/sys/process.hpp>
#include <gkr/net/lib.hpp>

#include <gkr/diagnostics.h>

#include <cstring>

extern "C" {

void* gkr_log_udpSocket_createConsumerParam(
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize,
    unsigned bufferCapacity
    )
{
    return new gkr::log::udp_socket_consumer(remoteHost, remotePort, maxPacketSize, bufferCapacity);
}

int gkr_log_udpSocket_initLogging(void* param)
{
    if(param == nullptr) return false;

    gkr::log::udp_socket_consumer* consumer = static_cast<gkr::log::udp_socket_consumer*>(param);

    return consumer->init_logging();
}

void gkr_log_udpSocket_doneLogging(void* param)
{
    if(param == nullptr) return;

    gkr::log::udp_socket_consumer* consumer = static_cast<gkr::log::udp_socket_consumer*>(param);

    consumer->done_logging();

    delete consumer;
}

int gkr_log_udpSocket_filterLogMessage(void* param, const struct gkr_log_message* msg)
{
    gkr::log::udp_socket_consumer* consumer = static_cast<gkr::log::udp_socket_consumer*>(param);

    return consumer->filter_log_message(*msg);
}

void gkr_log_udpSocket_consumeLogMessage(void* param, const struct gkr_log_message* msg)
{
    gkr::log::udp_socket_consumer* consumer = static_cast<gkr::log::udp_socket_consumer*>(param);

    consumer->consume_log_message(*msg);
}

}

namespace gkr
{
namespace log
{

static_assert(udp_socket_consumer::MINIMUM_UDP_PACKET_SIZE > sizeof(data::split_packet_head), "Minimum size is not enough to deliver data");

udp_socket_consumer::udp_socket_consumer(
    const char*    remoteHost,
    unsigned short remotePort,
    unsigned maxPacketSize,
    unsigned bufferCapacity
    )
    : m_sender(new comm::udp_socket_sender(maxPacketSize))
    , m_bufferPtr(new char [bufferCapacity])
    , m_bufferCap(bufferCapacity)
    , m_processId(unsigned(sys::get_current_process_id()))
{
    Check_Arg_IsValid(maxPacketSize >= MINIMUM_UDP_PACKET_SIZE, );

    m_sender->change_remote_address(remoteHost, remotePort);
}

udp_socket_consumer::~udp_socket_consumer()
{
    if(m_bufferPtr != nullptr) delete [] m_bufferPtr;
    if(m_sender    != nullptr) delete    m_sender;
}

bool udp_socket_consumer::init_logging()
{
    Check_ValidState(m_sender->max_packet_size() >= MINIMUM_UDP_PACKET_SIZE, false);

    if(!retrieve_process_name()) return false;
    if(!retrieve_host_name()) return false;

    Check_ValidState( m_sender->remote_address().is_valid(), false);
    Check_ValidState(!m_sender->is_started(), false);

    return m_sender->start_sending_packets();
}

void udp_socket_consumer::done_logging()
{
    m_sender->stop_sending_packets();
}

bool udp_socket_consumer::filter_log_message(const message& msg)
{
    return false;
}

void udp_socket_consumer::consume_log_message(const message& msg)
{
    if(construct_data(msg))
    {
        const data::log_message& messageData = *reinterpret_cast<data::log_message*>(m_bufferPtr);

        if(!m_sender->send_data(m_bufferPtr, messageData.size))
        {
            //TODO:LOG
        }
    }
}

bool udp_socket_consumer::retrieve_process_name()
{
    char name[256];

    const int len = sys::get_current_process_name(name, 256);

    Check_ValidState(len > 0, false);

    m_processName.assign(name, unsigned(len));

    return true;
}

bool udp_socket_consumer::retrieve_host_name()
{
    char name[256];

    const int len = net::get_hostname(name, 256);

    Check_ValidState(len > 0, false);

    m_hostName.assign(name, unsigned(len));

    return true;
}

bool udp_socket_consumer::construct_data(const message& msg)
{
    const std::size_t nameLenThread   = std::strlen(msg.threadName);
    const std::size_t nameLenFacility = std::strlen(msg.facilityName);
    const std::size_t nameLenSeverity = std::strlen(msg.severityName);

    const std::size_t dataSize =
        sizeof(data::log_message)
        + m_hostName   .size() + 1
        + m_processName.size() + 1
        + nameLenThread   + 1
        + nameLenFacility + 1
        + nameLenSeverity + 1
        + msg.messageLen  + 1
        ;
    Check_ValidState(m_bufferCap >= dataSize, false);

    data::log_message& messageData = *reinterpret_cast<data::log_message*>(m_bufferPtr);

    messageData.signature = data::SIGNITURE_LOG_MESSAGE;
    messageData.size      = std::uint32_t(dataSize);

    messageData.stamp     = msg.stamp;
    messageData.tid       = msg.tid;
    messageData.pid       = m_processId;
    messageData.severity  = msg.severity;
    messageData.facility  = msg.facility;

    std::size_t offsetToStr = sizeof(data::log_message);

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

    return true;
}

}
}
