#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>

terra_status_t terra_buffer_new(
    terra_app_t *app,
    uint64_t size,
    VkBufferUsageFlags usage,
    VkSharingMode mode,
    VkMemoryPropertyFlags req_flags,
    terra_buffer_t *out
) {
  logi_info("Creating buffer");
  VkBufferCreateInfo info = {VK_FALSE};
  info.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  info.size               = size;
  info.usage              = usage;
  info.sharingMode        = mode;

  VmaAllocationCreateInfo alloc_info = {VK_FALSE};
  alloc_info.usage                   = VMA_MEMORY_USAGE_AUTO;
  alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
  alloc_info.requiredFlags = req_flags;

  out->data = NULL;
  logi_debug("Calling VMA to create buffer");
  TERRA_VK_CALL_I(
      vmaCreateBuffer(
          app->vma_alloc, &info, &alloc_info, &out->buffer, &out->alloc, NULL
      ),
      "Failed to create buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_buffer_cleanup(terra_app_t *app, terra_buffer_t *buf) {
  logi_debug("Destroying buffer");
  vmaDestroyBuffer(app->vma_alloc, buf->buffer, buf->alloc);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_buffer_copy(
    terra_app_t *app, terra_buffer_t *src, terra_buffer_t *dst, size_t size
) {
  // A new command buffer is created only for memory transfer operations
  logi_debug("Creating buffer for memory transfer operations");
  VkCommandBufferAllocateInfo info = {VK_FALSE};
  info.sType              = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  info.level              = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  info.commandPool        = app->vk_mem_commands;
  info.commandBufferCount = 1;

  VkCommandBuffer cmd_buf;
  TERRA_VK_CALL_I(
      vkAllocateCommandBuffers(app->vk_ldevice, &info, &cmd_buf),
      "Failed allocating command buffer"
  );

  logi_debug("Recording memory transfer command buffer");
  VkCommandBufferBeginInfo begin_info = {VK_FALSE};
  begin_info.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  begin_info.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;

  TERRA_VK_CALL_I(
      vkBeginCommandBuffer(cmd_buf, &begin_info),
      "Failed starting command buffer"
  );

  VkBufferCopy region = {VK_FALSE};
  region.srcOffset    = 0;
  region.dstOffset    = 0;
  region.size         = size;
  vkCmdCopyBuffer(cmd_buf, src->buffer, dst->buffer, 1, &region);

  TERRA_VK_CALL_I(vkEndCommandBuffer(cmd_buf), "Failed ending command buffer");

  logi_debug("Submitting memory transfer for execution");
  VkSubmitInfo submit_info       = {VK_FALSE};
  submit_info.sType              = VK_STRUCTURE_TYPE_SUBMIT_INFO;
  submit_info.commandBufferCount = 1;
  submit_info.pCommandBuffers    = &cmd_buf;

  // TODO: Implement concurrent memory transfers
  TERRA_VK_CALL_I(
      vkQueueSubmit(app->vk_gqueue, 1, &submit_info, VK_NULL_HANDLE),
      "Failed submitting to queue"
  );
  TERRA_VK_CALL_I(
      vkQueueWaitIdle(app->vk_gqueue), "Failed waiting for submission"
  );
  vkFreeCommandBuffers(app->vk_ldevice, app->vk_mem_commands, 1, &cmd_buf);

  return TERRA_STATUS_SUCCESS;
}
