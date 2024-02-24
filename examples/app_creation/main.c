#include <stdio.h>

#include <terra/status.h>
#include <terrar/app.h>

terra_status_t start(terrar_app_t *app) {
  printf("Starting application\n");
  return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terrar_app_t *app) {
  printf("Iteration %I64u\n", app->state.i);
  if (app->state.i >= 10) {
    return TERRA_STATUS_EXIT;
  }
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terrar_app_metadata_t meta = terrar_app_metadata_default();
  meta.app_name = "Terra (example) - App creation";

  terrar_app_config_t conf = terrar_app_config_default();

  terrar_app_t app;
  terrar_app_new(&start, &loop, NULL, &meta, &conf, &app);
  terra_status_t app_status = terrar_app_run(&app);
  switch (app_status) {
  case TERRA_STATUS_SUCCESS:
    printf("App finished succesfully\n");
    break;
  case TERRA_STATUS_FAILURE:
    printf("App finished with failure\n");
    break;
  case TERRA_STATUS_EXIT:
    printf("Unexpected case\n");
    break;
  }
  return 0;
}
