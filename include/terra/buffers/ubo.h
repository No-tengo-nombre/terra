#pragma once

#include <cglm/cglm.h>
#include <terra/app.h>
#include <terra/status.h>

typedef struct terra_camera {
  mat4 model;
  mat4 view;
  mat4 proj;
} terra_camera_t;

terra_status_t terra_ubo_new(
    terra_app_t *app, uint64_t size, terra_buffer_t *out
);
terra_status_t terra_ubo_bind(
    terra_app_t *app, VkCommandBuffer cmd_buffer, terra_buffer_t *buf
);
