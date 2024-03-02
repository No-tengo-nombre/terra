#include <terra/app.h>
#include <terra/setup.h>
#include <terra/vk/pipeline.h>
#include <terra/vk/swapchain.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_status_t start(terra_app_t *app) {
  TERRA_CALL(terra_init(app, NULL), "Failed initializing app");
  terra_vk_pipeline_t pipeline;
  TERRA_CALL(
      terra_vk_pipeline_from_filenames(
          app,
          NULL,
          "bin/debug/ex.triangle/triangle.vert.spv",
          "bin/debug/ex.triangle/triangle.frag.spv",
          &pipeline
      ),
      "Failed creating pipeline"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terra_app_t *app) {
  if (terra_app_should_close(app)) {
    log_info("Terminating program loop");
    return TERRA_STATUS_EXIT;
  }

  glfwPollEvents();

  return TERRA_STATUS_SUCCESS;
}

terra_status_t cleanup(terra_app_t *app) {
  TERRA_CALL(terra_app_cleanup(app), "Failed cleanup");
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terra_app_metadata_t meta = terra_app_metadata_default();
  meta.app_name             = "Terra (example) - Triangle";

  terra_app_config_t conf = terra_app_config_default();

  terra_app_t app;
  terra_app_new(&start, &loop, &cleanup, &meta, &conf, &app);
  return terra_app_run(&app);
}
