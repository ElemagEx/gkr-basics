#pragma once

//TODO:Check for tty

#include <gkr/log/api.h>

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

GKR_LOG_API unsigned gkr_log_appConsole_composeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* gkr_log_appConsole_createConsumerParam(
    int method,
    unsigned bufferCapacity,
    unsigned (*composeOutput)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int  gkr_log_appConsole_initLogging(void* param);
GKR_LOG_API void gkr_log_appConsole_doneLogging(void* param);

GKR_LOG_API int  gkr_log_appConsole_filterLogMessage (void* param, const struct gkr_log_message* msg);
GKR_LOG_API void gkr_log_appConsole_consumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}
#endif
