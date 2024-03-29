#pragma once

#include "shader.h"

#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_pipeline_fnames {
  const char *vert;
  const char *frag;
  const char *geom;
  const char *comp;
  const char *tesc;
  const char *tese;
  const char *rgen;
  const char *rint;
  const char *rahit;
  const char *rchit;
  const char *rmiss;
  const char *rcall;
  const char *mesh;
  const char *task;
} terra_pipeline_fnames_t;

extern const terra_pipeline_fnames_t TERRA_DEFAULT_PIPELINE;

typedef struct terra_vk_pipeline_params {
  const char *vert_entrypoint;
  const char *frag_entrypoint;
  const VkDynamicState *dyn_state;
  uint32_t dyn_state_count;
  VkPrimitiveTopology topology;
  VkBool32 primitive_restart;
  VkBool32 depth_clamp;
  VkPolygonMode polygon_mode;
  VkCullModeFlags cull_mode;
  VkFrontFace front_face;
  VkBool32 depth_bias;
  float depth_bias_factor;
  float depth_bias_clamp;
  float depth_bias_slope;
  VkOffset2D scissor_offset;
} terra_vk_pipeline_params_t;

terra_vk_pipeline_params_t terra_vk_pipeline_params_default(void);
terra_pipeline_fnames_t terra_pipeline_fnames_default(void);

terra_status_t terra_vk_pipeline_new(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    terra_vk_shader_t *vertex,
    terra_vk_shader_t *frag
);
terra_status_t terra_vk_pipeline_from_filenames(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    const char *vertex,
    const char *frag
);

#ifdef __cplusplus
}
#endif
