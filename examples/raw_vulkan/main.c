#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <terra_utils/vendor/log.h>
#include <terrar/app.h>

#include "init.h"

#define _CALL(x)                                                                             \
    if (x == STATUS_FAILURE)                                                                       \
    return STATUS_FAILURE

status_t start(void *app) {
    terrar_app_t *app_p = (terrar_app_t *)app;

    _CALL(init_window(app_p));
    _CALL(init_instance(app_p));
    _CALL(create_render_surface(app_p));
    _CALL(choose_pdevice(app_p));
    _CALL(create_ldevice(app_p));
    _CALL(retrieve_device_queue(app_p));

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

    vkDestroySurfaceKHR(app_p->vk_instance, app_p->vk_surface, NULL);
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
