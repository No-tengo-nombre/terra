#include <terra/terra.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_vertex3_t triangle_vertices[] = {
    {{0.0f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    { {0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0, 1.0f}}
};
uint32_t triangle_indices[] = {0, 1, 2};
terra_vector_t triangle_v;
terra_vector_t triangle_iv;
terra_mesh_t triangle_m;

terra_status_t start(terra_app_t *app) {
  log_debug("Loading triangle");
  TERRA_CALL(
      terra_vector_from_array(
          app,
          triangle_vertices,
          sizeof(triangle_vertices) / sizeof(triangle_vertices[0]),
          sizeof(triangle_vertices[0]),
          &triangle_v
      ),
      "Failed reading triangle"
  );
  TERRA_CALL(
      terra_vector_from_array(
          app,
          triangle_indices,
          sizeof(triangle_indices) / sizeof(triangle_indices[0]),
          sizeof(triangle_indices[0]),
          &triangle_iv
      ),
      "Failed reading triangle indices"
  );
  TERRA_CALL(
      terra_mesh_new(app, &triangle_v, &triangle_iv, &triangle_m),
      "Failed to create mesh"
  );
  TERRA_CALL(terra_mesh_push(app, &triangle_m), "Failed to push triangle");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terra_app_t *app) {
  if (terra_app_should_close(app)) {
    log_info("Terminating program loop");
    return TERRA_STATUS_EXIT;
  }
  glfwPollEvents();

  // In reality you would probably not update the mesh on every frame
  TERRA_CALL(
      terra_mesh_update(app, &triangle_m, NULL, NULL), "Failed updating mesh"
  );
  TERRA_CALL(terra_app_draw(app), "Failed high-level draw call");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t cleanup(terra_app_t *app) {
  log_info("Cleaning up shapes");
  TERRA_CALL(
      terra_vector_cleanup(app, &triangle_v),
      "Failed to clean up triangle vertices"
  );
  TERRA_CALL(
      terra_mesh_cleanup(app, &triangle_m), "Failed to clean up triangle mesh"
  );
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terra_pipeline_fnames_t pl = terra_pipeline_fnames_default();
  pl.vert                    = "bin/debug/ex.triangle/triangle.vert.spv";
  pl.frag                    = "bin/debug/ex.triangle/triangle.frag.spv";
  terra_pipeline_fnames_t pipelines[] = {pl};
  size_t pipelines_count = sizeof(pipelines) / sizeof(pipelines[0]);

  terra_app_metadata_t meta = terra_app_metadata_default();
  meta.app_name             = "Terra (example) - Triangle";

  terra_app_config_t conf = terra_app_config_default();
  conf.log_dir            = "logs";
  conf.pipelines_params   = NULL;
  conf.pipelines_fnames   = pipelines;
  conf.pipelines_count    = pipelines_count;

  terra_app_t app;
  terra_app_new(&start, &loop, &cleanup, &meta, &conf, &app);
  return terra_app_run(&app);
}
