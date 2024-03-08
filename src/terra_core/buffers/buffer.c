#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>

terra_status_t terra_buffer_new(
    terra_app_t *app,
    uint64_t size,
    VkBufferUsageFlags usage,
    VkSharingMode mode,
    VmaAllocation *out_alloc,
    VkBuffer *out_buffer
) {
  logi_info("Creating buffer");
  VkBufferCreateInfo info = {VK_FALSE};
  info.sType              = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
  info.size               = size;
  info.usage              = usage;
  info.sharingMode        = mode;

  VmaAllocationCreateInfo alloc_info = {VK_FALSE};
  alloc_info.usage                   = VMA_MEMORY_USAGE_AUTO;

  logi_debug("Calling VMA to create buffer");
  TERRA_VK_CALL_I(
      vmaCreateBuffer(
          app->vma_alloc, &info, &alloc_info, out_buffer, out_alloc, NULL
      ),
      "Failed to create buffer"
  );

  return TERRA_STATUS_SUCCESS;
}
