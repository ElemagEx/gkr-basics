#pragma once

#include <gkr/capi/api.h>
#include <gkr/capi/log/consumer.h>

enum
{
    gkr_log_tf_eoln_none,
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

typedef unsigned long long gkr_log_tfs;

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_log_text_file_consumer_callbacks
{
    struct gkr_log_consumer_opt_callbacks opt_callbacks;

    const char* (*compose_output     )(void*, const struct gkr_log_message*, unsigned*, int);
    void        (*on_file_opened     )(void*, void*);
    void        (*on_file_closing    )(void*, void*);
    void        (*on_enter_file_write)(void*, void*);
    void        (*on_leave_file_write)(void*, void*);
};

GKR_LOG_API int gkr_log_add_text_file_consumer(
    void* channel,
    void* param,
    const gkr_log_text_file_consumer_callbacks* callbacks,
    const char* filepath,
    int eoln
    );

GKR_LOG_API gkr_log_tfs gkr_log_text_file_get_size(void* arg);
GKR_LOG_API const char* gkr_log_text_file_get_path(void* arg);
GKR_LOG_API void gkr_log_text_file_write_text(void* arg, const char* text, unsigned len, int add_eoln);
GKR_LOG_API void gkr_log_text_file_roll(void* arg, unsigned max_files);

#ifdef __cplusplus
}
#endif
