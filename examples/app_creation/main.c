#include <stdio.h>

#include <terra/status.h>
#include <terrar/app.h>

status_t start(void *app) {
    printf("Starting application\n");
    return STATUS_SUCCESS;
}

status_t loop(void *app) {
    terrar_app_t *app_p = (terrar_app_t *)app;
    printf("Iteration %I64u\n", app_p->state.i);
    if (app_p->state.i >= 10) {
        return STATUS_EXIT;
    }
    return STATUS_SUCCESS;
}

int main(void) {
    terrar_app_t app = terrar_app_new(&start, &loop, NULL, "Terra (example) - App creation");
    status_t app_status = terrar_app_run(&app);
    switch (app_status) {
        case STATUS_SUCCESS:
            printf("App finished succesfully\n");
            break;
        case STATUS_FAILURE:
            printf("App finished with failure\n");
            break;
        case STATUS_EXIT:
            printf("Unexpected case\n");
            break;
    }
    return 0;
}
