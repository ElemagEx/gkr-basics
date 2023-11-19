#include <gkr/log/message.h>

namespace gkr
{
namespace net
{

struct split_packet_head
{
    uint64_t packet_id;
    uint16_t packet_count;
    uint16_t packet_index;
    uint16_t packet_size;
    uint16_t data_offset;
    uint32_t data_size;
    uint32_t data_sent;
};
struct packet_data_head
{
    uint32_t signature;
    uint32_t size;
};

}

namespace log
{

struct message_desc
{
    uint32_t pid;
    uint16_t offset_to_host;
    uint16_t offset_to_process;
    uint16_t offset_to_thread;
    uint16_t offset_to_facility;
    uint16_t offset_to_severity;
    uint16_t offset_to_text;
};
struct message_data
{
    net::packet_data_head head;
    message_info          info;
    message_desc          desc;
};

constexpr uint32_t SIGNITURE_LOG_MSG = 0x12345678;

}
}
