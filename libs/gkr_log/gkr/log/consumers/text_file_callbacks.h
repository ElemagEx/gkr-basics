#pragma once

#include <gkr/api.h>

enum
{
    gkr_log_textFileEoln_LF,
    gkr_log_textFileEoln_CR,
    gkr_log_textFileEoln_CRLF,
#if   defined(_WIN32)
    gkr_log_textFileEoln_Default = gkr_log_textFileEoln_CRLF
#elif defined(__APPLE__)
    gkr_log_textFileEoln_Default = gkr_log_textFileEoln_CR
#else
    gkr_log_textFileEoln_Default = gkr_log_textFileEoln_LF
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

GKR_LOG_API unsigned gkr_log_textFile_makeFilePath(char* buf, unsigned cch);
GKR_LOG_API unsigned gkr_log_textFile_composeOutput(char* buf, unsigned cch, const struct gkr_log_message* msg);

GKR_LOG_API void* gkr_log_textFile_createConsumerParam(
    int eoln,
    unsigned buffer_capacity,
    unsigned (*make_file_path)(char*, unsigned),
    unsigned (*compose_output)(char*, unsigned, const struct gkr_log_message*)
    );

GKR_LOG_API int  gkr_log_textFile_initLogging(void* param);
GKR_LOG_API void gkr_log_textFile_doneLogging(void* param);

GKR_LOG_API int  gkr_log_textFile_filterLogMessage (void* param, const struct gkr_log_message* msg);
GKR_LOG_API void gkr_log_textFile_consumeLogMessage(void* param, const struct gkr_log_message* msg);

#ifdef __cplusplus
}
#endif
