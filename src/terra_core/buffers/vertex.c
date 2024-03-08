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

terra_status_t terra_vb_cleanup(terra_app_t *app) {
  logi_debug("Cleaning vertex buffer");
  TERRA_CALL_I(
      terra_buffer_cleanup(app, &app->vert_buffer),
      "Failed cleaning vertex buffer"
  );

  return TERRA_STATUS_SUCCESS;
}
