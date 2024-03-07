#include <terra/terra.h>
#include <terra_utils/macros.h>

// static terra_vb_find_memory_type(
//     terra_app_t *app, uint32_t type_filter, VkMemoryPropertyFlags props
// ) {
//   VkPhysicalDeviceMemoryProperties mem_props;
//   vkGetPhysicalDeviceMemoryProperties(app->vk_ldevice, &mem_props);

//   for (uint32_t i = 0; i < mem_props.memoryTypeCount; i++) {}
// }

terra_status_t terra_vb_new(terra_app_t *app, uint64_t size) {
  TERRA_CALL_I(
      terra_buffer_new(
          app,
          size,
          VK_BUFFER_USAGE_VERTEX_BUFFER_BIT,
          VK_SHARING_MODE_EXCLUSIVE,
          &app->vert_alloc,
          &app->vert_buffer
      ),
      "Failed to create vertex buffer"
  );

  // VkMemoryRequirements mem_reqs;
  // vkGetBufferMemoryRequirements(app->vk_ldevice, app->vert_buffer, &mem_reqs);

  return TERRA_STATUS_SUCCESS;
}
