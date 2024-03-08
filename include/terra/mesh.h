#pragma once

#include "app.h"
#include "buffers/buffers.h"
#include "data/vector.h"
#include "status.h"

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_mesh {
  terra_vector_t *verts;
  // terra_vector_t *indices;
  terra_buffer_t vert_buf;
  terra_buffer_t stag_buf;
  terra_buffer_t idx_buf;
} terra_mesh_t;

terra_status_t terra_mesh_new(
    terra_app_t *app,
    terra_vector_t *verts,
    // terra_vector_t *indices,
    terra_mesh_t *out
);
terra_status_t terra_mesh_cleanup(terra_app_t *app, terra_mesh_t *mesh);

terra_status_t terra_mesh_update(
    terra_app_t *app, terra_mesh_t *mesh, terra_vector_t *new_verts
);
terra_status_t terra_mesh_push(terra_app_t *app, terra_mesh_t *mesh);
terra_status_t terra_mesh_bind(
    terra_app_t *app, VkCommandBuffer cmd_buffer, terra_mesh_t *mesh
);
terra_status_t terra_mesh_draw(
    terra_app_t *app,
    VkCommandBuffer cmd_buffer,
    terra_mesh_t *mesh,
    uint32_t instances
);

#ifdef __cplusplus
}
#endif
