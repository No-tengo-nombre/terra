#include <terra/buffers/buffer.h>

terra_status_t terra_buffer_new(
    terra_app_t *app,
    uint64_t size,
    VkBufferUsageFlags usage,
    VkSharingMode mode,
    VkBuffer *out
) {
  VkBufferCreateInfo info = {VK_FALSE};
  info.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  info.size               = size;
  info.usage              = usage;
  info.sharingMode        = mode;

  TERRA_VK_CALL_I(
      vkCreateBuffer(app->vk_ldevice, &info, NULL, out),
      "Failed to create buffer"
  );

  return TERRA_STATUS_SUCCESS;
}
