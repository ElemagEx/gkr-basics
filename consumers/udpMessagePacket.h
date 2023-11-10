#include <gkr/log/message.h>

namespace gkr
{
namespace log
{

struct message_packet_head
{
    std::uint32_t packet_id;
    std::uint8_t  packet_count;
    std::uint8_t  packet_index;
    std::uint16_t packet_size;
    std::uint16_t data_offset;
    std::uint16_t data_size;
    std::uint16_t msg_offset;
    std::uint16_t msg_size;
};
struct message_desc : message_head
{
    std::int32_t  pid;
    std::int32_t  _unused0;
    std::uint16_t size;
    std::uint16_t offset_to_host;
    std::uint16_t offset_to_process;
    std::uint16_t offset_to_thread;
    std::uint16_t offset_to_facility;
    std::uint16_t offset_to_severity;
    std::uint16_t offset_to_text;
    std::uint16_t _unused1;
};

}
}
