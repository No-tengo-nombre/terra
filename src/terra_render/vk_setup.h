#include <stdint.h>

#include <terrar/app.h>
#include <terrar/vulkan.h>

typedef struct result_t {
    void *value;
    terrar_queue_t queue;
    uint32_t status;
} result_t;

VkApplicationInfo create_application_info(terrar_app_t *);
VkInstanceCreateInfo create_instance_info(terrar_app_t *, VkApplicationInfo *);
int check_validation_layer_support(void);
terrar_queue_t find_queue_families(VkPhysicalDevice, VkSurfaceKHR);
uint32_t rate_device(VkPhysicalDevice, VkSurfaceKHR, terrar_queue_t *);
result_t get_physical_device(terrar_app_t *);
VkDeviceQueueCreateInfo create_device_queue_info(uint32_t, float *);
VkPhysicalDeviceFeatures create_device_features(void);
VkDeviceCreateInfo create_device_info(VkDeviceQueueCreateInfo *, uint32_t,
                                      VkPhysicalDeviceFeatures *);
