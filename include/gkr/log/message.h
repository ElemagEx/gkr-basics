#pragma once

#include <cstddef>
#include <cinttypes>

namespace gkr
{
namespace log
{

struct message_info
{
	std::int64_t  tid;
	std::int64_t  stamp;
	std::uint16_t severity;
	std::uint16_t facility;
	std::uint32_t _reserved;
};
struct message : public message_info
{
	const char* threadName;
	const char* severityName;
	const char* facilityName;
	const char* messageText;
	std::size_t messageLen;
};

}
}
