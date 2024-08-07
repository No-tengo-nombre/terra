#include <terra/terra.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_status_t start(terra_app_t *app) { return TERRA_STATUS_SUCCESS; }

terra_status_t loop(terra_app_t *app) {
  if (terra_app_should_close(app)) {
    log_info("Terminating program loop");
    return TERRA_STATUS_EXIT;
  }

  glfwPollEvents();

  return TERRA_STATUS_SUCCESS;
}

terra_status_t cleanup(terra_app_t *app) {
  // This function could be removed and the associated argument in the
  // `terra_app_new` call be passed as NULL, but it is left to show how
  // it can be specified
  log_info("Nothing to clean up!");
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terra_app_metadata_t meta = terra_app_metadata_default();
  meta.app_name             = "Terra (example) - Window";

  terra_app_config_t conf = terra_app_config_default();

  terra_app_t app;
  terra_app_new(&start, &loop, &cleanup, &meta, &conf, &app);
  return terra_app_run(&app);
}
