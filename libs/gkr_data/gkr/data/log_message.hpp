#include <gkr/data/split_packet.hpp>

namespace gkr
{
namespace data
{

struct log_message : split_packet_data_head
{
    std:: int64_t stamp;
    std:: int64_t tid;
    std::uint32_t pid;
    std::uint32_t line;
    std:: int32_t severity;
    std:: int32_t facility;
    std::uint32_t _reserved; //count
    std::uint16_t sized_ver;
    std::uint16_t offset_to_text;
    std::uint16_t offset_to_func;
    std::uint16_t offset_to_file;
    std::uint16_t offset_to_host;
    std::uint16_t offset_to_process;
    std::uint16_t offset_to_module;
    std::uint16_t offset_to_thread;
    std::uint16_t offset_to_severity;
    std::uint16_t offset_to_facility;
};
static_assert((sizeof(log_message) % 8) == 0, "Please keep size of log_message multiple of 8");

constexpr std::uint32_t SIGNITURE_LOG_MESSAGE = 0x12345678;

}
}
