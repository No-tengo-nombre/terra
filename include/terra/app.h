#pragma once

#include "vulkan.h"

#include <stdint.h>
#include <terra/status.h>

#define TERRA_ENGINE_VERSION_MAJOR 1
#define TERRA_ENGINE_VERSION_MINOR 0
#define TERRA_ENGINE_VERSION_PATCH 0

/*
PREFIXES
--------
For queues
(q) Refering to queues
(g) Capable of rendering graphics
(p) Capable of presenting graphics

For devices
(p) Physical device
(l) Logical device
*/

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_app_state {
  uint64_t i;
  int should_close;
} terra_app_state_t;

typedef struct terra_queue {
  uint32_t gfamily;
  uint32_t pfamily;
  uint32_t gfound;
  uint32_t pfound;
} terra_queue_t;

typedef struct terra_app_metadata {
  uint32_t vmajor;
  uint32_t vminor;
  uint32_t vpatch;
  const char *app_name;
  const char *window_title;
  uint32_t window_width;
  uint32_t window_height;
} terra_app_metadata_t;

typedef struct terra_app_config {
  uint32_t validation_layers_total;
  uint32_t device_extensions_total;
  const char **validation_layers;
  const char **device_extensions;

  VkFormat surface_format;
  VkColorSpaceKHR color_space;
  VkPresentModeKHR present_mode;
  uint32_t image_array_layers;
  VkCompositeAlphaFlagBitsKHR composite_alpha;
  uint8_t clipped;
  VkCommandPoolCreateFlags command_pool_flags;

  /* Timeouts */

  uint64_t in_flight_fence_timeout;
  uint64_t img_acq_timeout;
} terra_app_config_t;

typedef struct terra_app {
  terra_status_t (*start)(struct terra_app *);
  terra_status_t (*loop)(struct terra_app *);
  terra_status_t (*cleanup)(struct terra_app *);

  terra_app_metadata_t *meta;
  terra_app_config_t *conf;

  terra_app_state_t state;
  void *glfw_window;

  /* Vulkan attributes */

  terra_queue_t vk_qinfo;
  VkInstance vk_instance;
  VkPhysicalDevice vk_pdevice;
  VkDevice vk_ldevice;
  VkQueue vk_gqueue;
  VkQueue vk_pqueue;
  VkSurfaceKHR vk_surface;
  VkSwapchainKHR vk_swapchain;
  VkExtent2D vk_extent;
  VkFormat vk_format;
  VkPipelineLayout vk_layout;
  VkRenderPass vk_render_pass;
  VkPipeline vk_pipeline;
  VkCommandPool vk_commands;
  VkCommandBuffer vk_command_buffer;

  VkImage *vk_images;
  VkImageView *vk_image_views;
  VkFramebuffer *vk_framebuffers;
  uint32_t vk_images_count;

  /* Synchronization variables */

  VkSemaphore vk_img_available_S;
  VkSemaphore vk_render_finished_S;
  VkFence vk_in_flight_F;

#ifndef NDEBUG
  /* Internal debug information */

  size_t _idebug_malloced_total;
#endif
} terra_app_t;

typedef terra_status_t(terra_start_ft)(terra_app_t *);
typedef terra_status_t(terra_loop_ft)(terra_app_t *);
typedef terra_status_t(terra_clean_ft)(terra_app_t *);

terra_app_state_t terra_app_state_default(void);
terra_app_metadata_t terra_app_metadata_default(void);
terra_status_t terra_app_config_new(
    const char **validation_layers,
    const char **device_extensions,
    uint32_t validation_layers_total,
    uint32_t device_extensions_total,
    terra_app_config_t *out
);
terra_app_config_t terra_app_config_default(void);

terra_status_t terra_app_new(
    terra_start_ft *start,
    terra_loop_ft *loop,
    terra_clean_ft *cleanup,
    terra_app_metadata_t *meta,
    terra_app_config_t *conf,
    terra_app_t *out
);
terra_status_t terra_app_new_wstate(
    terra_app_state_t state,
    terra_start_ft *start,
    terra_loop_ft *loop,
    terra_clean_ft *cleanup,
    terra_app_metadata_t *meta,
    terra_app_config_t *conf,
    terra_app_t *out
);
terra_status_t terra_app_run(terra_app_t *app);
terra_status_t terra_app_set_image_count(terra_app_t *app, uint32_t new_count);

terra_status_t terra_app_record_cmd_buffer(
    terra_app_t *app, uint32_t idx
);
terra_status_t terra_app_draw(terra_app_t *app);

terra_status_t terra_app_cleanup(terra_app_t *app);

int terra_app_should_close(terra_app_t *app);

#ifdef __cplusplus
}
#endif
