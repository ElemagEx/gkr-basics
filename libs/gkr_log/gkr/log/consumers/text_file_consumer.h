#pragma once

#include <gkr/api.h>

enum {
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

struct gkr_log_text_file_consumer_callbacks
{
    void      *param;
    unsigned (*compose_output     )(void*, const struct gkr_log_message*, char*, unsigned);
    void     (*on_file_opened     )(void*);
    void     (*on_file_closing    )(void*);
    void     (*on_enter_file_write)(void*);
    void     (*on_leave_file_write)(void*);
};

GKR_LOG_API int gkr_log_add_text_file_consumer(
    const gkr_log_text_file_consumer_callbacks* callbacks,
    const char* filepath,
    int eoln,
    unsigned bufferCapacity
    );

#ifdef __cplusplus
}
#endif
