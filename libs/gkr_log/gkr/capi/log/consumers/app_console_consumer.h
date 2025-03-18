#pragma once

#include <gkr/capi/api.h>
#include <gkr/capi/log/consumer.h>

enum
{
    gkr_log_appConsoleWriteMethod_puts,
    gkr_log_appConsoleWriteMethod_fputs2stdout,
    gkr_log_appConsoleWriteMethod_fputs2stderr,
#ifdef __cplusplus
    gkr_log_appConsoleWriteMethod_stream2cout,
    gkr_log_appConsoleWriteMethod_stream2cerr,
    gkr_log_appConsoleWriteMethod_stream2clog,
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_app_console_consumer_callbacks
{
    struct gkr_log_consumer_opt_callbacks opt_callbacks;

    const char* (*compose_output)(void*, const struct gkr_log_message*, unsigned*, int);
};

GKR_LOG_API int gkr_log_add_app_console_consumer(
    void* channel,
    void* param,
    const gkr_log_app_console_consumer_callbacks* callbacks,
    int method,
    int colorless
    );

#ifdef __cplusplus
}
#endif
