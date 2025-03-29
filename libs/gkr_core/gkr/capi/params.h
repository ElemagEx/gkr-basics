#pragma once

enum gkr_param_type
{
    gkr_param_type_none,
    gkr_param_type_null,
    gkr_param_type_array,
    gkr_param_type_object,
    gkr_param_type_string,
    gkr_param_type_double,
    gkr_param_type_integer,
    gkr_param_type_boolean,
};

#ifdef GKR_HIDE_PARAMS_CDEFS
#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_params;

GKR_CORE_API struct gkr_params* gkr_params_create();

GKR_CORE_API int gkr_params_destroy(struct gkr_params* params);
GKR_CORE_API int gkr_params_clear(struct gkr_params* params);

GKR_CORE_API int gkr_params_reserve(struct gkr_params* params, size_t size);

GKR_CORE_API size_t gkr_params_insert_null_value(struct gkr_params* params, const char* key);
GKR_CORE_API size_t gkr_params_insert_real_value(struct gkr_params* params, const char* key, double value);
GKR_CORE_API size_t gkr_params_insert_integer_value(struct gkr_params* params, const char* key, long long value);
GKR_CORE_API size_t gkr_params_insert_string_value(struct gkr_params* params, const char* key, const char* value);

#ifdef __cplusplus
}
#endif
#endif
