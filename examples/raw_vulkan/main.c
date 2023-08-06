#include <stdio.h>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include <terra_utils/vendor/log.h>
#include <terrar/app.h>

const uint32_t WINDOW_WIDTH = 800;
const uint32_t WINDOW_HEIGHT = 600;

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
    VkApplicationInfo app_info;
    app_info.sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
    app_info.apiVersion = VK_API_VERSION_1_3;
    app_info.applicationVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    app_info.engineVersion = VK_MAKE_API_VERSION(1, 1, 0, 0);
    app_info.pApplicationName = "Terra - Raw Vulkan Example";
    app_info.pEngineName = "No engine";
    app_info.pNext = NULL;

    log_debug("creating instance info");
    VkInstanceCreateInfo instance_info;
    uint32_t extension_count = 0;
    const char **extensions;
    extensions = glfwGetRequiredInstanceExtensions(&extension_count);
    instance_info.sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
    instance_info.pApplicationInfo = &app_info;
    instance_info.enabledExtensionCount = extension_count;
    instance_info.ppEnabledExtensionNames = extensions;
    instance_info.enabledLayerCount = 0;
    instance_info.pNext = NULL;

    log_debug("creating instance");
    if (vkCreateInstance(&instance_info, NULL, &app_p->vk_instance) != VK_SUCCESS) {
        return STATUS_FAILURE;
    }

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
