#include <stdlib.h>

#include <terra_utils/vendor/log.h>
#include <terra/status.h>
#include <terrar/app.h>

terrar_app_state_t terrar_state_default(void) {
    terrar_app_state_t s;
    s.i = 0;
    return s;
}

terrar_app_t terrar_app_new(void *start, void *loop, void *cleanup, const char *app_name, const char *window_title) {
    terrar_app_t app;
    app.start = start;
    app.loop = loop;
    app.cleanup = cleanup;
    app.state = terrar_state_default();
    app.app_name = app_name;
    app.window_title = window_title;
    return app;
}

terrar_app_t terrar_app_new_wstate(terrar_app_state_t state, void *start, void *loop, void *cleanup, const char *app_name, const char *window_title) {
    terrar_app_t app = terrar_app_new(start, loop, cleanup, app_name, window_title);
    app.state = state;
    return app;
}

status_t terrar_app_run(terrar_app_t *app) {
    log_debug("Application start");
    status_t start_status = app->start(app);
    status_t loop_status = STATUS_SUCCESS;
    status_t cleanup_status = STATUS_SUCCESS;
    if (start_status == STATUS_EXIT) {
        return STATUS_SUCCESS;
    }
    if (start_status == STATUS_FAILURE) {
        log_fatal("Application failed on startup");
        return STATUS_FAILURE;
    }

    if ((void *)app->loop != NULL) {
        log_debug("Application loop");
        while (loop_status == STATUS_SUCCESS) {
            loop_status = app->loop(app);
            app->state.i++;
        }
    }
    if ((void *)app->cleanup != NULL) {
        log_debug("Application cleanup");
        cleanup_status = app->cleanup(app);
    }
    if (loop_status == STATUS_FAILURE) {
        log_fatal("Application failed in loop");
        if (cleanup_status == STATUS_FAILURE) {
            log_fatal("could not perform cleanup");
        }
        return STATUS_FAILURE;
    }
    if (cleanup_status == STATUS_FAILURE) {
        log_fatal("Application failed on cleanup");
        return STATUS_FAILURE;
    }
    return STATUS_SUCCESS;
}
