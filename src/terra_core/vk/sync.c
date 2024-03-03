#include <terra/app.h>
#include <terra/status.h>
#include <terra/vk/sync.h>
#include <terra/vulkan.h>
#include <terra_utils/macros.h>

terra_status_t terra_vk_create_sync_objects(terra_app_t *app) {
  VkSemaphoreCreateInfo img_available_info = {VK_FALSE};
  img_available_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkSemaphoreCreateInfo render_finished_info = {VK_FALSE};
  render_finished_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo in_flight_info = {VK_FALSE};
  in_flight_info.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;

  TERRA_VK_CALL_I(
      vkCreateSemaphore(
          app->vk_ldevice, &img_available_info, NULL, &app->vk_img_available_S
      ),
      "Failed creating 'vk_img_available_S' semaphore"
  );
  TERRA_VK_CALL_I(
      vkCreateSemaphore(
          app->vk_ldevice,
          &render_finished_info,
          NULL,
          &app->vk_render_finished_S
      ),
      "Failed creating 'vk_render_finished_S' semaphore"
  );
  TERRA_VK_CALL_I(
      vkCreateFence(
          app->vk_ldevice, &in_flight_info, NULL, &app->vk_in_flight_F
      ),
      "Failed creating 'vk_in_flight_F' fence"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_detroy_sync_objects(terra_app_t *app) {
  vkDestroySemaphore(app->vk_ldevice, app->vk_img_available_S, NULL);
  vkDestroySemaphore(app->vk_ldevice, app->vk_render_finished_S, NULL);
  vkDestroyFence(app->vk_ldevice, app->vk_in_flight_F, NULL);

  return TERRA_STATUS_SUCCESS;
}
