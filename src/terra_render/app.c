#include <stdlib.h>

#include <terra/status.h>
#include <terra_utils/vendor/log.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

terrar_app_state terrar_state_default(void) {
    terrar_app_state s;
    s.i = 0;
    return s;
}
terrar_app terrar_app_new(void *start, void *loop, void *cleanup, const char *app_name) {

    terrar_app app;
    app.start = start;
    app.loop = loop;
    app.cleanup = cleanup;
    app.state = terrar_state_default();
    app.app_name = app_name;
    app.window_title = NULL;
    app.version_major = 1;
    app.version_minor = 0;
    app.version_patch = 0;
    app.window_width = 800;
    app.window_height = 600;
    return app;
}

terrar_app terrar_app_new_wstate(terrar_app_state state, void *start, void *loop, void *cleanup,
                                 const char *app_name) {
    terrar_app app = terrar_app_new(start, loop, cleanup, app_name);
    app.state = state;
    return app;
}

terra_status terrar_app_run(terrar_app *app) {
    log_info("Application start");
    terra_status start_status = app->start(app);
    terra_status loop_status = TERRA_STATUS_SUCCESS;
    terra_status cleanup_status = TERRA_STATUS_SUCCESS;
    if (start_status == TERRA_STATUS_EXIT) {
        return TERRA_STATUS_SUCCESS;
    }
    if (start_status == TERRA_STATUS_FAILURE) {
        log_fatal("Application failed on startup");
        return TERRA_STATUS_FAILURE;
    }

    if ((void *)app->loop != NULL) {
        log_info("Application loop");
        while (loop_status == TERRA_STATUS_SUCCESS) {
            loop_status = app->loop(app);
            app->state.i++;
        }
    }
    if ((void *)app->cleanup != NULL) {
        log_info("Application cleanup");
        cleanup_status = app->cleanup(app);
    }
    if (loop_status == TERRA_STATUS_FAILURE) {
        log_fatal("Application failed in loop");
        if (cleanup_status == TERRA_STATUS_FAILURE) {
            log_fatal("could not perform cleanup");
        }
        return TERRA_STATUS_FAILURE;
    }
    if (cleanup_status == TERRA_STATUS_FAILURE) {
        log_fatal("Application failed on cleanup");
        return TERRA_STATUS_FAILURE;
    }
    return TERRA_STATUS_SUCCESS;
}

terra_status terrar_app_cleanup(terrar_app *app) {
    log_debug("Cleaning Vulkan objects");
    vkDestroySurfaceKHR(app->vk_instance, app->vk_surface, NULL);
    vkDestroyDevice(app->vk_ldevice, NULL);

    log_debug("Destroying contexts");
    vkDestroyInstance(app->vk_instance, NULL);
    glfwDestroyWindow(app->glfw_window);

    log_debug("Terminating GLFW");
    glfwTerminate();

    return TERRA_STATUS_SUCCESS;
}

int terrar_app_should_close(terrar_app *app) {
    return glfwWindowShouldClose(app->glfw_window);
}
