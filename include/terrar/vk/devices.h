#pragma once

#include <stdint.h>

#include <terra/status.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

typedef struct terrar_result {
  void *value;
  terrar_queue_t queue;
} terrar_result_t;

terra_status_t terrar_vk_create_application_info(terrar_app_t *app,
                                               VkApplicationInfo *out);
terra_status_t terrar_vk_create_instance_info(terrar_app_t *app,
                                            VkApplicationInfo *app_info,
                                            VkInstanceCreateInfo *out);
int terrar_vk_check_validation_layer_support(terrar_app_t *app);
terra_status_t terrar_vk_find_queue_families(VkPhysicalDevice device,
                                           VkSurfaceKHR surface,
                                           terrar_queue_t *out);
terra_status_t terrar_vk_rate_device(terrar_app_t *app, VkPhysicalDevice device,
                                   VkSurfaceKHR surface, terrar_queue_t *queue,
                                   uint32_t *out);
terra_status_t terrar_vk_get_physical_device(terrar_app_t *app,
                                           terrar_result_t *out);
terra_status_t terrar_vk_create_device_queue_info(uint32_t index, float *prio,
                                                VkDeviceQueueCreateInfo *out);
terra_status_t terrar_vk_create_device_features(VkPhysicalDeviceFeatures *out);
terra_status_t terrar_vk_create_device_info(
    VkDeviceQueueCreateInfo *queue_info, uint32_t queue_count,
    VkPhysicalDeviceFeatures *device_features, const char **device_extensions,
    uint32_t device_extension_count, VkDeviceCreateInfo *out);
