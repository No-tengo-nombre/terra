#include <string.h>
#include <terra/terra.h>
#include <terra_utils/vendor/log.h>
#include <terrau/mem.h>

terra_status_t terra_vector_new(
    terra_app_t *app, const size_t data_size, terra_vector_t *out
) {
  terra_vector_t result = {
      .data      = NULL,
      .data_size = data_size,
      .len       = 0,
      .capacity  = 0,
  };
  *out = result;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_zero(
    terra_app_t *app,
    const size_t len,
    const size_t data_size,
    terra_vector_t *out
) {
  void *data = terrau_calloc(app, len, data_size);
  if (data == NULL) {
    return TERRA_STATUS_FAILURE;
  }

  terra_vector_t result = {
      .data      = data,
      .data_size = data_size,
      .len       = len,
      .capacity  = len,
  };
  *out = result;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_with_capacity(
    terra_app_t *app,
    const size_t capacity,
    const size_t data_size,
    terra_vector_t *out
) {
  void *data = terrau_malloc(app, data_size * capacity);
  if (data == NULL) {
    return TERRA_STATUS_FAILURE;
  }

  terra_vector_t result = {
      .data      = data,
      .data_size = data_size,
      .len       = 0,
      .capacity  = capacity,
  };
  *out = result;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_from_array(
    terra_app_t *app,
    const void *data,
    const size_t len,
    const size_t data_size,
    terra_vector_t *out
) {
  void *new_data = terrau_malloc(app, len * data_size);
  if (new_data == NULL) {
    return TERRA_STATUS_FAILURE;
  }

  memcpy(new_data, data, len * data_size);
  terra_vector_t result = {
      .data      = new_data,
      .data_size = data_size,
      .len       = len,
      .capacity  = len,
  };
  *out = result;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_cleanup(
    terra_app_t *app, const terra_vector_t *const vec
) {
  terrau_free(app, vec->data);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_get(
    terra_app_t *app, const terra_vector_t *vec, const size_t i, void *out
) {
  if (vec->data == NULL || i >= vec->len || out == NULL) {
    return TERRA_STATUS_FAILURE;
  }
  memcpy(out, (char *)vec->data + i * vec->data_size, vec->data_size);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_set(
    terra_app_t *app, terra_vector_t *vec, const size_t i, const void *src
) {
  if (vec->data == NULL || i >= vec->len || src == NULL) {
    return TERRA_STATUS_FAILURE;
  }
  memcpy((char *)vec->data + i * vec->data_size, src, vec->data_size);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_resize(
    terra_app_t *app, terra_vector_t *vec, const size_t new_capacity
) {
  void *data = terrau_realloc(app, vec->data, new_capacity * vec->data_size);
  if (data == NULL) {
    return TERRA_STATUS_FAILURE;
  }

  if (new_capacity < vec->len) {
    vec->len = new_capacity;
  }
  vec->capacity = new_capacity;
  vec->data     = data;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_push(
    terra_app_t *app, terra_vector_t *vec, const void *src
) {
  if (src == NULL) {
    return TERRA_STATUS_FAILURE;
  }

  if (vec->len == vec->capacity) {
    // The vector is full so we have to reallocate
    // Default strategy is duplicate the current capacity
    // Perhaps the user can customize the behaviour?
    void *new_data =
        terrau_realloc(app, vec->data, (2 * vec->capacity) * vec->data_size);
    if (new_data == NULL) {
      return TERRA_STATUS_FAILURE;
    }
    memcpy((char *)new_data + vec->len * vec->data_size, src, vec->data_size);
    vec->data = new_data;
    vec->len++;
    vec->capacity++;
  } else {
    // It can be assumed that vec->len < vec->capacity, so this
    // means we dont have to allocate more memory.
    memcpy((char *)vec->data + vec->len * vec->data_size, src, vec->data_size);
    vec->len++;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vector_extend_array(
    terra_app_t *app, terra_vector_t *vec, const void *src, const size_t len
) {
  if (src == NULL) {
    return TERRA_STATUS_FAILURE;
  }

  uint32_t delta = len - (vec->capacity - vec->len);
  if (delta > 0) {
    // The vector does not have enough space so we have to allocate more
    // memory
    logi_info("Allocating memory for extension of vector");
    logi_debug("Allocating %d more bytes", delta);
    void *new_data = terrau_realloc(
        app, vec->data, (vec->capacity + delta) * vec->data_size
    );
    if (new_data == NULL) {
      logi_error("Reallocation failed");
      return TERRA_STATUS_FAILURE;
    }
    logi_debug("Copying new memory");
    memcpy(
        (char *)new_data + vec->len * vec->data_size, src, len * vec->data_size
    );
    vec->data      = new_data;
    vec->len      += len;
    vec->capacity += delta;
    logi_debug("New len is %d", vec->len);
    logi_debug("New capacity is %d", vec->capacity);
  } else {
    // It can be assumed that vec->len < vec->capacity, so this
    // means we dont have to allocate more memory.
    memcpy(
        (char *)vec->data + vec->len * vec->data_size, src, len * vec->data_size
    );
    vec->len += len;
  }
  return TERRA_STATUS_SUCCESS;
}
