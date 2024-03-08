#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>

terra_status_t terra_vb_new(
    terra_app_t *app, uint64_t size, terra_buffer_t *out
) {
  TERRA_CALL_I(
      terra_buffer_new(
          app,
          size,
          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
          VK_SHARING_MODE_EXCLUSIVE,
          &out->alloc,
          &out->buffer
      ),
      "Failed to create vertex buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vb_bind(terra_app_t *app, terra_buffer_t *vb) {
  logi_debug("Binding vertex buffer");
  TERRA_VK_CALL_I(
      vmaBindBufferMemory(app->vma_alloc, vb->alloc, vb->buffer),
      "Failed binding vertex buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vb_cleanup(terra_app_t *app) {
  logi_debug("Cleaning vertex buffer");
  vmaDestroyBuffer(
      app->vma_alloc, app->vert_buffer.buffer, app->vert_buffer.alloc
  );

  return TERRA_STATUS_SUCCESS;
}
