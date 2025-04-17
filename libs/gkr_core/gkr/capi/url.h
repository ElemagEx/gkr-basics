#pragma once

#include <gkr/capi/api.h>

#include <stddef.h>

enum
{
    gkr_url_part_scheme,
    gkr_url_part_username,
    gkr_url_part_password,
    gkr_url_part_host,
    gkr_url_part_port,
    gkr_url_part_path,
    gkr_url_part_query,
    gkr_url_part_fragment,
    gkr_url_parts_count,
};

struct gkr_url_parts
{
    const char* str[gkr_url_parts_count];
    unsigned    len[gkr_url_parts_count];

    int         port;
    int         args;
};

#ifdef __cplusplus
extern "C" {
#endif

GKR_CORE_API gkr_bool gkr_url_decompose(struct gkr_url_parts* parts, int unescape, const char* url, size_t len);

//GKR_CORE_API int gkr_url_validate(struct gkr_url_parts* parts);

GKR_CORE_API int gkr_url_construct(const struct gkr_url_parts* parts, char* buf, int cch);

#ifdef __cplusplus
}
#endif
