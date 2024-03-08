#include <string.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>

terra_status_t terra_vb_new(
    terra_app_t *app,
    terra_vector_t *data,
    terra_buffer_t *out_stag,
    terra_buffer_t *out_vert
) {
  size_t size = terra_vector_total_size(app, data);

  logi_debug("Creating staging buffer");
  TERRA_CALL_I(
      terra_buffer_new(
          app,
          size,
          VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
          VK_SHARING_MODE_EXCLUSIVE,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
          out_stag
      ),
      "Failed creating staging buffer"
  );

  logi_debug("Mapping data to staging buffer");
  void *temp_data;
  TERRA_VK_CALL_I(
      vmaMapMemory(app->vma_alloc, out_stag->alloc, &temp_data),
      "Failed mapping memory"
  );
  memcpy(temp_data, data->data, size);
  vmaUnmapMemory(app->vma_alloc, out_stag->alloc);

  logi_debug("Creating vertex buffer");
  TERRA_CALL_I(
      terra_buffer_new(
          app,
          size,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
          VK_SHARING_MODE_EXCLUSIVE,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
          out_vert
      ),
      "Failed to create vertex buffer"
  );

  logi_debug("Transfering memory from staging buffer to vertex buffer");
  TERRA_CALL_I(
      terra_buffer_copy(app, out_stag, out_vert, size),
      "Failed memory transfer from staging to vertex buffer"
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
