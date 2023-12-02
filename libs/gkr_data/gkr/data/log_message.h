#include <gkr/data/split_packet.h>

namespace gkr
{
namespace data
{

struct log_message : split_packet_data_head
{
    std:: int64_t stamp;
    std:: int64_t tid;
    std::uint32_t pid;
    std:: int32_t severity;
    std:: int32_t facility;
    std::uint16_t offset_to_host;
    std::uint16_t offset_to_process;
    std::uint16_t offset_to_thread;
    std::uint16_t offset_to_facility;
    std::uint16_t offset_to_severity;
    std::uint16_t offset_to_text;
};

constexpr std::uint32_t SIGNITURE_LOG_MESSAGE = 0x12345678;

}
}
