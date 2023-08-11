#include <stdint.h>

#include <terrar/app.h>
#include <terrar/vulkan.h>

typedef struct terrar_result {
    void *value;
    terrar_queue queue;
    uint32_t status;
} terrar_result;

typedef struct terrar_swapchain_details {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR *formats;
    VkPresentModeKHR *modes;
    uint32_t format_count;
    uint32_t mode_count;
} terrar_swapchain_details;

VkApplicationInfo terrar_create_application_info(terrar_app *);
VkInstanceCreateInfo terrar_create_instance_info(terrar_app *, VkApplicationInfo *);
int terrar_check_validation_layer_support(void);
terrar_queue terrar_find_queue_families(VkPhysicalDevice, VkSurfaceKHR);
uint32_t terrar_rate_device(VkPhysicalDevice, VkSurfaceKHR, terrar_queue *);
terrar_result terrar_get_physical_device(terrar_app *);
VkDeviceQueueCreateInfo terrar_create_device_queue_info(uint32_t, float *);
VkPhysicalDeviceFeatures terrar_create_device_features(void);
VkDeviceCreateInfo terrar_create_device_info(VkDeviceQueueCreateInfo *, uint32_t,
                                             VkPhysicalDeviceFeatures *, const char **, uint32_t);
