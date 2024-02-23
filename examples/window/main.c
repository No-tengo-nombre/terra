#include <terrar/app.h>
#include <terrar/setup.h>
#include <terrau/log.h>

#define _CALL(x)                                                               \
  if (x == TERRA_STATUS_FAILURE)                                               \
  return TERRA_STATUS_FAILURE

terra_status start(terrar_app *app) {
  _CALL(terrar_setup_init_window(app));
  _CALL(terrar_setup_init_instance(app));
  _CALL(terrar_setup_create_render_surface(app));
  _CALL(terrar_setup_choose_pdevice(app));
  _CALL(terrar_setup_create_ldevice(app));
  _CALL(terrar_setup_retrieve_device_queue(app));

  return TERRA_STATUS_SUCCESS;
}

terra_status loop(terrar_app *app) {
  if (terrar_app_should_close(app)) {
    log_info("Terminating program loop");
    return TERRA_STATUS_EXIT;
  }

  glfwPollEvents();

  return TERRA_STATUS_SUCCESS;
}

terra_status cleanup(terrar_app *app) {
  _CALL(terrar_app_cleanup(app));
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terrar_app_metadata meta = terrar_app_metadata_default();
  meta.app_name = "Terra (example) - Window";

  terrar_app_config conf = terrar_app_config_default();

  terrar_app app;
  terrar_app_new(&start, &loop, &cleanup, &meta, &conf, &app);
  return terrar_app_run(&app);
}
