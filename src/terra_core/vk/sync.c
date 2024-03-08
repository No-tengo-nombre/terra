#include <stdint.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>

terra_status_t terra_vk_create_sync_objects(terra_app_t *app) {
  // This function assumes the arrays already have the proper size

  VkSemaphoreCreateInfo img_available_info = {VK_FALSE};
  img_available_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkSemaphoreCreateInfo render_finished_info = {VK_FALSE};
  render_finished_info.sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO;

  VkFenceCreateInfo in_flight_info = {VK_FALSE};
  in_flight_info.sType             = VK_STRUCTURE_TYPE_FENCE_CREATE_INFO;
  in_flight_info.flags             = VK_FENCE_CREATE_SIGNALED_BIT;
  // The flag ensures the first draw call does not fail

  VkSemaphore *img_av          = app->vk_img_available_S;
  VkSemaphore *render_finished = app->vk_render_finished_S;
  VkFence *in_flight           = app->vk_in_flight_F;
  for (uint32_t i = 0; i < app->conf->max_frames_in_flight; i++) {
    TERRA_VK_CALL_I(
        vkCreateSemaphore(app->vk_ldevice, &img_available_info, NULL, img_av),
        "Failed creating %i-th 'vk_img_available_S' semaphore",
        i
    );
    TERRA_VK_CALL_I(
        vkCreateSemaphore(
            app->vk_ldevice, &render_finished_info, NULL, render_finished
        ),
        "Failed creating %i-th 'vk_render_finished_S' semaphore",
        i
    );
    TERRA_VK_CALL_I(
        vkCreateFence(app->vk_ldevice, &in_flight_info, NULL, in_flight),
        "Failed creating %i-th 'vk_in_flight_F' fence",
        i
    );
    img_av++;
    render_finished++;
    in_flight++;
  }

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_await_sync_objects(terra_app_t *app) {
  TERRA_VK_CALL_I(
      vkWaitForFences(
          app->vk_ldevice,
          1,
          app->vk_in_flight_F + app->state.vk_frame,
          VK_TRUE,
          app->conf->in_flight_fence_timeout
      ),
      "Failed waiting for fence (check timeout)"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_reset_sync_objects(terra_app_t *app) {
  TERRA_VK_CALL_I(
      vkResetFences(
          app->vk_ldevice, 1, app->vk_in_flight_F + app->state.vk_frame
      ),
      "Failed resetting fence"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_detroy_sync_objects(terra_app_t *app) {
  for (uint32_t i = 0; i < app->conf->max_frames_in_flight; i++) {
    vkDestroySemaphore(app->vk_ldevice, app->vk_img_available_S[i], NULL);
    vkDestroySemaphore(app->vk_ldevice, app->vk_render_finished_S[i], NULL);
    vkDestroyFence(app->vk_ldevice, app->vk_in_flight_F[i], NULL);
  }

  return TERRA_STATUS_SUCCESS;
}
