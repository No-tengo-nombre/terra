#pragma once

#include <cglm/cglm.h>
#include <terra/app.h>
#include <terra/status.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_camera {
  mat4 model;
  mat4 view;
  mat4 proj;
} terra_camera_t;

typedef struct terra_ubo_descriptor {
  uint32_t count;
  VkShaderStageFlags stage;
} terra_ubo_descriptor_t;

terra_ubo_descriptor_t terra_ubo_descriptor_default(void);

terra_status_t terra_ubo_new(
    terra_app_t *app, const char *name, uint64_t size, terra_buffer_t *out
);

#ifdef __cplusplus
}
#endif

