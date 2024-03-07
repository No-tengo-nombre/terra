#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terrau/mem.h>

terra_status_t terra_vk_command_pool_new(terra_app_t *app) {
  // This function assumes the array for command buffers already has enough size

  VkCommandPoolCreateInfo pool_info = {VK_FALSE};
  pool_info.sType            = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
  pool_info.flags            = app->conf->command_pool_flags;
  pool_info.queueFamilyIndex = app->vk_qinfo.gfamily;

  TERRA_VK_CALL_I(
      vkCreateCommandPool(app->vk_ldevice, &pool_info, NULL, &app->vk_commands),
      "Failed creating the command pool"
  );

  // TODO: Evaluate need to extract command buffer creation outside for multiple
  // buffer creation
  VkCommandBufferAllocateInfo buffer_info = {VK_FALSE};
  buffer_info.sType       = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
  buffer_info.commandPool = app->vk_commands;
  buffer_info.level       = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
  buffer_info.commandBufferCount = app->conf->max_frames_in_flight;

  TERRA_VK_CALL_I(
      vkAllocateCommandBuffers(
          app->vk_ldevice, &buffer_info, app->vk_command_buffers
      ),
      "Failed creating the command buffer"
  );

  return TERRA_STATUS_SUCCESS;
}
