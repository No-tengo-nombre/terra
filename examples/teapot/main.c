#include <terra/terra.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_mesh_t teapot;

terra_status_t start(terra_app_t *app) {
  log_debug("Creating meshes");
  TERRA_CALL(
      terra_mesh_from_off(app, "resources/off/utah_teapot/teapot.off", &teapot),
      "Failed to create mesh from file"
  );

  log_debug("Registering mesh with app");
  TERRA_CALL(terra_mesh_push(app, &teapot), "Failed to push teapot");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terra_app_t *app) {
  terra_app_poll_events(app);
  TERRA_CALL(terra_app_draw(app), "Failed high-level draw call");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t cleanup(terra_app_t *app) {
  log_info("Cleaning up shapes");
  TERRA_CALL(terra_mesh_cleanup(app, &teapot), "Failed to clean up teapot");
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  // Load data
  terra_pipeline_fnames_t pl          = terra_pipeline_fnames_default();
  pl.vert                             = "bin/debug/ex.teapot/teapot.vert.spv";
  pl.frag                             = "bin/debug/ex.teapot/teapot.frag.spv";
  terra_pipeline_fnames_t pipelines[] = {pl};
  size_t pipelines_count = sizeof(pipelines) / sizeof(pipelines[0]);

  // Set up application
  terra_app_metadata_t meta = terra_app_metadata_default();
  meta.app_name             = "Terra (example) - Tea time!";

  terra_app_config_t conf = terra_app_config_default();
  conf.log_dir            = "logs";
  conf.log_stdlvl         = LOG_DEBUG;
  conf.pipelines_params   = NULL;
  conf.pipelines_fnames   = pipelines;
  conf.pipelines_count    = pipelines_count;
  conf.present_mode       = VK_PRESENT_MODE_IMMEDIATE_KHR;

  terra_app_t app;
  terra_app_new(&start, &loop, &cleanup, &meta, &conf, &app);
  return terra_app_run(&app);
}
