#pragma once

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_message;

struct gkr_log_callbacks
{
    int  (*init_logging)(void*);
    void (*done_logging)(void*);

    int  (*filter_log_message)(void*, const struct gkr_log_message*);

    void (*consume_log_message)(void*, const struct gkr_log_message*);
};


#ifdef __cplusplus
}
#endif
