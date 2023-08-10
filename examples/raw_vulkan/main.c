#include <terra_utils/vendor/log.h>
#include <terrar/app.h>
#include <terrar/setup.h>

// TODO: Move the raw *Vulkan* code to `terra_render`

#define _CALL(x)                                                                                   \
    if (x == TERRA_STATUS_FAILURE)                                                                       \
    return TERRA_STATUS_FAILURE

terra_status_t start(terrar_app_t *app) {
    _CALL(terrar_init_window(app));
    _CALL(terrar_init_instance(app));
    _CALL(terrar_create_render_surface(app));
    _CALL(terrar_choose_pdevice(app));
    _CALL(terrar_create_ldevice(app));
    _CALL(terrar_retrieve_device_queue(app));

    return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terrar_app_t *app) {
    if (glfwWindowShouldClose(app->glfw_window)) {
        log_info("Terminating program loop");
        return TERRA_STATUS_EXIT;
    }

    glfwPollEvents();

    return TERRA_STATUS_SUCCESS;
}

terra_status_t cleanup(terrar_app_t *app) {
    vkDestroySurfaceKHR(app->vk_instance, app->vk_surface, NULL);
    vkDestroyDevice(app->vk_ldevice, NULL);

    vkDestroyInstance(app->vk_instance, NULL);
    glfwDestroyWindow(app->glfw_window);
    glfwTerminate();

    return TERRA_STATUS_SUCCESS;
}

int main(void) {
    terrar_app_t app = terrar_app_new(&start, &loop, &cleanup, "Terra (example) - Raw Vulkan");
    return terrar_app_run(&app);
}
