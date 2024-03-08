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

terra_status_t terra_vb_bind(
    terra_app_t *app, VkCommandBuffer cmd_buffer, terra_buffer_t *buf
) {
  logi_debug("Binding vertex buffer");
  VkDeviceSize offsets[] = {0};
  vkCmdBindVertexBuffers(cmd_buffer, 0, 1, &buf->buffer, offsets);

  return TERRA_STATUS_SUCCESS;
}
