#include <gkr/log/message.h>

namespace gkr
{
namespace net
{

struct split_packet_head
{
    std::uint64_t packet_id;
    std::uint16_t packet_count;
    std::uint16_t packet_index;//???
    std::uint16_t packet_size;
    std::uint16_t data_offset;
    std::uint32_t data_size;
    std::uint32_t data_sent;
};
struct packet_data_head
{
    std::uint32_t signature;
    std::uint32_t size;
};

}

namespace log
{

struct message_desc
{
    std::uint32_t pid;
    std::uint16_t offset_to_host;
    std::uint16_t offset_to_process;
    std::uint16_t offset_to_thread;
    std::uint16_t offset_to_facility;
    std::uint16_t offset_to_severity;
    std::uint16_t offset_to_text;
};
struct message_data
{
    net::packet_data_head head;
    message_info          info;
    message_desc          desc;
};

constexpr std::uint32_t SIGNITURE_LOG_MSG = 0x12345678;

}
}
