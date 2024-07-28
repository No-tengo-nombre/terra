#pragma once

#include <terra/app.h>
#include <terra/buffers/buffers.h>
#include <terra/data/vector.h>
#include <terra/status.h>
#include <terrau/formats/descriptors.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_mesh {
  terra_vector_t *verts;
  terra_vector_t *idx;
  terra_buffer_t vert_buf;
  terra_buffer_t vert_sbuf;
  terra_buffer_t idx_buf;
  terra_buffer_t idx_sbuf;
} terra_mesh_t;

terra_status_t terra_mesh_new(
    terra_app_t *app,
    terra_vector_t *verts,
    terra_vector_t *idx,
    terra_mesh_t *out
);
terra_status_t terra_mesh_from_descriptor(
    terra_app_t *app, const terrau_mesh_descriptor_t *desc, terra_mesh_t *out
);
terra_status_t terra_mesh_cleanup(terra_app_t *app, terra_mesh_t *mesh);

terra_status_t terra_mesh_update(
    terra_app_t *app,
    terra_mesh_t *mesh,
    terra_vector_t *new_verts,
    terra_vector_t *new_idx
);
terra_status_t terra_mesh_update_verts(
    terra_app_t *app, terra_mesh_t *mesh, terra_vector_t *new_verts
);
terra_status_t terra_mesh_update_idx(
    terra_app_t *app, terra_mesh_t *mesh, terra_vector_t *new_idx
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

/*
  Creation of meshes from files
*/

typedef enum terra_mesh_formats {
  TERRA_MESH_OBJ = 0,
  TERRA_MESH_OFF = 1,
} terra_mesh_formats_t;

terra_status_t terra_mesh_from_obj(
    terra_app_t *app, const char *filename, terra_mesh_t *out
);
terra_status_t terra_mesh_from_off(
    terra_app_t *app, const char *filename, terra_mesh_t *out
);

#ifdef __cplusplus
}
#endif
