#pragma once

#include "vulkan.h"

#include <stddef.h>
#include <terra/status.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_app terra_app_t;
typedef struct terra_pipeline_fnames terra_pipeline_fnames_t;
typedef struct terra_vk_pipeline_params terra_vk_pipeline_params_t;

typedef struct terra_init_params {
  VkImageUsageFlags image_usage;
  VkImageViewType view_type;
  VkSampleCountFlagBits samples;
  VkAttachmentLoadOp load_op;
  VkAttachmentStoreOp store_op;
  VkAttachmentLoadOp stencil_load_op;
  VkAttachmentStoreOp stencil_store_op;
  VkImageLayout initial_layout;
  VkImageLayout final_layout;
} terra_init_params_t;

terra_status_t terra_init_params_default(terra_init_params_t *out);

terra_status_t terra_init(
    terra_app_t *app,
    terra_init_params_t *params,
    terra_vk_pipeline_params_t *pipelines_params,
    terra_pipeline_fnames_t *pipelines_files,
    size_t pipelines_count
);

#ifndef NDEBUG
terra_status_t terra_init_debug(terra_app_t *app);
terra_status_t terra_init_debug_callback(terra_app_t *app);
#endif

terra_status_t terra_init_window(terra_app_t *app);
terra_status_t terra_init_instance(terra_app_t *app);
terra_status_t terra_init_vma(terra_app_t *app);
terra_status_t terra_create_render_surface(terra_app_t *app);
terra_status_t terra_choose_pdevice(terra_app_t *app);
terra_status_t terra_create_ldevice(terra_app_t *app);
terra_status_t terra_retrieve_device_queue(terra_app_t *app);
terra_status_t terra_create_render_pass(
    terra_app_t *app, terra_init_params_t *params
);
terra_status_t terra_create_camera(terra_app_t *app);

terra_status_t terra_recreate_swapchain(
    terra_app_t *app, terra_init_params_t *params
);

#ifdef __cplusplus
}
#endif
