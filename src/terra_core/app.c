#include <stdlib.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vk/sync.h>
#include <terra/vulkan.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>
#include <terrau/mem.h>

const char *DEFAULT_VALIDATION_LAYERS[] = {"VK_LAYER_KHRONOS_validation"};
const char *DEFAULT_DEVICE_EXTENSIONS[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

terra_app_state_t terra_app_state_default(void) {
  terra_app_state_t s;
  s.i            = 0;
  s.should_close = 0;
  return s;
}

terra_app_metadata_t terra_app_metadata_default(void) {
  terra_app_metadata_t meta = {
      .vmajor        = 1,
      .vminor        = 0,
      .vpatch        = 0,
      .app_name      = "Terrar - Default application",
      .window_title  = NULL,
      .window_width  = 800,
      .window_height = 600,
  };
  return meta;
}

terra_status_t terra_app_config_new(
    const char **validation_layers,
    const char **device_extensions,
    uint32_t validation_layers_total,
    uint32_t device_extensions_total,
    terra_app_config_t *out
) {
  terra_app_config_t conf = {
      .validation_layers_total = validation_layers_total,
      .device_extensions_total = device_extensions_total,
      .validation_layers       = validation_layers,
      .device_extensions       = device_extensions,
      .surface_format          = VK_FORMAT_B8G8R8A8_SRGB,
      .color_space             = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
      .present_mode            = VK_PRESENT_MODE_MAILBOX_KHR,
      .image_array_layers      = 1,
      .composite_alpha         = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .clipped                 = VK_TRUE,
      .command_pool_flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .in_flight_fence_timeout = UINT64_MAX,
      .img_acq_timeout         = UINT64_MAX,
  };
  *out = conf;
  return TERRA_STATUS_SUCCESS;
}

terra_app_config_t terra_app_config_default(void) {
  terra_app_config_t conf;
  terra_app_config_new(
      DEFAULT_VALIDATION_LAYERS, DEFAULT_DEVICE_EXTENSIONS, 1, 1, &conf
  );
  return conf;
}

terra_status_t terra_app_new(
    terra_start_ft *start,
    terra_loop_ft *loop,
    terra_clean_ft *cleanup,
    terra_app_metadata_t *meta,
    terra_app_config_t *conf,
    terra_app_t *out
) {
  terra_app_t app = {
      .start   = start,
      .loop    = loop,
      .cleanup = cleanup,
      .state   = terra_app_state_default(),
      .meta    = meta,
      .conf    = conf,

      // These have to be NULL to avoid UB
      .vk_images       = NULL,
      .vk_image_views  = NULL,
      .vk_framebuffers = NULL,
#ifndef NDEBUG // Internal debug information
      ._idebug_malloced_total = 0,
#endif
  };
  *out = app;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_new_wstate(
    terra_app_state_t state,
    terra_start_ft *start,
    terra_loop_ft *loop,
    terra_clean_ft *cleanup,
    terra_app_metadata_t *meta,
    terra_app_config_t *conf,
    terra_app_t *out
) {
  terra_app_new(start, loop, cleanup, meta, conf, out);
  out->state = state;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_run(terra_app_t *app) {
  logi_info("Application start");
  terra_status_t start_status   = app->start(app);
  terra_status_t loop_status    = TERRA_STATUS_SUCCESS;
  terra_status_t cleanup_status = TERRA_STATUS_SUCCESS;

  // The way this is handled might harm performance
  // TODO: Check if this method harms performance
  if (start_status == TERRA_STATUS_EXIT) {
    return TERRA_STATUS_SUCCESS;
  }
  if (start_status == TERRA_STATUS_FAILURE) {
    logi_fatal("Application failed on startup");
    return TERRA_STATUS_FAILURE;
  }

  if (app->loop != NULL) {
    logi_info("Application loop");
    while (loop_status == TERRA_STATUS_SUCCESS) {
      loop_status = app->loop(app);
      app->state.i++;
    }
    logi_info("Finished loop");
    logi_info("Waiting for remaining draw calls");
    TERRA_VK_CALL_I(
        vkDeviceWaitIdle(app->vk_ldevice), "Waiting for draw call to end"
    );
  }
  if (app->cleanup != NULL) {
    logi_info("Application cleanup");
    cleanup_status = app->cleanup(app);
  }
  if (loop_status == TERRA_STATUS_FAILURE) {
    logi_fatal("Application failed in loop");
    if (cleanup_status == TERRA_STATUS_FAILURE) {
      logi_fatal("Could not perform cleanup");
    }
    return TERRA_STATUS_FAILURE;
  }
  if (cleanup_status == TERRA_STATUS_FAILURE) {
    logi_fatal("Application failed on cleanup");
    return TERRA_STATUS_FAILURE;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_set_image_count(terra_app_t *app, uint32_t new_count) {
  app->vk_images_count = new_count;
  void *images =
      terrau_realloc(app, app->vk_images, new_count * sizeof(VkImage));
  void *image_views =
      terrau_realloc(app, app->vk_image_views, new_count * sizeof(VkImageView));
  void *framebuffers = terrau_realloc(
      app, app->vk_framebuffers, new_count * sizeof(VkFramebuffer)
  );
  if (images == NULL) {
    logi_error("Could not reallocate memory for %i images", new_count);
    return TERRA_STATUS_FAILURE;
  }
  if (image_views == NULL) {
    logi_error("Could not reallocate memory for %i image views", new_count);
    return TERRA_STATUS_FAILURE;
  }
  if (framebuffers == NULL) {
    logi_error("Could not reallocate memory for %i framebuffers", new_count);
    return TERRA_STATUS_FAILURE;
  }
  app->vk_images       = images;
  app->vk_image_views  = image_views;
  app->vk_framebuffers = framebuffers;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_record_cmd_buffer(terra_app_t *app, uint32_t idx) {
  VkCommandBufferBeginInfo info = {VK_FALSE};
  info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  info.flags                    = 0;
  info.pInheritanceInfo         = NULL;

  TERRA_VK_CALL_I(
      vkBeginCommandBuffer(app->vk_command_buffer, &info),
      "Failed to begin command buffer"
  );

  // TODO: Move the clear color to the configurations of the app
  VkOffset2D offset        = {0, 0};
  VkClearValue clear_color = {{{0.0f, 0.0f, 0.0f, 1.0f}}};

  VkRenderPassBeginInfo rp_info = {VK_FALSE};
  rp_info.sType                 = VK_STRUCTURE_TYPE_RENDER_PASS_BEGIN_INFO;
  rp_info.renderPass            = app->vk_render_pass;
  rp_info.framebuffer           = app->vk_framebuffers[idx];
  rp_info.renderArea.offset     = offset; // TODO: Take the offset from the app
  rp_info.renderArea.extent     = app->vk_extent;
  rp_info.clearValueCount       = 1;
  rp_info.pClearValues          = &clear_color;

  vkCmdBeginRenderPass(
      app->vk_command_buffer, &rp_info, VK_SUBPASS_CONTENTS_INLINE
  );
  vkCmdBindPipeline(
      app->vk_command_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, app->vk_pipeline
  );

  // TODO: Add check at runtime if they are dynamic
  // Viewport and scissor are dynamic by default
  VkViewport viewport = {VK_FALSE};
  viewport.x          = 0.0f;
  viewport.y          = 0.0f;
  viewport.width      = (float)app->vk_extent.width;
  viewport.height     = (float)app->vk_extent.height;
  viewport.minDepth   = 0.0f;
  viewport.maxDepth   = 1.0f;
  vkCmdSetViewport(app->vk_command_buffer, 0, 1, &viewport);

  VkRect2D scissor = {VK_FALSE};
  scissor.offset   = offset;
  scissor.extent   = app->vk_extent;
  vkCmdSetScissor(app->vk_command_buffer, 0, 1, &scissor);

  vkCmdDraw(app->vk_command_buffer, 3, 1, 0, 0);
  vkCmdEndRenderPass(app->vk_command_buffer);

  TERRA_VK_CALL_I(
      vkEndCommandBuffer(app->vk_command_buffer),
      "Failed recording command buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_draw(terra_app_t *app) {
  uint32_t img_idx;
  TERRA_VK_CALL_I(
      vkAcquireNextImageKHR(
          app->vk_ldevice,
          app->vk_swapchain,
          app->conf->img_acq_timeout,
          app->vk_img_available_S,
          VK_NULL_HANDLE,
          &img_idx
      ),
      "Failed acquiring next image"
  );

  TERRA_VK_CALL_I(
      vkResetCommandBuffer(app->vk_command_buffer, 0),
      "Failed resetting command buffer"
  );
  TERRA_CALL_I(
      terra_app_record_cmd_buffer(app, img_idx),
      "Failed recording command buffer"
  );

  VkSubmitInfo submit_info = {VK_FALSE};
  submit_info.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  VkSemaphore wait_semaphores[]   = {app->vk_img_available_S};
  VkSemaphore signal_semaphores[] = {app->vk_render_finished_S};
  VkPipelineStageFlags stages[]   = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
  };

  submit_info.waitSemaphoreCount   = 1;
  submit_info.pWaitSemaphores      = wait_semaphores;
  submit_info.pWaitDstStageMask    = stages;
  submit_info.commandBufferCount   = 1;
  submit_info.pCommandBuffers      = &app->vk_command_buffer;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores    = signal_semaphores;
  TERRA_VK_CALL_I(
      vkQueueSubmit(app->vk_gqueue, 1, &submit_info, app->vk_in_flight_F),
      "Failed submitting draw command to queue"
  );

  /* Present the result of the draw call */

  VkSwapchainKHR swapchains[] = {app->vk_swapchain};

  VkPresentInfoKHR pres_info   = {VK_FALSE};
  pres_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pres_info.waitSemaphoreCount = 1;
  pres_info.pWaitSemaphores    = signal_semaphores;
  pres_info.swapchainCount     = 1;
  pres_info.pSwapchains        = swapchains;
  pres_info.pImageIndices      = &img_idx;
  pres_info.pResults           = NULL;

  TERRA_VK_CALL_I(
      vkQueuePresentKHR(app->vk_pqueue, &pres_info),
      "Failed to present to screen"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_cleanup(terra_app_t *app) {
  logi_debug("Cleaning up sync objects");
  TERRA_CALL_I(
      terra_vk_detroy_sync_objects(app), "Failed destroying sync objects"
  );

  logi_debug("Cleaning up command pool");
  vkDestroyCommandPool(app->vk_ldevice, app->vk_commands, NULL);

  logi_debug("Cleaning image views and framebuffers");
  VkFramebuffer *fb = app->vk_framebuffers;
  VkImageView *view = app->vk_image_views;
  for (int i = 0; i < app->vk_images_count; i++, view++, fb++) {
    vkDestroyFramebuffer(app->vk_ldevice, *fb, NULL);
    vkDestroyImageView(app->vk_ldevice, *view, NULL);
  }

  logi_debug("Releasing heap allocated arrays");
  terrau_free(app, app->vk_images);
  terrau_free(app, app->vk_image_views);
  terrau_free(app, app->vk_framebuffers);

  logi_debug("Cleaning Vulkan objects");
  vkDestroyPipeline(app->vk_ldevice, app->vk_pipeline, NULL);
  vkDestroyPipelineLayout(app->vk_ldevice, app->vk_layout, NULL);
  vkDestroyRenderPass(app->vk_ldevice, app->vk_render_pass, NULL);
  vkDestroySwapchainKHR(app->vk_ldevice, app->vk_swapchain, NULL);
  vkDestroySurfaceKHR(app->vk_instance, app->vk_surface, NULL);
  vkDestroyDevice(app->vk_ldevice, NULL);

  logi_debug("Destroying contexts");
  vkDestroyInstance(app->vk_instance, NULL);
  glfwDestroyWindow(app->glfw_window);

  logi_debug("Terminating GLFW");
  glfwTerminate();

#ifndef NDEBUG
  if (app->_idebug_malloced_total != 0) {
    logi_warn(
        "MEMORY LEAK: Found %i elements in heap after cleanup",
        app->_idebug_malloced_total
    );
  }
#endif

  return TERRA_STATUS_SUCCESS;
}

int terra_app_should_close(terra_app_t *app) {
  return glfwWindowShouldClose(app->glfw_window) || app->state.should_close;
}

VKAPI_ATTR VkBool32 VKAPI_CALL terra_app_debug_callback(
    VkDebugUtilsMessageSeverityFlagBitsEXT msg_severity,
    VkDebugUtilsMessageTypeFlagsEXT msg_type,
    const VkDebugUtilsMessengerCallbackDataEXT *cb_data,
    void *user_data
) {
  logi_error("validation layer: %s", cb_data->pMessage);
  return VK_FALSE;
}
