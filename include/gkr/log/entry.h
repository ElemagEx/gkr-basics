#pragma once

//TODO:Rename file to message

#include <cinttypes>

namespace gkr
{
namespace log
{

struct message_head
{
	std::int64_t  tid;
	std::int64_t  stamp;

	std::uint16_t severity;
	std::uint16_t facility;
	std::uint16_t mesageLen;
	std::uint16_t _reserved;
};
struct message : public message_head
{
	const char* threadName;
	const char* messageText;
	const char* severityName;
	const char* facilityName;
};

}
}
