#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_message
{
    long long      tid;
    long long      stamp;
    unsigned short severity;
    unsigned short facility;
    unsigned int   messageLen;
    const char*    messageText;
    const char*    threadName;
    const char*    severityName;
    const char*    facilityName;
};

#ifdef __cplusplus
}

namespace gkr
{
namespace log
{
using message = gkr_log_message;
}
}

#endif
