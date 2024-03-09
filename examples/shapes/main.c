#include <terra/terra.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_vertex3_t quad_vertices[] = {
    {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f, 1.0f}},
    { {-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {  {0.5f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0, 1.0f}},
    { {0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 1.0, 1.0f}}
};
terra_vertex3_t triangle_vertices[] = {
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.75f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0, 1.0f}},
    {{0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 1.0, 1.0f}}
};
terra_vertex3_t triangle2_vertices[] = {
    {{-0.5f, 0.5f, 0.0f}, {0.0f, 1.0f, 0.0f, 1.0f}},
    {{0.15f, 0.5f, 0.0f},  {0.0f, 0.0f, 1.0, 1.0f}},
    {{0.5f, -0.5f, 0.0f},  {1.0f, 1.0f, 1.0, 1.0f}}
};
uint32_t quad_indices[]     = {0, 1, 2, 2, 3, 0};
uint32_t triangle_indices[] = {0, 1, 2};

terra_vector_t quad_v;
terra_vector_t triangle_v;
terra_vector_t triangle2_v;
terra_vector_t quad_iv;
terra_vector_t triangle_iv;

terra_mesh_t quad_m;
terra_mesh_t triangle_m;

terra_status_t create_vectors(terra_app_t *app) {
  log_debug("Loading vectors with data");
  TERRA_CALL(
      terra_vector_from_array(
          app,
          quad_vertices,
          sizeof(quad_vertices) / sizeof(quad_vertices[0]),
          sizeof(quad_vertices[0]),
          &quad_v
      ),
      "Failed reading quad"
  );
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
          triangle2_vertices,
          sizeof(triangle2_vertices) / sizeof(triangle2_vertices[0]),
          sizeof(triangle2_vertices[0]),
          &triangle2_v
      ),
      "Failed reading triangle2"
  );
  TERRA_CALL(
      terra_vector_from_array(
          app,
          quad_indices,
          sizeof(quad_indices) / sizeof(quad_indices[0]),
          sizeof(quad_indices[0]),
          &quad_iv
      ),
      "Failed reading quad indices"
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

  return TERRA_STATUS_SUCCESS;
}

terra_status_t start(terra_app_t *app) {
  TERRA_CALL(terra_init(app, NULL), "Failed initializing app");

  // TODO: Move the rest of the initializations into the terra_init function
  log_info("Creating pipeline");
  TERRA_CALL(
      terra_vk_pipeline_from_filenames(
          app,
          NULL,
          "bin/debug/ex.shapes/shapes.vert.spv",
          "bin/debug/ex.shapes/shapes.frag.spv"
      ),
      "Failed creating pipeline"
  );
  TERRA_CALL(terra_vk_framebuffer_new(app), "Failed creating framebuffers");
  TERRA_CALL(terra_vk_command_pool_new(app), "Failed creating command pool");
  TERRA_CALL(terra_vk_create_sync_objects(app), "Failed creating sync objects");

  log_info("Initialized application, reading shapes");
  TERRA_CALL(create_vectors(app), "Failed to create vectors");

  log_debug("Creating meshes");
  TERRA_CALL(
      terra_mesh_new(app, &quad_v, &quad_iv, &quad_m), "Failed to create mesh"
  );
  TERRA_CALL(
      terra_mesh_new(app, &triangle_v, &triangle_iv, &triangle_m),
      "Failed to create mesh"
  );

  log_debug("Registering meshes with app");
  TERRA_CALL(terra_mesh_push(app, &quad_m), "Failed to push quad");
  TERRA_CALL(terra_mesh_push(app, &triangle_m), "Failed to push triangle");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t loop(terra_app_t *app) {
  if (terra_app_should_close(app)) {
    log_info("Terminating program loop");
    return TERRA_STATUS_EXIT;
  }
  glfwPollEvents();

  if (app->state.i == 10000) {
    log_info("Updating triangle mesh");
    TERRA_CALL(
        terra_mesh_update(app, &triangle_m, &triangle2_v, NULL),
        "Failed updating triangle mesh"
    );
  }
  TERRA_CALL(terra_app_draw(app), "Failed high-level draw call");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t cleanup(terra_app_t *app) {
  log_info("Cleaning up shapes");
  TERRA_CALL(
      terra_vector_cleanup(app, &quad_v), "Failed to clean up quad vertices"
  );
  TERRA_CALL(
      terra_vector_cleanup(app, &triangle_v),
      "Failed to clean up triangle vertices"
  );
  TERRA_CALL(
      terra_vector_cleanup(app, &triangle2_v),
      "Failed to clean up triangle2 vertices"
  );
  TERRA_CALL(
      terra_vector_cleanup(app, &triangle_iv),
      "Failed to clean up triangle indices"
  );
  TERRA_CALL(
      terra_vector_cleanup(app, &quad_iv), "Failed to clean up quad indices"
  );

  TERRA_CALL(terra_mesh_cleanup(app, &quad_m), "Failed to clean up quad mesh");
  TERRA_CALL(
      terra_mesh_cleanup(app, &triangle_m), "Failed to clean up triangle mesh"
  );
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
