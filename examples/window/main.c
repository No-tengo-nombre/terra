#include <terrar/app.h>
#include <terrar/setup.h>
#include <terrar/vk/swapchain.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_status_t start(terrar_app_t *app) {
  TERRA_CALL(terrar_init_window(app), "Failed initializing window");
  TERRA_CALL(terrar_init_instance(app), "Failed initializing instance");
  TERRA_CALL(terrar_create_render_surface(app),
             "Failed creating render surface");
  TERRA_CALL(terrar_choose_pdevice(app), "Failed choosing physical device");
  TERRA_CALL(terrar_create_ldevice(app), "Failed creating logical device");
  TERRA_CALL(terrar_retrieve_device_queue(app),
             "Failed retrieving device queue");
  TERRA_CALL(
      terrar_vk_create_sc(app, VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, NULL),
      "Failed creating swapchain");

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
  TERRA_CALL(terrar_app_cleanup(app), "Failed cleanup");
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
