#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <terra_utils/vendor/log.h>
#include <terrar/app.h>

#include "init.h"

status_t start(void *app) {
    terrar_app_t *app_p = (terrar_app_t *)app;

    init_window(app_p);
    init_instance(app_p);
    choose_pdevice(app_p);
    create_ldevice(app_p);
    retrieve_device_queue(app_p);

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
