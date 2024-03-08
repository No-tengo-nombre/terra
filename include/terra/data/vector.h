#pragma once

#include <stddef.h>
#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>

typedef struct terra_vector {
  void *data;
  size_t data_size;
  size_t len;
  size_t capacity;
} terra_vector_t;

/* Vector initialization */

terra_status_t terra_vector_new(
    terra_app_t *app, const size_t data_size, terra_vector_t *out
);
terra_status_t terra_vector_zero(
    terra_app_t *app,
    const size_t len,
    const size_t data_size,
    terra_vector_t *out
);
terra_status_t terra_vector_with_capacity(
    terra_app_t *app,
    const size_t capacity,
    const size_t data_size,
    terra_vector_t *out
);
terra_status_t terra_vector_from_array(
    terra_app_t *app,
    const void *data,
    const size_t len,
    const size_t data_size,
    terra_vector_t *out
);

/* Vector finalization */

terra_status_t terra_vector_cleanup(
    terra_app_t *app, const terra_vector_t *const vec
);

/* Vector attributes */

terra_status_t terra_vector_get(
    terra_app_t *app, const terra_vector_t *vec, const size_t i, void *out
);
terra_status_t terra_vector_set(
    terra_app_t *app, terra_vector_t *vec, const size_t i, const void *src
);
terra_status_t terra_vector_resize(
    terra_app_t *app, terra_vector_t *vec, const size_t new_capacity
);
terra_status_t terra_vector_push(
    terra_app_t *app, terra_vector_t *vec, const void *src
);
terra_status_t terra_vector_extend_array(
    terra_app_t *app, terra_vector_t *vec, const void *src, const size_t len
);
