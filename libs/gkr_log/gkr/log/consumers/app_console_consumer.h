#pragma once

#include <gkr/api.h>

enum
{
    gkr_log_appConsoleWriteMethod_printf,
    gkr_log_appConsoleWriteMethod_fputs2stderr,
    gkr_log_appConsoleWriteMethod_fputs2stdout,
#ifdef __cplusplus
    gkr_log_appConsoleWriteMethod_stream2cout,
    gkr_log_appConsoleWriteMethod_stream2cerr,
    gkr_log_appConsoleWriteMethod_stream2clog,
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_app_console_consumer_callbacks {
    void         *param;
    const char* (*compose_output)(void*, const struct gkr_log_message*, int);
};

GKR_LOG_API int gkr_log_add_app_console_consumer(const gkr_log_app_console_consumer_callbacks* callbacks, int method);

#ifdef __cplusplus
}
#endif
