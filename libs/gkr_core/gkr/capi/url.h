#pragma once

#include <gkr/capi/api.h>

struct gkr_url_parts
{
    const char* scheme;
    const char* username;
    const char* password;
    const char* host;
    const char* path;
    const char* query;
    const char* fragment;
    int         port;
    int         args;
};

#ifdef __cplusplus
extern "C" {
#endif

GKR_CORE_API int gkr_url_decompose(char* url, int unescape, struct gkr_url_parts* parts);

//GKR_CORE_API int gkr_url_validate(struct gkr_url_parts* parts);

GKR_CORE_API int gkr_url_construct(const struct gkr_url_parts* parts, char* buf, int cch);

#ifdef __cplusplus
}
#endif
