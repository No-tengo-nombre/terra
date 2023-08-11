#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <terra_utils/vendor/log.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

#include "vk_setup.h"

#define _MAX_LAYER_SIZE 128
#define _VALIDATION_LAYER_TOTAL 1
const char *_VALIDATION_LAYERS[_VALIDATION_LAYER_TOTAL] = {
    "VK_LAYER_KHRONOS_validation",
};

#define _DEVICE_EXTENSION_TOTAL 1
const char *_DEVICE_EXTENSIONS[_DEVICE_EXTENSION_TOTAL] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME,
};

VkApplicationInfo terrar_create_application_info(terrar_app *app) {
    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.apiVersion = VK_API_VERSION_1_3;
    app_info.applicationVersion =
        VK_MAKE_API_VERSION(1, app->version_major, app->version_minor, app->version_patch);
    app_info.engineVersion = VK_MAKE_API_VERSION(
        1, TERRAR_ENGINE_VERSION_MAJOR, TERRAR_ENGINE_VERSION_MINOR, TERRAR_ENGINE_VERSION_PATCH);
    app_info.pApplicationName = app->app_name;
    app_info.pEngineName = "Terra";
    app_info.pNext = NULL;
    return app_info;
}

VkInstanceCreateInfo terrar_create_instance_info(terrar_app *app, VkApplicationInfo *app_info) {
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
    instance_info.enabledLayerCount = _VALIDATION_LAYER_TOTAL;
    instance_info.ppEnabledLayerNames = _VALIDATION_LAYERS;
#else
    instance_info.enabledLayerCount = 0;
#endif
    return instance_info;
}

int terrar_check_validation_layer_support(void) {
    uint32_t layers;
    vkEnumerateInstanceLayerProperties(&layers, NULL);
    VkLayerProperties *properties = malloc(layers * sizeof(VkLayerProperties));
    if (properties == NULL) {
        return 0;
    }
    vkEnumerateInstanceLayerProperties(&layers, properties);

    for (int i = 0; i < _VALIDATION_LAYER_TOTAL; i++) {
        int found = 0;
        for (int j = 0; j < layers; j++) {
            if (strncmp(_VALIDATION_LAYERS[i], properties[j].layerName, _MAX_LAYER_SIZE) == 0) {
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

terrar_queue terrar_find_queue_families(VkPhysicalDevice device, VkSurfaceKHR surface) {
    terrar_queue indices;
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

int terrar_check_device_extensions(VkPhysicalDevice device, const char **target,
                                   size_t target_total) {
    uint32_t ext_total;
    vkEnumerateDeviceExtensionProperties(device, NULL, &ext_total, NULL);
    VkExtensionProperties *ext_props = malloc(ext_total * sizeof(VkExtensionProperties));
    vkEnumerateDeviceExtensionProperties(device, NULL, &ext_total, ext_props);

    int all_found = 1;
    for (int i = 0; i < target_total; i++) {
        int found = 0;
        for (int j = 0; j < ext_total; j++) {
            if (strncmp(target[i], ext_props[j].extensionName, _MAX_LAYER_SIZE) == 0) {
                found = 1;
                break;
            }
        }
        if (!found) {
            log_warn("Extension %s was not found", target[i]);
            all_found = 0;
            break;
        }
        log_debug("Extension %s was found", target[i]);
    }

    free(ext_props);
    return all_found;
}

uint32_t terrar_rate_device(VkPhysicalDevice device, VkSurfaceKHR surface, terrar_queue *queue) {
    VkPhysicalDeviceProperties props;
    VkPhysicalDeviceFeatures feats;
    vkGetPhysicalDeviceProperties(device, &props);
    vkGetPhysicalDeviceFeatures(device, &feats);

    int extensions_supported =
        terrar_check_device_extensions(device, _DEVICE_EXTENSIONS, _DEVICE_EXTENSION_TOTAL);
    if (extensions_supported) {
        log_debug("Device '%s' supports all extensions", props.deviceName);
    }

    if (!queue->gfound || !queue->pfound || !extensions_supported) {
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

terrar_result terrar_get_physical_device(terrar_app *app) {
    terrar_result result;
    VkPhysicalDevice device = VK_NULL_HANDLE;
    terrar_queue device_queue;
    uint32_t device_count = 0;
    vkEnumeratePhysicalDevices(app->vk_instance, &device_count, NULL);
    if (device_count == 0) {
        log_error("Failed to find GPUs that support Vulkan");
        result.status = TERRA_STATUS_FAILURE;
        return result;
    }
    VkPhysicalDevice *devices = malloc(device_count * sizeof(VkPhysicalDevice));
    if (devices == NULL) {
        log_error("Could not allocate enough memory for the devices");
        result.status = TERRA_STATUS_FAILURE;
        return result;
    }
    vkEnumeratePhysicalDevices(app->vk_instance, &device_count, devices);

    int32_t score = -2;
    int32_t new_score = 0;
    for (int i = 0; i < device_count; i++) {
        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(devices[i], &props);
        log_info("Evaluating device '%s'", props.deviceName);
        device_queue = terrar_find_queue_families(devices[i], app->vk_surface);
        new_score = terrar_rate_device(devices[i], app->vk_surface, &device_queue);
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
        result.status = TERRA_STATUS_FAILURE;
    } else {
        result.status = TERRA_STATUS_SUCCESS;
        result.value = device;
        result.queue = device_queue;

        VkPhysicalDeviceProperties props;
        vkGetPhysicalDeviceProperties(device, &props);
        log_info("Using device %s", props.deviceName);
    }
    free(devices);
    return result;
}

VkDeviceQueueCreateInfo terrar_create_device_queue_info(uint32_t index, float *prio) {
    VkDeviceQueueCreateInfo queue_info;
    queue_info.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queue_info.queueCount = 1;
    queue_info.queueFamilyIndex = index;
    queue_info.pQueuePriorities = prio;
    queue_info.pNext = NULL;
    queue_info.flags = 0;
    return queue_info;
}

VkPhysicalDeviceFeatures terrar_create_device_features(void) {
    VkPhysicalDeviceFeatures device_features = {VK_FALSE};
    device_features.geometryShader = VK_TRUE;
    return device_features;
}

VkDeviceCreateInfo terrar_create_device_info(VkDeviceQueueCreateInfo *queue_info,
                                             uint32_t queue_count,
                                             VkPhysicalDeviceFeatures *device_features) {
    VkDeviceCreateInfo device_info;
    device_info.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    device_info.queueCreateInfoCount = 1;
    device_info.pQueueCreateInfos = queue_info;
    device_info.pEnabledFeatures = device_features;

    device_info.pNext = NULL;
    device_info.flags = 0;

    // The validation checks are done, so it can be assumed that the requested features
    // are available
    device_info.enabledLayerCount = 0;
    device_info.ppEnabledLayerNames = NULL;

    device_info.enabledExtensionCount = _DEVICE_EXTENSION_TOTAL;
    device_info.ppEnabledExtensionNames = _DEVICE_EXTENSIONS;
    return device_info;
}
