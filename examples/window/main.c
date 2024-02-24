#include <terrar/app.h>
#include <terrar/setup.h>
#include <terrau/log.h>

#define _CALL(x)                                                               \
  if (x == TERRA_STATUS_FAILURE)                                               \
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
  if (terrar_app_should_close(app)) {
    log_info("Terminating program loop");
    return TERRA_STATUS_EXIT;
  }

  glfwPollEvents();

  return TERRA_STATUS_SUCCESS;
}

terra_status_t cleanup(terrar_app_t *app) {
  _CALL(terrar_app_cleanup(app));
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terrar_app_metadata_t meta = terrar_app_metadata_default();
  meta.app_name = "Terra (example) - Window";

  terrar_app_config_t conf = terrar_app_config_default();

  terrar_app_t app;
  terrar_app_new(&start, &loop, &cleanup, &meta, &conf, &app);
  return terrar_app_run(&app);
}
