#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <terra_utils/vendor/log.h>
#include <terrar/app.h>

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

#define VALIDATION_LAYER_TOTAL 1
const char *REQUESTED_VALIDATION_LAYERS[VALIDATION_LAYER_TOTAL] = {
    "VK_LAYER_KHRONOS_validation",
};

VkApplicationInfo create_application_info(void) {
    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.apiVersion = VK_API_VERSION_1_3;
    app_info.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    app_info.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    app_info.pApplicationName = "Terra - Raw Vulkan Example";
    app_info.pEngineName = "No engine";
    app_info.pNext = NULL;
    return app_info;
}

VkInstanceCreateInfo create_instance_info(VkApplicationInfo *app_info) {
    VkInstanceCreateInfo instance_info;
    uint32_t extension_count = 0;
    const char **extensions;
    extensions = glfwGetRequiredInstanceExtensions(&extension_count);
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = app_info;
    instance_info.enabledExtensionCount = extension_count;
    instance_info.ppEnabledExtensionNames = extensions;
    instance_info.flags = 0;
    instance_info.pNext = NULL;
#ifndef NDEBUG
    instance_info.enabledLayerCount = VALIDATION_LAYER_TOTAL;
    instance_info.ppEnabledLayerNames = REQUESTED_VALIDATION_LAYERS;
#else
    instance_info.enabledLayerCount = 0;
#endif
    return instance_info;
}

int check_validation_layer_support(void) {
    uint32_t layers;
    vkEnumerateInstanceLayerProperties(&layers, NULL);
    VkLayerProperties *properties = malloc(layers * sizeof(VkLayerProperties));
    if (properties == NULL) {
        return 0;
    }
    vkEnumerateInstanceLayerProperties(&layers, properties);

    for (int i = 0; i < VALIDATION_LAYER_TOTAL; i++) {
        int found = 0;
        for (int j = 0; j < layers; j++) {
            if (strcmp(REQUESTED_VALIDATION_LAYERS[i], properties[j].layerName) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            return 0;
        }
    }
    return 1;
}

typedef struct queue_indices_t {
    uint32_t family;
    int found;
} queue_indices_t;

queue_indices_t find_queue_families(VkPhysicalDevice device) {
    queue_indices_t indices;
    indices.found = 0;

    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, NULL);
    VkQueueFamilyProperties *props = malloc(count * sizeof(VkQueueFamilyProperties));
    if (props == NULL) {
        return indices;
    }
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, props);

    for (int i = 0; i < count; i++) {
        if (props[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.family = i;
            indices.found = 1;
        }
    }

    return indices;
}

uint32_t rate_device(VkPhysicalDevice device) {
    queue_indices_t queue = find_queue_families(device);
    if (!queue.found) {
        return -1;
    }
    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceFeatures feats;
    vkGetPhysicalDeviceProperties(device, &props);
    vkGetPhysicalDeviceFeatures(device, &feats);

    uint32_t score = 0;
    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    score += props.limits.maxImageDimension2D;
    return score;
}

typedef struct result_t {
    void *value;
    uint32_t status;
} result_t;

result_t get_physical_device(terrar_app_t *app) {
    result_t result;
    VkPhysicalDevice device = VK_NULL_HANDLE;
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(app->vk_instance, &device_count, NULL);
    if (device_count == 0) {
        log_error("failed to find GPUs that support Vulkan");
        result.status = STATUS_FAILURE;
        return result;
    }
    VkPhysicalDevice *devices = malloc(device_count * sizeof(VkPhysicalDevice));
    if (devices == NULL) {
        log_error("could not allocate enough memory for the devices");
        result.status = STATUS_FAILURE;
        return result;
    }
    vkEnumeratePhysicalDevices(app->vk_instance, &device_count, devices);

    uint32_t score = -2;
    uint32_t new_score = 0;
    for (int i = 0; i < device_count; i++) {
        new_score = rate_device(devices[i]);
        if (score == -2) {
            device = devices[i];
            score = new_score;
            continue;
        }
        if (new_score > score) {
            device = devices[i];
            score = new_score;
        }
    }
    if (score == -1) {
        log_error("no devices are suitable for the application");
        result.status = STATUS_FAILURE;
    } else {
        result.status = STATUS_SUCCESS;
        result.value = device;
    }
    return result;
}

