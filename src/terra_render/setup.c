#include <terra/status.h>
#include <terra_utils/vendor/log.h>
#include <terrar/setup.h>
#include <terrar/vulkan.h>

#include "vk_setup.h"

status_t init_window(terrar_app_t *app) {
    log_debug("Initializing GLFW and window");
    glfwInit();
    glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
    glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
    const char *title = app->app_name;
    if (app->window_title != NULL) {
        title = app->window_title;
    }
    GLFWwindow *window = glfwCreateWindow(app->window_width, app->window_height, title, NULL, NULL);
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
    // TODO: Figure out a better way to store the queue info
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
