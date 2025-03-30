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

#include <gkr/capi/api.h>

#include <stddef.h>

#ifdef __cplusplus
extern "C" {
#endif

struct gkr_params;

GKR_CORE_API struct gkr_params* gkr_params_create();

GKR_CORE_API int gkr_params_destroy(struct gkr_params* params);

GKR_CORE_API int gkr_params_copy(struct gkr_params* params, struct gkr_params* other_params, size_t pitch);
GKR_CORE_API int gkr_params_clear(struct gkr_params* params);
GKR_CORE_API int gkr_params_reserve(struct gkr_params* params, size_t size, size_t pitch);

GKR_CORE_API int gkr_params_reset_root(struct gkr_params* params, size_t index);

GKR_CORE_API size_t gkr_params_insert_object(struct gkr_params* params, const char* key);
GKR_CORE_API size_t gkr_params_insert_array (struct gkr_params* params, const char* key);
GKR_CORE_API size_t gkr_params_insert_null  (struct gkr_params* params, const char* key);

GKR_CORE_API size_t gkr_params_insert_string_value (struct gkr_params* params, const char* key, const char* value);
GKR_CORE_API size_t gkr_params_insert_double_value (struct gkr_params* params, const char* key, double value);
GKR_CORE_API size_t gkr_params_insert_integer_value(struct gkr_params* params, const char* key, long long value);
GKR_CORE_API size_t gkr_params_insert_boolean_value(struct gkr_params* params, const char* key, int value);

GKR_CORE_API size_t gkr_params_find_value(struct gkr_params* params, const char* key);

GKR_CORE_API enum gkr_param_type gkr_params_get_type(struct gkr_params* params, const char* key);

GKR_CORE_API const char* gkr_params_get_string_value (struct gkr_params* params, size_t index, const char* def_val);
GKR_CORE_API double      gkr_params_get_double_value (struct gkr_params* params, size_t index, double def_val);
GKR_CORE_API long long   gkr_params_get_integer_value(struct gkr_params* params, size_t index, long long def_val);
GKR_CORE_API int         gkr_params_get_boolean_value(struct gkr_params* params, size_t index, int def_val);

#ifdef __cplusplus
}
#endif
