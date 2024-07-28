#include <terra/terra.h>
#include <terrau/log.h>

terra_status_t start(terra_app_t *app) {
  log_info("Starting application");
  return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terra_app_t *app) {
  log_debug("Iteration %llu", app->state.i);
  if (app->state.i >= 10) {
    return TERRA_STATUS_EXIT;
  }
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terra_app_metadata_t meta = terra_app_metadata_default();
  meta.app_name             = "Terra (example) - App creation";

  terra_app_config_t conf = terra_app_config_default();

  terra_app_t app;
  terra_app_new(&start, &loop, NULL, &meta, &conf, &app);
  return terra_app_run(&app);
}
