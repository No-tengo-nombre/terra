#pragma once

#include <stdint.h>
#include <terra/status.h>
#include <vulkan/vulkan.h>

#define TERRAR_ENGINE_VERSION_MAJOR 1
#define TERRAR_ENGINE_VERSION_MINOR 0
#define TERRAR_ENGINE_VERSION_PATCH 0

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

typedef struct terrar_app_state {
  uint64_t i;
  int should_close;
} terrar_app_state_t;

typedef struct terrar_queue {
  uint32_t gfamily;
  uint32_t pfamily;
  int gfound;
  int pfound;
} terrar_queue_t;

typedef struct terrar_app_metadata {
  uint32_t vmajor;
  uint32_t vminor;
  uint32_t vpatch;
  const char *app_name;
  const char *window_title;
  uint32_t window_width;
  uint32_t window_height;
} terrar_app_metadata_t;

typedef struct terrar_app_config {
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
} terrar_app_config_t;

typedef struct terrar_app {
  terra_status_t (*start)(struct terrar_app *);
  terra_status_t (*loop)(struct terrar_app *);
  terra_status_t (*cleanup)(struct terrar_app *);

  terrar_app_metadata_t *meta;
  terrar_app_config_t *conf;

  terrar_app_state_t state;
  void *glfw_window;

  /* Vulkan attributes */

  terrar_queue_t vk_qinfo;
  VkInstance vk_instance;
  VkPhysicalDevice vk_pdevice;
  VkDevice vk_ldevice;
  VkQueue vk_gqueue;
  VkQueue vk_pqueue;
  VkSurfaceKHR vk_surface;
  VkSwapchainKHR vk_swapchain;
} terrar_app_t;

terrar_app_state_t terrar_app_state_default(void);
terrar_app_metadata_t terrar_app_metadata_default(void);
terra_status_t terrar_app_config_new(const char **validation_layers,
                                     const char **device_extensions,
                                     uint32_t validation_layers_total,
                                     uint32_t device_extensions_total,
                                     terrar_app_config_t *out);
terrar_app_config_t terrar_app_config_default(void);

terra_status_t terrar_app_new(void *start, void *loop, void *cleanup,
                              terrar_app_metadata_t *meta,
                              terrar_app_config_t *conf, terrar_app_t *out);
terra_status_t terrar_app_new_wstate(terrar_app_state_t state, void *start,
                                     void *loop, void *cleanup,
                                     terrar_app_metadata_t *meta,
                                     terrar_app_config_t *conf,
                                     terrar_app_t *out);
terra_status_t terrar_app_run(terrar_app_t *app);
terra_status_t terrar_app_cleanup(terrar_app_t *app);

int terrar_app_should_close(terrar_app_t *app);
