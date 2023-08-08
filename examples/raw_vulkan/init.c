#include <terra/status.h>
#include <terra_utils/vendor/log.h>
#include <terrar/vulkan.h>

#include "init.h"

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

#define VALIDATION_LAYER_TOTAL 1
const char *REQUESTED_VALIDATION_LAYERS[VALIDATION_LAYER_TOTAL] = {
    "VK_LAYER_KHRONOS_validation",
};

#define DEVICE_EXTENSION_TOTAL 1
const char *REQUESTED_DEVICE_EXTENSIONS[DEVICE_EXTENSION_TOTAL] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

VkApplicationInfo create_application_info(terrar_app_t *app) {
    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.apiVersion = VK_API_VERSION_1_3;
    app_info.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    app_info.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    app_info.pApplicationName = app->app_name;
    app_info.pEngineName = "Terra";
    app_info.pNext = NULL;
    return app_info;
}

VkInstanceCreateInfo create_instance_info(terrar_app_t *app, VkApplicationInfo *app_info) {
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
    free(properties);
    return 1;
}

terrar_queue_t find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
    terrar_queue_t indices;
    indices.gfound = 0;
    indices.pfound = 0;
    VkPhysicalDeviceProperties dev_props;
    vkGetPhysicalDeviceProperties(device, &dev_props);

    uint32_t count = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, NULL);
    VkQueueFamilyProperties *qprops = malloc(count * sizeof(VkQueueFamilyProperties));
    if (qprops == NULL) {
        return indices;
    }
    vkGetPhysicalDeviceQueueFamilyProperties(device, &count, qprops);

    int pfound = 0;
    for (int i = 0; i < count; i++) {
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &pfound);
        unsigned char qflags_bin[5];
        for (int b = 0; b < 4; b++) {
            qflags_bin[b] = (qprops[i].queueFlags >> (3 - b)) & 1 ? '1' : '0';
        }
        qflags_bin[4] = '\0';
        log_debug("Queues %s %ix%s", dev_props.deviceName, qprops[i].queueCount, qflags_bin);
        if (pfound) {
            indices.pfamily = i;
            indices.pfound = 1;
        }
        if (qprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT) {
            indices.gfamily = i;
            indices.gfound = 1;
        }
    }
    free(qprops);
    return indices;
}

uint32_t rate_device(VkPhysicalDevice device, VkSurfaceKHR surface, terrar_queue_t *queue) {
    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceFeatures feats;
    vkGetPhysicalDeviceProperties(device, &props);
    vkGetPhysicalDeviceFeatures(device, &feats);

    if (!queue->gfound || !queue->pfound) {
        log_debug("Device '%s' not suitable", props.deviceName);
        return -1;
    }

    uint32_t score = 0;
    if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
        score += 1000;
    }
    score += props.limits.maxImageDimension2D;
    log_debug("Device '%s' score: %I32u", props.deviceName, score);
    return score;
}

typedef struct result_t {
    void *value;
    terrar_queue_t queue;
    uint32_t status;
} result_t;

result_t get_physical_device(terrar_app_t *app) {
    result_t result;
    VkPhysicalDevice device = VK_NULL_HANDLE;
    terrar_queue_t device_queue;
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(app->vk_instance, &device_count, NULL);
    if (device_count == 0) {
        log_error("Failed to find GPUs that support Vulkan");
        result.status = STATUS_FAILURE;
        return result;
    }
    VkPhysicalDevice *devices = malloc(device_count * sizeof(VkPhysicalDevice));
    if (devices == NULL) {
        log_error("Could not allocate enough memory for the devices");
        result.status = STATUS_FAILURE;
        return result;
    }
    vkEnumeratePhysicalDevices(app->vk_instance, &device_count, devices);

    int32_t score = -2;
    int32_t new_score = 0;
    for (int i = 0; i < device_count; i++) {
        device_queue = find_queue_families(devices[i], app->vk_surface);
        new_score = rate_device(devices[i], app->vk_surface, &device_queue);
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
        log_error("No devices are suitable for the application");
        result.status = STATUS_FAILURE;
    } else {
        result.status = STATUS_SUCCESS;
        result.value = device;
        result.queue = device_queue;

        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);
        log_info("Using device %s", props.deviceName);
    }
    free(devices);
    return result;
}

