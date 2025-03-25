#pragma once

#include <gkr/capi/api.h>

struct gkr_url_parts
{
    char* scheme;
    char* username;
    char* password;
    char* host;
    char* path;
    char* query;
    char* fragment;
    int   port;
    int   args;
};

#ifdef __cplusplus
extern "C" {
#endif

GKR_CORE_API int gkr_url_decompose(char* url, int unescape, gkr_url_parts* parts);

#ifdef __cplusplus
}
#endif
