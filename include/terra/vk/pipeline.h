#pragma once

#include "shader.h"
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

typedef struct terra_vk_pipeline_params {
  const char *vert_entrypoint;
  const char *frag_entrypoint;
  VkDynamicState *dyn_state;
  uint32_t dyn_state_count;
} terra_vk_pipeline_params_t;

terra_vk_pipeline_params_t terra_vk_pipeline_params_default(void);

typedef struct terra_vk_pipeline {
  // terra_vk_shader_t *vertex;
  // terra_vk_shader_t *frag;
} terra_vk_pipeline_t;

terra_status_t terra_vk_pipeline_new(terra_app_t *app,
                                     terra_vk_pipeline_params_t *params,
                                     terra_vk_shader_t *vertex,
                                     terra_vk_shader_t *frag,
                                     terra_vk_pipeline_t *out);
terra_status_t terra_vk_pipeline_from_filenames(
    terra_app_t *app, terra_vk_pipeline_params_t *params, const char *vertex,
    const char *frag, terra_vk_pipeline_t *out);

terra_status_t terra_vk_pipeline_cleanup(terra_app_t *app,
                                         terra_vk_pipeline_t *pipe);