VkDeviceQueueCreateInfo create_device_queue_info(terrar_app_t *app, float *prio) {
    // We can assume the device is valid
    queue_indices_t queue = find_queue_families(app->vk_pdevice);

    VkDeviceQueueCreateInfo queue_info;
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.queueCount = 1;
    queue_info.queueFamilyIndex = queue.family;
    queue_info.pQueuePriorities = prio;
    queue_info.pNext = NULL;
    queue_info.flags = 0;
    return queue_info;
}

VkPhysicalDeviceFeatures create_device_features(void) {
    VkPhysicalDeviceFeatures device_features = {VK_FALSE};
    device_features.geometryShader = VK_TRUE;
    return device_features;
}

VkDeviceCreateInfo create_device_info(VkDeviceQueueCreateInfo *queue_info,
                                      VkPhysicalDeviceFeatures *device_features) {
    VkDeviceCreateInfo device_info;
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = queue_info;
    device_info.pEnabledFeatures = device_features;

    device_info.pNext = NULL;
    device_info.enabledExtensionCount = 0;
    device_info.enabledLayerCount = 0;
    device_info.flags = 0;
    device_info.ppEnabledExtensionNames = NULL;
    device_info.ppEnabledLayerNames = NULL;
    return device_info;
}

status_t start(void *app) {
    terrar_app_t *app_p = (terrar_app_t *)app;

    glfwInit();

    // Initialize the window
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    GLFWwindow *window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Terra - Raw Vulkan example", NULL, NULL);
    app_p->glfw_window = window;

    // Initialize Vulkan
#ifndef NDEBUG
    log_debug("checking validation layers support");
    if (!check_validation_layer_support()) {
        log_error("validation layers required but not supported");
        return STATUS_FAILURE;
    }
#endif
    log_debug("creating app info");
    VkApplicationInfo app_info = create_application_info();
    log_debug("creating instance info");
    VkInstanceCreateInfo instance_info = create_instance_info(&app_info);
    log_debug("creating instance");
    if (vkCreateInstance(&instance_info, NULL, &app_p->vk_instance) != VK_SUCCESS) {
        log_error("could not create Vulkan instance");
        return STATUS_FAILURE;
    }
    log_debug("choosing physical device");
    result_t result = get_physical_device(app_p);
    if (result.status == STATUS_FAILURE) {
        log_error("could not find a suitable device");
        return STATUS_FAILURE;
    }
    app_p->vk_pdevice = result.value;

    log_debug("creating device queue info");
    float queue_prio = 1.0f;
    VkDeviceQueueCreateInfo queue_info = create_device_queue_info(app_p, &queue_prio);
    log_debug("creating device features");
    VkPhysicalDeviceFeatures device_features = create_device_features();
    log_debug("creating logical device info");
    VkDeviceCreateInfo device_info = create_device_info(&queue_info, &device_features);
    if (vkCreateDevice(app_p->vk_pdevice, &device_info, NULL, &app_p->vk_ldevice) != VK_SUCCESS) {
        log_error("could not create logical device");
        return STATUS_FAILURE;
    }
    log_debug("retrieving graphics queue");
    vkGetDeviceQueue(app_p->vk_ldevice, find_queue_families(app_p->vk_pdevice).family, 0,
                     &app_p->vk_gqueue);

    return STATUS_SUCCESS;
}

status_t loop(void *app) {
    terrar_app_t *app_p = (terrar_app_t *)app;

    if (glfwWindowShouldClose(app_p->glfw_window)) {
        log_info("terminating program loop");
        return STATUS_EXIT;
    }

    glfwPollEvents();

    return STATUS_SUCCESS;
}

status_t cleanup(void *app) {
    terrar_app_t *app_p = (terrar_app_t *)app;

    // Device must be destroyed before instance
    vkDestroyDevice(app_p->vk_ldevice, NULL);
    vkDestroyInstance(app_p->vk_instance, NULL);
    glfwDestroyWindow(app_p->glfw_window);
    glfwTerminate();

    return STATUS_SUCCESS;
}

int main(void) {
    terrar_app_t app = terrar_app_new(&start, &loop, &cleanup);
    return terrar_app_run(&app);
}
