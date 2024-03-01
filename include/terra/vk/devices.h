#pragma once

#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

typedef struct terra_result {
  void *value;
  terra_queue_t queue;
} terra_result_t;

terra_status_t terra_vk_create_application_info(
    terra_app_t *app, VkApplicationInfo *out
);
terra_status_t terra_vk_create_instance_info(
    terra_app_t *app, VkApplicationInfo *app_info, VkInstanceCreateInfo *out
);
int terra_vk_check_validation_layer_support(terra_app_t *app);
terra_status_t terra_vk_find_queue_families(
    terra_app_t *app,
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    terra_queue_t *out
);
terra_status_t terra_vk_rate_device(
    terra_app_t *app,
    VkPhysicalDevice device,
    VkSurfaceKHR surface,
    terra_queue_t *queue,
    uint32_t *out
);
terra_status_t terra_vk_get_physical_device(
    terra_app_t *app, terra_result_t *out
);
terra_status_t terra_vk_create_device_queue_info(
    uint32_t index, float *prio, VkDeviceQueueCreateInfo *out
);
terra_status_t terra_vk_create_device_features(VkPhysicalDeviceFeatures *out);
terra_status_t terra_vk_create_device_info(
    VkDeviceQueueCreateInfo *queue_info,
    uint32_t queue_count,
    VkPhysicalDeviceFeatures *device_features,
    const char **device_extensions,
    uint32_t device_extension_count,
    VkDeviceCreateInfo *out
);
