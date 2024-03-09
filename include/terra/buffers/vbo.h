#pragma once

#include <stdint.h>
#include <terra/app.h>
#include <terra/data/vector.h>
#include <terra/status.h>
#include <terra/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

terra_status_t terra_vbo_new(
    terra_app_t *app,
    terra_vector_t *data,
    terra_buffer_t *out_stag,
    terra_buffer_t *out_vert
);

terra_status_t terra_vbo_bind(
    terra_app_t *app, VkCommandBuffer cmd_buffer, terra_buffer_t *buf
);

#ifdef __cplusplus
}
#endif
