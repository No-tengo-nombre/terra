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
} terrar_app_state;

typedef struct terrar_queue {
    uint32_t gfamily;
    uint32_t pfamily;
    int gfound;
    int pfound;
} terrar_queue;

typedef struct terrar_app_metadata {
    uint32_t vmajor;
    uint32_t vminor;
    uint32_t vpatch;
    const char *app_name;
    const char *window_title;
    uint32_t window_width;
    uint32_t window_height;
} terrar_app_metadata;

typedef struct terrar_app_config {
    uint32_t validation_layers_total;
    uint32_t device_extensions_total;
    const char **validation_layers;
    const char **device_extensions;

    VkFormat surface_format;
    VkColorSpaceKHR color_space;
    VkPresentModeKHR present_mode;
} terrar_app_config;

typedef struct terrar_app {
    terra_status (*start)(struct terrar_app *);
    terra_status (*loop)(struct terrar_app *);
    terra_status (*cleanup)(struct terrar_app *);

    terrar_app_metadata *meta;
    terrar_app_config *conf;

    terrar_app_state state;
    void *glfw_window;

    /* Vulkan attributes */

    terrar_queue vk_qinfo;
    VkInstance vk_instance;
    VkPhysicalDevice vk_pdevice;
    VkDevice vk_ldevice;
    VkQueue vk_gqueue;
    VkQueue vk_pqueue;
    VkSurfaceKHR vk_surface;
} terrar_app;

terrar_app_state terrar_state_default(void);
terrar_app_metadata terrar_metadata_default(void);
terrar_app_config terrar_config_new(const char **validation_layers, const char **device_extensions, uint32_t validation_layers_total, uint32_t device_extensions_total);
terrar_app_config terrar_config_default(void);

terrar_app terrar_app_new(void *start, void *loop, void *cleanup, terrar_app_metadata *meta,
                          terrar_app_config *conf);
terrar_app terrar_app_new_wstate(terrar_app_state state, void *start, void *loop, void *cleanup,
                                 terrar_app_metadata *meta, terrar_app_config *conf);
terra_status terrar_app_run(terrar_app *app);
terra_status terrar_app_cleanup(terrar_app *app);

int terrar_app_should_close(terrar_app *app);
