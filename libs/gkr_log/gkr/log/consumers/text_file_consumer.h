#pragma once

#include <gkr/api.h>

enum {
    gkr_log_tf_eoln_lf,
    gkr_log_tf_eoln_cr,
    gkr_log_tf_eoln_crlf,
#if   defined(_WIN32)
    gkr_log_tf_eoln_default = gkr_log_tf_eoln_crlf
#elif defined(__APPLE__)
    gkr_log_tf_eoln_default = gkr_log_tf_eoln_cr
#else
    gkr_log_tf_eoln_default = gkr_log_tf_eoln_lf
#endif
};

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_text_file_consumer_callbacks
{
    void         *param;
    const char* (*compose_output     )(void*, const struct gkr_log_message*);
    void        (*on_file_opened     )(void*);
    void        (*on_file_closing    )(void*);
    void        (*on_enter_file_write)(void*);
    void        (*on_leave_file_write)(void*);
};

GKR_LOG_API int gkr_log_add_text_file_consumer(
    void* instance,
    const gkr_log_text_file_consumer_callbacks* callbacks,
    const char* filepath,
    int eoln
    );

#ifdef __cplusplus
}
#endif
