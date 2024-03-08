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
  terra_buffer_t buf;
} terra_mesh_t;

terra_status_t terra_mesh_new(
    terra_app_t *app, terra_vector_t *verts, terra_mesh_t *out
);
terra_status_t terra_mesh_cleanup(terra_app_t *app, terra_mesh_t *mesh);

terra_status_t terra_mesh_update(terra_app_t *app, terra_mesh_t *mesh);
terra_status_t terra_mesh_push(terra_app_t *app, terra_mesh_t *mesh);

#ifdef __cplusplus
}
#endif
