#include <string.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>

terra_status_t terra_mesh_new(
    terra_app_t *app, terra_vector_t *verts, terra_mesh_t *out
) {
  logi_debug("Creating new mesh from data");
  terra_mesh_t mesh;
  mesh.verts = verts;
  TERRA_CALL_I(
      terra_vb_new(app, verts, &mesh.stag_buf, &mesh.vert_buf),
      "Failed allocating vertex buffer for the mesh"
  );
  *out = mesh;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_cleanup(terra_app_t *app, terra_mesh_t *mesh) {
  logi_info("Cleaning up mesh");
  TERRA_CALL_I(
      terra_buffer_cleanup(app, &mesh->stag_buf),
      "Failed cleaning up staging buffer"
  );
  TERRA_CALL_I(
      terra_buffer_cleanup(app, &mesh->vert_buf),
      "Failed cleaning up vertex buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_update(terra_app_t *app, terra_mesh_t *mesh) {
  logi_debug("Updating mesh");
  void *temp_data;
  TERRA_VK_CALL_I(
      vmaMapMemory(app->vma_alloc, mesh->stag_buf.alloc, &temp_data),
      "Failed mapping memory"
  );
  memcpy(
      temp_data, mesh->verts->data, terra_vector_total_size(app, mesh->verts)
  );
  vmaUnmapMemory(app->vma_alloc, mesh->stag_buf.alloc);

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

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_mesh_draw(
    terra_app_t *app,
    VkCommandBuffer cmd_buffer,
    terra_mesh_t *mesh,
    uint32_t instances
) {
  TERRA_CALL_I(terra_mesh_bind(app, cmd_buffer, mesh), "Failed binding mesh");
  vkCmdDraw(
      cmd_buffer,
      (uint32_t)terra_vector_total_size(app, mesh->verts),
      instances,
      0,
      0
  );

  return TERRA_STATUS_SUCCESS;
}
