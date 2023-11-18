#pragma once

#include <inttypes.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_message_info
{
	int64_t  tid;
	int64_t  stamp;
	uint16_t severity;
	uint16_t facility;
	uint32_t _reserved;
};
struct gkr_log_message
{
	gkr_log_message_info info;
	const char* threadName;
	const char* severityName;
	const char* facilityName;
	const char* messageText;
	unsigned    messageLen;
};

#ifdef __cplusplus
}

namespace gkr
{
namespace log
{
using message_info = gkr_log_message_info;
using message      = gkr_log_message;
}
}

#endif
