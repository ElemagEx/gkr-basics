#pragma once

#include <cinttypes>

namespace gkr
{
namespace log
{

struct entry_head
{
	std::int64_t  tid;
	std::int64_t  stamp;

	std::uint16_t flags;
	std::uint16_t severity;
	std::uint16_t facility;
	std::uint16_t mesageLen;
};
struct entry_info
{
	const char* threadName;
	const char* messageText;
	const char* severityName;
	const char* facilityName;

	entry_head head;
};

}
}
