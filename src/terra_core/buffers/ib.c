#include <string.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>

// TODO: Make vertex and index buffers be allocated together using offsets to
// make them cache friendly

terra_status_t terra_ib_new(
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

  logi_debug("Creating index buffer");
  TERRA_CALL_I(
      terra_buffer_new(
          app,
          size,
          VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT,
          VK_SHARING_MODE_EXCLUSIVE,
          VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT,
          out_vert
      ),
      "Failed to create index buffer"
  );

  logi_debug("Transfering memory from staging buffer to index buffer");
  TERRA_CALL_I(
      terra_buffer_copy(app, out_stag, out_vert, size),
      "Failed memory transfer from staging to index buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_ib_bind(
    terra_app_t *app, VkCommandBuffer cmd_buffer, terra_buffer_t *buf
) {
  vkCmdBindIndexBuffer(cmd_buffer, buf->buffer, 0, app->conf->vk_idx_type);

  return TERRA_STATUS_SUCCESS;
}
