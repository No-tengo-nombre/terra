#include <terra/terra.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_vertex3_t quad_vertices[] = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    { {-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {  {0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0, 1.0f}},
    { {0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 1.0, 1.0f}}
};
uint32_t quad_indices[] = {0, 1, 2, 1, 2, 3};
terra_vector_t quad_v;
terra_mesh_t quad_m;

terra_status_t start(terra_app_t *app) {
  TERRA_CALL(terra_init(app, NULL), "Failed initializing app");

  // TODO: Move the rest of the initializations into the terra_init function
  log_info("Creating pipeline");
  TERRA_CALL(
      terra_vk_pipeline_from_filenames(
          app,
          NULL,
          "bin/debug/ex.quad/quad.vert.spv",
          "bin/debug/ex.quad/quad.frag.spv"
      ),
      "Failed creating pipeline"
  );
  TERRA_CALL(terra_vk_framebuffer_new(app), "Failed creating framebuffers");
  TERRA_CALL(terra_vk_command_pool_new(app), "Failed creating command pool");
  TERRA_CALL(terra_vk_create_sync_objects(app), "Failed creating sync objects");

  log_info("Initialized application, reading shapes");
  log_debug("Loading quad");
  TERRA_CALL(
      terra_vector_from_array(
          app,
          quad_vertices,
          sizeof(quad_vertices),
          sizeof(quad_vertices[0]),
          &quad_v
      ),
      "Failed reading quad"
  );
  TERRA_CALL(terra_mesh_new(app, &quad_v, &quad_m), "Failed to create mesh");
  TERRA_CALL(terra_mesh_push(app, &quad_m), "Failed to push quad");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terra_app_t *app) {
  if (terra_app_should_close(app)) {
    log_info("Terminating program loop");
    return TERRA_STATUS_EXIT;
  }
  glfwPollEvents();

  // In reality you would probably not update the mesh on every frame
  TERRA_CALL(terra_mesh_update(app, &quad_m), "Failed updating mesh");
  TERRA_CALL(terra_app_draw(app), "Failed high-level draw call");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t cleanup(terra_app_t *app) {
  log_info("Cleaning up shapes");
  TERRA_CALL(
      terra_vector_cleanup(app, &quad_v), "Failed to clean up quad vertices"
  );
  TERRA_CALL(terra_mesh_cleanup(app, &quad_m), "Failed to clean up quad mesh");
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
