#include <cinttypes>

namespace gkr
{
namespace data
{

struct split_packet_head
{
    std::uint64_t packet_id;
    std::uint16_t packet_count;
    std::uint16_t packet_index;
    std::uint16_t packet_size;
    std::uint16_t data_offset;
    std::uint32_t data_size;
    std::uint32_t data_sent;
};
static_assert((sizeof(split_packet_head) % 8) == 0, "Please keep size of split_packet_head multiple of 8");

struct split_packet_data_head
{
    std::uint32_t signature;
    std::uint32_t size;
};
static_assert((sizeof(split_packet_data_head) % 8) == 0, "Please keep size of split_packet_data_head multiple of 8");

}
}
