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
    instance_info.enabledLayerCount = 0;
    instance_info.pNext = NULL;
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
    log_debug("creating app info");
    VkApplicationInfo app_info = create_application_info();

    log_debug("creating instance info");
    VkInstanceCreateInfo instance_info = create_instance_info(&app_info);

    log_debug("creating instance");
    if (vkCreateInstance(&instance_info, NULL, &app_p->vk_instance) != VK_SUCCESS) {
        return STATUS_FAILURE;
    }

    #ifndef NDEBUG
    log_debug("checking validation layers support");
    if (!check_validation_layer_support()) {
        return STATUS_FAILURE;
    }
    #endif

    return STATUS_SUCCESS;
}

status_t loop(void *app) {
    terrar_app_t *app_p = (terrar_app_t *)app;

    if (glfwWindowShouldClose(app_p->glfw_window)) {
        return STATUS_EXIT;
    }

    glfwPollEvents();

    return STATUS_SUCCESS;
}

status_t cleanup(void *app) {
    terrar_app_t *app_p = (terrar_app_t *)app;

    vkDestroyInstance(app_p->vk_instance, NULL);
    glfwDestroyWindow(app_p->glfw_window);
    glfwTerminate();

    return STATUS_SUCCESS;
}

int main(void) {
    terrar_app_t app = terrar_app_new(&start, &loop, &cleanup);
    return terrar_app_run(&app);
}
