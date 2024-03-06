#include <terra/app.h>
#include <terra/setup.h>
#include <terra/vk/command_pool.h>
#include <terra/vk/framebuffer.h>
#include <terra/vk/pipeline.h>
#include <terra/vk/swapchain.h>
#include <terra/vk/sync.h>
#include <terra/vulkan.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_status_t start(terra_app_t *app) {
  TERRA_CALL(terra_init(app, NULL), "Failed initializing app");

  // TODO: Move the rest of the initializations into the terra_init function
  log_info("Initialized app, creating pipeline");
  TERRA_CALL(
      terra_vk_pipeline_from_filenames(
          app,
          NULL,
          "bin/debug/ex.triangle/triangle.vert.spv",
          "bin/debug/ex.triangle/triangle.frag.spv"
      ),
      "Failed creating pipeline"
  );
  TERRA_CALL(terra_vk_framebuffer_new(app), "Failed creating framebuffers");
  TERRA_CALL(terra_vk_command_pool_new(app), "Failed creating command pool");
  TERRA_CALL(terra_vk_create_sync_objects(app), "Failed creating sync objects");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terra_app_t *app) {
  if (terra_app_should_close(app)) {
    log_info("Terminating program loop");
    return TERRA_STATUS_EXIT;
  }
  TERRA_CALL(terra_vk_await_sync_objects(app), "Failed awaiting sync objects");
  TERRA_CALL(terra_app_draw(app), "Failed high-level draw call");

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
  meta.app_name             = "Terra (example) - Triangle";

  terra_app_config_t conf = terra_app_config_default();

  terra_app_t app;
  terra_app_new(&start, &loop, &cleanup, &meta, &conf, &app);
  return terra_app_run(&app);
}
