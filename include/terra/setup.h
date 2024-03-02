#pragma once

#include "app.h"
#include "vulkan.h"

#include <terra/status.h>

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

terra_status_t terra_init(terra_app_t *app, terra_init_params_t *params);

terra_status_t terra_init_window(terra_app_t *app);
terra_status_t terra_init_instance(terra_app_t *app);
terra_status_t terra_create_render_surface(terra_app_t *app);
terra_status_t terra_choose_pdevice(terra_app_t *app);
terra_status_t terra_create_ldevice(terra_app_t *app);
terra_status_t terra_retrieve_device_queue(terra_app_t *app);
terra_status_t terra_create_render_pass(
    terra_app_t *app, terra_init_params_t *params
);
