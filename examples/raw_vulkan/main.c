#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <terra_utils/vendor/log.h>
#include <terrar/app.h>

#include "init.h"

// TODO: Move the raw *Vulkan* code to `terra_render`

#define _CALL(x)                                                                                   \
    if (x == STATUS_FAILURE)                                                                       \
    return STATUS_FAILURE

status_t start(terrar_app_t *app) {
    _CALL(init_window(app));
    _CALL(init_instance(app));
    _CALL(create_render_surface(app));
    _CALL(choose_pdevice(app));
    _CALL(create_ldevice(app));
    _CALL(retrieve_device_queue(app));

    return STATUS_SUCCESS;
}

status_t loop(terrar_app_t *app) {
    if (glfwWindowShouldClose(app->glfw_window)) {
        log_info("Terminating program loop");
        return STATUS_EXIT;
    }

    glfwPollEvents();

    return STATUS_SUCCESS;
}

status_t cleanup(terrar_app_t *app) {
    vkDestroySurfaceKHR(app->vk_instance, app->vk_surface, NULL);
    vkDestroyDevice(app->vk_ldevice, NULL);

    vkDestroyInstance(app->vk_instance, NULL);
    glfwDestroyWindow(app->glfw_window);
    glfwTerminate();

    return STATUS_SUCCESS;
}

int main(void) {
    terrar_app_t app = terrar_app_new(&start, &loop, &cleanup, "Terra (example) - Raw Vulkan");
    return terrar_app_run(&app);
}
