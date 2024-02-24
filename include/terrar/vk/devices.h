#include <stdint.h>

#include <terra/status.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

typedef struct terrar_result {
  void *value;
  terrar_queue queue;
} terrar_result;

terra_status terrar_vk_create_application_info(terrar_app *app,
                                               VkApplicationInfo *out);
terra_status terrar_vk_create_instance_info(terrar_app *app,
                                            VkApplicationInfo *app_info,
                                            VkInstanceCreateInfo *out);
int terrar_vk_check_validation_layer_support(terrar_app *app);
terra_status terrar_vk_find_queue_families(VkPhysicalDevice device,
                                           VkSurfaceKHR surface,
                                           terrar_queue *out);
terra_status terrar_vk_rate_device(terrar_app *app, VkPhysicalDevice device,
                                   VkSurfaceKHR surface, terrar_queue *queue,
                                   uint32_t *out);
terra_status terrar_vk_get_physical_device(terrar_app *app, terrar_result *out);
terra_status terrar_vk_create_device_queue_info(uint32_t index, float *prio,
                                                VkDeviceQueueCreateInfo *out);
terra_status terrar_vk_create_device_features(VkPhysicalDeviceFeatures *out);
terra_status terrar_vk_create_device_info(
    VkDeviceQueueCreateInfo *queue_info, uint32_t queue_count,
    VkPhysicalDeviceFeatures *device_features, const char **device_extensions,
    uint32_t device_extension_count, VkDeviceCreateInfo *out);
