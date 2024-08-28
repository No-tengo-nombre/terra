#pragma once

#include "data/vector.h"
#include "setup.h"
#include "vulkan.h"

#include <stdint.h>
#include <stdio.h>
#include <terra/status.h>
#include <terra/vma.h>
#include <time.h>

#define TERRA_ENGINE_VERSION_MAJOR 1
#define TERRA_ENGINE_VERSION_MINOR 0
#define TERRA_ENGINE_VERSION_PATCH 0
#define TERRA_ENGINE_NAME          "Terra"

#define _TERRA_LOGFILE_MAX 64

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

// Forward declarations

typedef struct terra_buffer terra_buffer_t;
typedef struct terra_pipeline_fnames terra_pipeline_fnames_t;
typedef struct terra_vk_pipeline_params terra_vk_pipeline_params_t;
typedef struct terra_vk_descriptor_set terra_vk_descriptor_set_t;
typedef struct terra_init_params terra_init_params_t;

// Debug declarations

#ifndef NDEBUG
typedef struct _idebug_heap_info {
  void *addr;
  size_t size;
  char file[FILENAME_MAX];
  int line;
  struct _idebug_heap_info *next;
} _idebug_heap_info_t;

_idebug_heap_info_t *heapinfo_new(terra_app_t *app);
void heapinfo_free_node(_idebug_heap_info_t *node);
void heapinfo_clean(terra_app_t *app);
void heapinfo_push(
    terra_app_t *app, void *addr, size_t size, const char *file, int line
);
size_t heapinfo_popaddr(terra_app_t *app, void *addr);
size_t heapinfo_count(terra_app_t *app);
#endif

// Application declarations

typedef enum terra_dims {
  TERRA_2D = 2,
  TERRA_3D = 3,
} terra_dims_t;

typedef struct terra_app_state {
  uint64_t i;
  int should_close;
  uint32_t vk_frame;
  int fb_resized;
  int64_t start_msec;
  int64_t curr_msec;
  int64_t time_msec;
  double delta_sec;
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
  uint32_t instance_extensions_total;
  const char **validation_layers;
  const char **device_extensions;
  const char **instance_extensions;

  terra_init_params_t init_params;
  terra_vk_pipeline_params_t *pipelines_params;
  terra_pipeline_fnames_t *pipelines_fnames;
  size_t pipelines_count;

  terra_vk_descriptor_set_t *descriptor_sets;
  size_t descriptor_sets_count;

  uint32_t vk_version;
  VkIndexType vk_idx_type;

  VkFormat surface_format;
  VkColorSpaceKHR color_space;
  VkPresentModeKHR present_mode;
  uint32_t image_array_layers;
  VkCompositeAlphaFlagBitsKHR composite_alpha;
  uint8_t clipped;
  VkCommandPoolCreateFlags command_pool_flags;
  uint32_t max_frames_in_flight;
  terra_dims_t dims;

  /* GLFW configurations */

  int resizable;

  /* Timeouts */

  uint64_t in_flight_fence_timeout;
  uint64_t img_acq_timeout;

  /* Logging configuration */

  const char *log_dir;
  int log_stdlvl;
  int log_filelvl;
} terra_app_config_t;

typedef terra_status_t(terra_start_ft)(terra_app_t *);
typedef terra_status_t(terra_loop_ft)(terra_app_t *);
typedef terra_status_t(terra_clean_ft)(terra_app_t *);

typedef struct terra_app {
  terra_start_ft *start;
  terra_loop_ft *loop;
  terra_clean_ft *cleanup;

  terra_app_metadata_t *meta;
  terra_app_config_t *conf;
  terra_app_state_t state;

  void *glfw_window;
  VmaAllocator vma_alloc;
  terra_init_params_t init_params;

  /* Data handles */

  char log_filename[FILENAME_MAX + _TERRA_LOGFILE_MAX];
  FILE *log_file;

  /* Data containers */

  terra_vector_t shapes;

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
  VkDescriptorSetLayout vk_camera_layout;
  VkPipelineLayout vk_layout;
  VkRenderPass vk_render_pass;
  VkPipeline vk_pipeline;
  VkCommandPool vk_commands;
  VkCommandPool vk_mem_commands;
  VkCommandBuffer *vk_command_buffers;

  VkImage *vk_images;
  VkImageView *vk_image_views;
  VkFramebuffer *vk_framebuffers;
  terra_buffer_t *ubos;
  uint32_t vk_images_count;

  /* Synchronization variables */

  VkSemaphore *vk_img_available_S;
  VkSemaphore *vk_render_finished_S;
  VkFence *vk_in_flight_F;

#ifndef NDEBUG
  /* Internal debug information */

  VkDebugUtilsMessengerEXT _idebug_messenger;
  int64_t _idebug_malloced_total;
  size_t _idebug_malloced_size;
  _idebug_heap_info_t *_idebug_heap_head;
#endif
} terra_app_t;

// Constants

extern const char *DEFAULT_VALIDATION_LAYERS[];
extern const char *DEFAULT_DEVICE_EXTENSIONS[];
extern const char *DEFAULT_INSTANCE_EXTENSIONS[];
extern const size_t DEFAULT_VALIDATION_LAYERS_TOTAL;
extern const size_t DEFAULT_DEVICE_EXTENSIONS_TOTAL;
extern const size_t DEFAULT_INSTANCE_EXTENSIONS_TOTAL;
extern const uint32_t DEFAULT_MAX_FRAMES_IN_FLIGHT;
extern const terra_app_metadata_t TERRA_APP_METADATA_DEFAULT;
extern const terra_app_config_t TERRA_APP_CONFIG_DEFAULT;

terra_app_state_t terra_app_state_default(void);
terra_app_metadata_t terra_app_metadata_default(void);
terra_status_t terra_app_config_new(
    const char **validation_layers,
    const char **device_extensions,
    const char **instance_extensions,
    uint32_t validation_layers_total,
    uint32_t device_extensions_total,
    uint32_t instance_extensions_total,
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
terra_status_t terra_app_set_frames_in_flight(
    terra_app_t *app, uint32_t new_size
);

terra_status_t terra_app_record_cmd_buffer(terra_app_t *app, uint32_t idx);
terra_status_t terra_app_draw(terra_app_t *app);
int terra_app_should_close(terra_app_t *app);
void terra_app_poll_events(terra_app_t *app);

terra_status_t terra_app_cleanup_swapchain(
    terra_app_t *app, VkSwapchainKHR *sc
);
terra_status_t terra_app_cleanup(terra_app_t *app);

VKAPI_ATTR VkBool32 VKAPI_CALL terra_app_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
    VkDebugUtilsMessageTypeFlagsEXT msg_type,
    const VkDebugUtilsMessengerCallbackDataEXT *cb_data,
    void *user_data
);
void terra_app_fb_resize_callback(GLFWwindow *window, int width, int height);

#ifdef __cplusplus
}
#endif
