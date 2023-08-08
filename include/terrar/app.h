#pragma once

#include <stdint.h>

#include <vulkan/vulkan.h>

#include <terra/status.h>

typedef struct terrar_app_state_t {
    uint64_t i;
} terrar_app_state_t;

typedef struct terrar_queue_t {
    uint32_t gfamily;
    uint32_t pfamily;
    int gfound;
    int pfound;
} terrar_queue_t;

typedef struct terrar_app_t {
    status_t (*start)(void *);
    status_t (*loop)(void *);
    status_t (*cleanup)(void *);

    terrar_app_state_t state;
    void *glfw_window;

    /* Vulkan attributes */

    terrar_queue_t vk_queues;
    VkInstance vk_instance;
    VkPhysicalDevice vk_pdevice;
    VkDevice vk_ldevice;
    VkQueue vk_gqueue;
    VkQueue vk_pqueue;
    VkSurfaceKHR vk_surface;
} terrar_app_t;

terrar_app_state_t terrar_state_default(void);

terrar_app_t terrar_app_new(void *start, void *loop, void *cleanup);
terrar_app_t terrar_app_new_wstate(terrar_app_state_t state, void *start, void *loop, void *cleanup);
status_t terrar_app_run(terrar_app_t *app);
