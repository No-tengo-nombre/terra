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
  alloc_info.flags = VMA_ALLOCATION_CREATE_HOST_ACCESS_SEQUENTIAL_WRITE_BIT;
  // alloc_info.requiredFlags           = VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
  //                            VK_MEMORY_PROPERTY_HOST_COHERENT_BIT;

  logi_debug("Calling VMA to create buffer");
  TERRA_VK_CALL_I(
      vmaCreateBuffer(
          app->vma_alloc, &info, &alloc_info, out_buffer, out_alloc, NULL
      ),
      "Failed to create buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

// terra_status_t terra_buffer_bind(terra_app_t *app, terra_buffer_t *buf) {
//   logi_debug("Binding buffer");
//   TERRA_VK_CALL_I(
//       vmaBindBufferMemory(app->vma_alloc, buf->alloc, buf->buffer),
//       "Failed binding buffer"
//   );

//   return TERRA_STATUS_SUCCESS;
// }

terra_status_t terra_buffer_cleanup(terra_app_t *app, terra_buffer_t *buf) {
  logi_debug("Destroying buffer");
  vmaDestroyBuffer(app->vma_alloc, buf->buffer, buf->alloc);
  return TERRA_STATUS_SUCCESS;
}
