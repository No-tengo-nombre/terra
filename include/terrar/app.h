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
} terrar_app_state;

typedef struct terrar_queue {
    uint32_t gfamily;
    uint32_t pfamily;
    int gfound;
    int pfound;
} terrar_queue;

typedef struct terrar_app {
    terra_status (*start)(struct terrar_app *);
    terra_status (*loop)(struct terrar_app *);
    terra_status (*cleanup)(struct terrar_app *);

    /* Application metadata */
    uint32_t version_major;
    uint32_t version_minor;
    uint32_t version_patch;
    const char *window_title;
    const char *app_name;
    uint32_t window_width;
    uint32_t window_height;

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

terrar_app terrar_app_new(void *start, void *loop, void *cleanup, const char *app_name);
terrar_app terrar_app_new_wstate(terrar_app_state state, void *start, void *loop, void *cleanup,
                                   const char *app_name);
terra_status terrar_app_run(terrar_app *app);
terra_status terrar_app_cleanup(terrar_app *app);

int terrar_app_should_close(terrar_app *app);
