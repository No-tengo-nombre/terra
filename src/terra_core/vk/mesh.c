#include <string.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>

terra_status_t terra_mesh_new(
    terra_app_t *app,
    terra_vector_t *verts,
    terra_vector_t *indices,
    terra_mesh_t *out
) {
  logi_debug("Creating new mesh from data");
  terra_mesh_t mesh;
  mesh.verts = verts;
  mesh.idx   = indices;
  TERRA_CALL_I(
      terra_vb_new(app, verts, &mesh.vert_sbuf, &mesh.vert_buf),
      "Failed allocating vertex buffer for the mesh"
  );
  TERRA_CALL_I(
      terra_ib_new(app, indices, &mesh.idx_sbuf, &mesh.idx_buf),
      "Failed allocating index buffer for the mesh"
  );
  *out = mesh;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_cleanup(terra_app_t *app, terra_mesh_t *mesh) {
  logi_info("Cleaning up mesh");
  TERRA_CALL_I(
      terra_buffer_cleanup(app, &mesh->vert_sbuf),
      "Failed cleaning up vertex staging buffer"
  );
  TERRA_CALL_I(
      terra_buffer_cleanup(app, &mesh->vert_buf),
      "Failed cleaning up vertex buffer"
  );
  TERRA_CALL_I(
      terra_buffer_cleanup(app, &mesh->idx_sbuf),
      "Failed cleaning up index staging buffer"
  );
  TERRA_CALL_I(
      terra_buffer_cleanup(app, &mesh->idx_buf),
      "Failed cleaning up index buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

// NOTE: Behaviour is different when using `terra_mesh_update` versus updating
// each buffer separately. When passing NULL to `terra_mesh_update` the
// corresponding buffer does not get updated, while when passing NULL to the
// functions that update separately, they update using the `mesh->[verts|idx]`
// attributes

terra_status_t terra_mesh_update(
    terra_app_t *app,
    terra_mesh_t *mesh,
    terra_vector_t *new_verts,
    terra_vector_t *new_idx
) {
  if (new_verts != NULL) {
    TERRA_CALL_I(
        terra_mesh_update_verts(app, mesh, new_verts),
        "Failed updating vertices"
    );
  }
  if (new_idx != NULL) {
    TERRA_CALL_I(
        terra_mesh_update_idx(app, mesh, new_idx), "Failed updating indices"
    );
  }

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_update_verts(
    terra_app_t *app, terra_mesh_t *mesh, terra_vector_t *new_verts
) {
  logi_debug("Updating mesh vertices");

  terra_vector_t *verts_p;
  size_t size;
  if (new_verts != NULL) {
    verts_p = new_verts;
  } else {
    verts_p = mesh->verts;
  }
  size = terra_vector_total_size(app, verts_p);

  void *temp_data;
  TERRA_VK_CALL_I(
      vmaMapMemory(app->vma_alloc, mesh->vert_sbuf.alloc, &temp_data),
      "Failed mapping memory"
  );
  memcpy(temp_data, verts_p->data, size);
  vmaUnmapMemory(app->vma_alloc, mesh->vert_sbuf.alloc);

  logi_debug("Transfering memory from staging buffer to vertex buffer");
  TERRA_CALL_I(
      terra_buffer_copy(app, &mesh->vert_sbuf, &mesh->vert_buf, size),
      "Failed memory transfer from staging to vertex buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_update_idx(
    terra_app_t *app, terra_mesh_t *mesh, terra_vector_t *new_idx
) {
  logi_debug("Updating mesh indices");

  terra_vector_t *idx_p;
  size_t size;
  if (new_idx != NULL) {
    idx_p = new_idx;
  } else {
    idx_p = mesh->idx;
  }
  size = terra_vector_total_size(app, idx_p);

  void *temp_data;
  TERRA_VK_CALL_I(
      vmaMapMemory(app->vma_alloc, mesh->idx_sbuf.alloc, &temp_data),
      "Failed mapping memory"
  );
  memcpy(temp_data, idx_p->data, size);
  vmaUnmapMemory(app->vma_alloc, mesh->idx_sbuf.alloc);

  logi_debug("Transfering memory from staging buffer to index buffer");
  TERRA_CALL_I(
      terra_buffer_copy(app, &mesh->idx_sbuf, &mesh->idx_buf, size),
      "Failed memory transfer from staging to index buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_push(terra_app_t *app, terra_mesh_t *mesh) {
  logi_debug("Registering mesh in app");
  TERRA_CALL_I(
      terra_vector_push(app, &app->shapes, mesh), "Failed to push shape"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_bind(
    terra_app_t *app, VkCommandBuffer cmd_buffer, terra_mesh_t *mesh
) {
  TERRA_CALL_I(
      terra_vb_bind(app, cmd_buffer, &mesh->vert_buf),
      "Failed binding vertex buffer"
  );
  TERRA_CALL_I(
      terra_ib_bind(app, cmd_buffer, &mesh->idx_buf),
      "Failed binding index buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_draw(
    terra_app_t *app,
    VkCommandBuffer cmd_buffer,
    terra_mesh_t *mesh,
    uint32_t instances
) {
  TERRA_CALL_I(terra_mesh_bind(app, cmd_buffer, mesh), "Failed binding mesh");
  vkCmdDrawIndexed(cmd_buffer, (uint32_t)mesh->idx->len, instances, 0, 0, 0);

  return TERRA_STATUS_SUCCESS;
}
