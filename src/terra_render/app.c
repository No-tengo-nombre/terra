#include <stdlib.h>

#include <terra_utils/vendor/log.h>
#include <terra/status.h>
#include <terrar/app.h>

terrar_app_state_t terrar_state_default(void) {
    terrar_app_state_t s;
    s.i = 0;
    return s;
}

terrar_app_t terrar_app_new(void *start, void *loop, void *cleanup) {
    terrar_app_t app;
    app.start = start;
    app.loop = loop;
    app.cleanup = cleanup;
    app.state = terrar_state_default();
    return app;
}

terrar_app_t terrar_app_new_wstate(terrar_app_state_t state, void *start, void *loop, void *cleanup) {
    terrar_app_t app;
    app.start = start;
    app.loop = loop;
    app.cleanup = cleanup;
    app.state = state;
    return app;
}

status_t terrar_app_run(terrar_app_t *app) {
    log_debug("application start");
    status_t start_status = app->start(app);
    status_t loop_status = STATUS_SUCCESS;
    status_t cleanup_status = STATUS_SUCCESS;
    if (start_status == STATUS_EXIT) {
        return STATUS_SUCCESS;
    }
    if (start_status == STATUS_FAILURE) {
        log_warn("application failed on startup");
        return STATUS_FAILURE;
    }

    if ((void *)app->loop != NULL) {
        log_debug("application loop");
        while (loop_status == STATUS_SUCCESS) {
            loop_status = app->loop(app);
            app->state.i++;
        }
    }
    if ((void *)app->cleanup != NULL) {
        log_debug("application cleanup");
        cleanup_status = app->cleanup(app);
    }
    if (loop_status == STATUS_FAILURE || cleanup_status == STATUS_FAILURE) {
        log_warn("application failed");
        return STATUS_FAILURE;
    }
    return STATUS_SUCCESS;
}