VkDeviceQueueCreateInfo create_device_queue_info(uint32_t index, float *prio) {
    VkDeviceQueueCreateInfo queue_info;
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.queueCount = 1;
    queue_info.queueFamilyIndex = index;
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

VkDeviceCreateInfo create_device_info(VkDeviceQueueCreateInfo *queue_info, uint32_t queue_count,
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

status_t init_window(terrar_app_t *app) {
    log_debug("Initializing GLFW and window");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    const char *title = app->app_name;
    if (app->window_title != NULL) {
        title = app->window_title;
    }
    GLFWwindow *window =
        glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, title, NULL, NULL);
    app->glfw_window = window;
    return STATUS_SUCCESS;
}

status_t init_instance(terrar_app_t *app) {
    // Initialize Vulkan
#ifndef NDEBUG
    log_debug("Checking validation layers support");
    if (!check_validation_layer_support()) {
        log_error("Validation layers required but not supported");
        return STATUS_FAILURE;
    }
#endif
    log_debug("Creating app info");
    VkApplicationInfo app_info = create_application_info(app);
    log_debug("Creating instance info");
    VkInstanceCreateInfo instance_info = create_instance_info(app, &app_info);
    log_debug("Creating instance");
    if (vkCreateInstance(&instance_info, NULL, &app->vk_instance) != VK_SUCCESS) {
        log_error("Could not create Vulkan instance");
        return STATUS_FAILURE;
    }
    return STATUS_SUCCESS;
}

status_t create_render_surface(terrar_app_t *app) {
    log_debug("Creating render surface");
    if (glfwCreateWindowSurface(app->vk_instance, app->glfw_window, NULL, &app->vk_surface) !=
        VK_SUCCESS) {
        log_error("Failed to create render surface");
        return STATUS_FAILURE;
    }
    return STATUS_SUCCESS;
}

status_t choose_pdevice(terrar_app_t *app) {
    log_debug("Choosing physical device");
    result_t result = get_physical_device(app);
    if (result.status == STATUS_FAILURE) {
        log_error("Could not find a suitable physical device");
        return STATUS_FAILURE;
    }
    app->vk_pdevice = result.value;
    app->vk_qinfo = result.queue;
    return STATUS_SUCCESS;
}

status_t create_ldevice(terrar_app_t *app) {
    log_debug("Creating device queue info");
    float queue_prio = 1.0f;
    VkDeviceQueueCreateInfo queue_infos[2];
    uint32_t queue_count = 1;
    if (app->vk_qinfo.gfamily == app->vk_qinfo.pfamily) {
        queue_infos[0] = create_device_queue_info(app->vk_qinfo.gfamily, &queue_prio);
    } else {
        queue_infos[0] = create_device_queue_info(app->vk_qinfo.gfamily, &queue_prio);
        queue_infos[1] = create_device_queue_info(app->vk_qinfo.pfamily, &queue_prio);
        queue_count = 2;
    }
    log_debug("Using %i queues", queue_count);
    log_debug("Creating device features");
    VkPhysicalDeviceFeatures device_features = create_device_features();
    log_debug("Creating logical device info");
    VkDeviceCreateInfo device_info = create_device_info(queue_infos, queue_count, &device_features);
    if (vkCreateDevice(app->vk_pdevice, &device_info, NULL, &app->vk_ldevice) != VK_SUCCESS) {
        log_error("Could not create logical device");
        return STATUS_FAILURE;
    }
    return STATUS_SUCCESS;
}

status_t retrieve_device_queue(terrar_app_t *app) {
    log_debug("Retrieving graphics queue");
    vkGetDeviceQueue(app->vk_ldevice, app->vk_qinfo.gfamily, 0, &app->vk_gqueue);
    vkGetDeviceQueue(app->vk_ldevice, app->vk_qinfo.pfamily, 0, &app->vk_pqueue);
    return STATUS_SUCCESS;
}
