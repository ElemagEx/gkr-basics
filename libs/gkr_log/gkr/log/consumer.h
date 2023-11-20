#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_message;

struct gkr_log_functions
{
    int  (*init_logging)(void*);
    void (*done_logging)(void*);

    int  (*filter_log_message)(void*, const struct gkr_log_message*);

    void (*consume_log_message)(void*, const struct gkr_log_message*);
};

#ifdef __cplusplus
}

namespace gkr
{
namespace log
{

using message = gkr_log_message;

class consumer
{
public:
    virtual ~consumer() = default;

    virtual bool init_logging() = 0;
    virtual void done_logging() = 0;

    virtual bool filter_log_message(const message& msg) = 0;

    virtual void consume_log_message(const message& msg) = 0;
};

}
}
#endif
