#include <stdlib.h>

#include <terra/status.h>
#include <terra_utils/vendor/log.h>
#include <terrar/app.h>

terrar_app_state_t terrar_state_default(void) {
    terrar_app_state_t s;
    s.i = 0;
    return s;
}

terrar_app_t terrar_app_new(void *start, void *loop, void *cleanup, const char *app_name) {
    terrar_app_t app;
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

terrar_app_t terrar_app_new_wstate(terrar_app_state_t state, void *start, void *loop, void *cleanup,
                                   const char *app_name) {
    terrar_app_t app = terrar_app_new(start, loop, cleanup, app_name);
    app.state = state;
    return app;
}

terra_status_t terrar_app_run(terrar_app_t *app) {
    log_debug("Application start");
    terra_status_t start_status = app->start(app);
    terra_status_t loop_status = TERRA_STATUS_SUCCESS;
    terra_status_t cleanup_status = TERRA_STATUS_SUCCESS;
    if (start_status == TERRA_STATUS_EXIT) {
        return TERRA_STATUS_SUCCESS;
    }
    if (start_status == TERRA_STATUS_FAILURE) {
        log_fatal("Application failed on startup");
        return TERRA_STATUS_FAILURE;
    }

    if ((void *)app->loop != NULL) {
        log_debug("Application loop");
        while (loop_status == TERRA_STATUS_SUCCESS) {
            loop_status = app->loop(app);
            app->state.i++;
        }
    }
    if ((void *)app->cleanup != NULL) {
        log_debug("Application cleanup");
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
