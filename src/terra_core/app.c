#include <stdlib.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>
#include <terrau/mem.h>

static const char *DEFAULT_VALIDATION_LAYERS[] = {"VK_LAYER_KHRONOS_validation"
};
static const char *DEFAULT_DEVICE_EXTENSIONS[] = {
    VK_KHR_SWAPCHAIN_EXTENSION_NAME
};
static const uint32_t DEFAULT_MAX_FRAMES_IN_FLIGHT = 2;

terra_app_state_t terra_app_state_default(void) {
  terra_app_state_t s = {
      .i            = 0,
      .should_close = 0,
      .vk_frame     = 0,
      .fb_resized   = 0,
  };
  return s;
}

terra_app_metadata_t terra_app_metadata_default(void) {
  terra_app_metadata_t meta = {
      .vmajor        = 1,
      .vminor        = 0,
      .vpatch        = 0,
      .app_name      = "Terra - Default application",
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

      .vk_version = VK_API_VERSION_1_0,

      .surface_format       = VK_FORMAT_B8G8R8A8_SRGB,
      .color_space          = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
      .present_mode         = VK_PRESENT_MODE_MAILBOX_KHR,
      .image_array_layers   = 1,
      .composite_alpha      = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
      .clipped              = VK_TRUE,
      .command_pool_flags   = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
      .max_frames_in_flight = DEFAULT_MAX_FRAMES_IN_FLIGHT,
      .dims                 = TERRA_3D,

      .resizable = 1,

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
      .vk_images            = NULL,
      .vk_image_views       = NULL,
      .vk_framebuffers      = NULL,
      .vk_img_available_S   = NULL,
      .vk_render_finished_S = NULL,
      .vk_in_flight_F       = NULL,

#ifndef NDEBUG // Internal debug information
      ._idebug_malloced_total = 0,
#endif
  };
  TERRA_CALL_I(
      terra_vector_new(&app, sizeof(terra_mesh_t), &app.shapes),
      "Failed creating vector for shapes"
  );
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

static void terra_app_log_startup_info(terra_app_t *app) {
  logi_info("~~~~~~~~ STARTUP INFORMATION ~~~~~~~~");
#ifndef NDEBUG
  logi_warn("RUNNING IN DEBUG MODE");
#endif
  logi_info("Renderer                  : %iD", app->conf->dims);
  logi_info("Image array layers        : %u", app->conf->image_array_layers);
  logi_info("Max frames in flight      : %u", app->conf->max_frames_in_flight);
  logi_info("Resizable                 : %i", app->conf->resizable);
  logi_info(
      "Target surface format     : %s",
      terra_vk_format_name(app->conf->surface_format)
  );
  logi_info(
      "Target color space        : %s",
      terra_vk_colorspace_name(app->conf->color_space)
  );
  logi_info(
      "Target present mode       : %s",
      terra_vk_present_mode_name(app->conf->present_mode)
  );
  logi_info(
      "In flight fence timeout   : %llu", app->conf->in_flight_fence_timeout
  );
  logi_info("Image acquisition timeout : %llu", app->conf->img_acq_timeout);
  logi_info("~~~~~~~~ END OF INFORMATION ~~~~~~~~");
}

terra_status_t terra_app_run(terra_app_t *app) {
  logi_info("Application start");
  terra_app_log_startup_info(app);

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

      // Update internal state
      app->state.i++;
      app->state.vk_frame =
          (app->state.vk_frame + 1) % app->conf->max_frames_in_flight;
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
  cleanup_status |= terra_app_cleanup(app);
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

terra_status_t terra_app_set_frames_in_flight(
    terra_app_t *app, uint32_t new_size
) {
  app->conf->max_frames_in_flight = new_size;
  void *command_buffers           = terrau_realloc(
      app, app->vk_command_buffers, new_size * sizeof(VkCommandBuffer)
  );
  void *img_available_smph = terrau_realloc(
      app, app->vk_img_available_S, new_size * sizeof(VkSemaphore)
  );
  void *render_finished_smph = terrau_realloc(
      app, app->vk_render_finished_S, new_size * sizeof(VkSemaphore)
  );
  void *in_flight_fences =
      terrau_realloc(app, app->vk_in_flight_F, new_size * sizeof(VkFence));

  if (command_buffers == NULL) {
    logi_error("Could not allocate %i command buffers", new_size);
    return TERRA_STATUS_FAILURE;
  }
  if (img_available_smph == NULL) {
    logi_error("Could not allocate %i semaphores", new_size);
    return TERRA_STATUS_FAILURE;
  }
  if (render_finished_smph == NULL) {
    logi_error("Could not allocate %i semaphores", new_size);
    return TERRA_STATUS_FAILURE;
  }
  if (in_flight_fences == NULL) {
    logi_error("Could not allocate %i fences", new_size);
    return TERRA_STATUS_FAILURE;
  }

  app->vk_command_buffers   = command_buffers;
  app->vk_img_available_S   = img_available_smph;
  app->vk_render_finished_S = render_finished_smph;
  app->vk_in_flight_F       = in_flight_fences;

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_record_cmd_buffer(terra_app_t *app, uint32_t idx) {
  uint32_t frame_idx         = app->state.vk_frame;
  VkCommandBuffer cmd_buffer = app->vk_command_buffers[frame_idx];

  VkCommandBufferBeginInfo info = {VK_FALSE};
  info.sType                    = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
  info.flags                    = 0;
  info.pInheritanceInfo         = NULL;

  TERRA_VK_CALL_I(
      vkBeginCommandBuffer(cmd_buffer, &info), "Failed to begin command buffer"
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

  vkCmdBeginRenderPass(cmd_buffer, &rp_info, VK_SUBPASS_CONTENTS_INLINE);
  vkCmdBindPipeline(
      cmd_buffer, VK_PIPELINE_BIND_POINT_GRAPHICS, app->vk_pipeline
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
  vkCmdSetViewport(cmd_buffer, 0, 1, &viewport);

  VkRect2D scissor = {VK_FALSE};
  scissor.offset   = offset;
  scissor.extent   = app->vk_extent;
  vkCmdSetScissor(cmd_buffer, 0, 1, &scissor);

  // TODO: Implement instancing
  terra_mesh_t *mesh = (terra_mesh_t *)app->shapes.data;
  for (size_t i = 0; i < app->shapes.len; i++, mesh++) {
    TERRA_CALL_I(
        terra_mesh_draw(app, cmd_buffer, mesh, 1), "Failed to draw mesh"
    );
  }

  vkCmdEndRenderPass(cmd_buffer);

  TERRA_VK_CALL_I(
      vkEndCommandBuffer(cmd_buffer), "Failed recording command buffer"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t determine_swapchain_recreation(terra_app_t *app, VkResult ret) {
  switch (ret) {
  case VK_SUCCESS:
    return TERRA_STATUS_SUCCESS;

  case VK_ERROR_OUT_OF_DATE_KHR:
    logi_info("Found out of date swapchain, recreating swapchain");
    TERRA_CALL_I(
        terra_recreate_swapchain(app, NULL), "Failed recreating swapchain"
    );
    return TERRA_STATUS_SUCCESS;

  case VK_SUBOPTIMAL_KHR:
    logi_info("Found suboptimal swapchain, recreating swapchain");
    TERRA_CALL_I(
        terra_recreate_swapchain(app, NULL), "Failed recreating swapchain"
    );
    return TERRA_STATUS_SUCCESS;

  default:
    logi_warn("Failed acquiring next image");
    return TERRA_STATUS_FAILURE;
  }
}

terra_status_t terra_app_draw(terra_app_t *app) {
  TERRA_CALL_I(
      terra_vk_await_sync_objects(app), "Failed awaiting sync objects"
  );

  uint32_t frame_idx = app->state.vk_frame;
  uint32_t img_idx;

  VkCommandBuffer *cmd_buffer_p   = app->vk_command_buffers + frame_idx;
  VkSemaphore *img_available_pS   = app->vk_img_available_S + frame_idx;
  VkSemaphore *render_finished_pS = app->vk_render_finished_S + frame_idx;
  VkFence *in_flight_pF           = app->vk_in_flight_F + frame_idx;

  VkResult ret = vkAcquireNextImageKHR(
      app->vk_ldevice,
      app->vk_swapchain,
      app->conf->img_acq_timeout,
      *img_available_pS,
      VK_NULL_HANDLE,
      &img_idx
  );

  TERRA_CALL_I(
      determine_swapchain_recreation(app, ret),
      "Encountered error at image acquisition"
  );
  TERRA_CALL_I(
      terra_vk_reset_sync_objects(app), "Failed resetting sync objects"
  );

  TERRA_VK_CALL_I(
      vkResetCommandBuffer(*cmd_buffer_p, 0), "Failed resetting command buffer"
  );
  TERRA_CALL_I(
      terra_app_record_cmd_buffer(app, img_idx),
      "Failed recording command buffer"
  );

  VkSubmitInfo submit_info = {VK_FALSE};
  submit_info.sType        = VK_STRUCTURE_TYPE_SUBMIT_INFO;

  // These might need to be changed when the purpose of the application changes
  VkSemaphore *wait_semaphores   = img_available_pS;
  VkSemaphore *signal_semaphores = render_finished_pS;
  VkPipelineStageFlags stages[]  = {
      VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
  };

  submit_info.waitSemaphoreCount   = 1;
  submit_info.pWaitSemaphores      = wait_semaphores;
  submit_info.pWaitDstStageMask    = stages;
  submit_info.commandBufferCount   = 1;
  submit_info.pCommandBuffers      = cmd_buffer_p;
  submit_info.signalSemaphoreCount = 1;
  submit_info.pSignalSemaphores    = signal_semaphores;
  TERRA_VK_CALL_I(
      vkQueueSubmit(app->vk_gqueue, 1, &submit_info, *in_flight_pF),
      "Failed submitting draw command to queue"
  );

  /* Present the result of the draw call */

  VkPresentInfoKHR pres_info   = {VK_FALSE};
  pres_info.sType              = VK_STRUCTURE_TYPE_PRESENT_INFO_KHR;
  pres_info.waitSemaphoreCount = 1;
  pres_info.pWaitSemaphores    = signal_semaphores;
  pres_info.swapchainCount     = 1;
  pres_info.pSwapchains        = &app->vk_swapchain;
  pres_info.pImageIndices      = &img_idx;
  pres_info.pResults           = NULL;

  ret = vkQueuePresentKHR(app->vk_pqueue, &pres_info);
  if (app->state.fb_resized || ret == VK_ERROR_OUT_OF_DATE_KHR ||
      ret == VK_SUBOPTIMAL_KHR) {
    app->state.fb_resized = 0;
    logi_info("Framebuffer resized, recreating swapchain");
    TERRA_CALL_I(
        terra_recreate_swapchain(app, NULL), "Failed recreating swapchain"
    );
  } else if (ret != VK_SUCCESS) {
    logi_error("Found an error presenting to queue");
    return TERRA_STATUS_FAILURE;
  }

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_cleanup_swapchain(
    terra_app_t *app, VkSwapchainKHR *sc
) {
  logi_debug("Cleaning swapchain");
  VkFramebuffer *fb = app->vk_framebuffers;
  VkImageView *view = app->vk_image_views;
  for (uint32_t i = 0; i < app->vk_images_count; i++, view++, fb++) {
    vkDestroyFramebuffer(app->vk_ldevice, *fb, NULL);
    vkDestroyImageView(app->vk_ldevice, *view, NULL);
  }
  if (sc == NULL) {
    vkDestroySwapchainKHR(app->vk_ldevice, app->vk_swapchain, NULL);
  } else {
    vkDestroySwapchainKHR(app->vk_ldevice, *sc, NULL);
  }
  vkDestroyRenderPass(app->vk_ldevice, app->vk_render_pass, NULL);

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_app_cleanup(terra_app_t *app) {
  logi_debug("Terminating VMA");
  vmaDestroyAllocator(app->vma_alloc);

  logi_debug("Cleaning up sync objects");
  TERRA_CALL_I(
      terra_vk_detroy_sync_objects(app), "Failed destroying sync objects"
  );

  logi_debug("Cleaning up command pool");
  vkDestroyCommandPool(app->vk_ldevice, app->vk_commands, NULL);
  TERRA_CALL_I(
      terra_app_cleanup_swapchain(app, NULL), "Failed cleaning up swapchain"
  );

  logi_debug("Releasing heap allocated arrays");
  terrau_free(app, app->vk_img_available_S);
  terrau_free(app, app->vk_render_finished_S);
  terrau_free(app, app->vk_in_flight_F);
  terrau_free(app, app->vk_command_buffers);
  terrau_free(app, app->vk_images);
  terrau_free(app, app->vk_image_views);
  terrau_free(app, app->vk_framebuffers);
  TERRA_CALL_I(
      terra_vector_cleanup(app, &app->shapes), "Failed to clean up shapes"
  );

  logi_debug("Cleaning Vulkan objects");
  vkDestroyPipeline(app->vk_ldevice, app->vk_pipeline, NULL);
  vkDestroyPipelineLayout(app->vk_ldevice, app->vk_layout, NULL);
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
  logi_error("[VALIDATION LAYER] %s", cb_data->pMessage);
  return VK_FALSE;
}

void terra_app_fb_resize_callback(GLFWwindow *window, int width, int height) {
  logi_debug("Resized to %dx%d", width, height);
  terra_app_t *app      = (terra_app_t *)glfwGetWindowUserPointer(window);
  app->state.fb_resized = 1;
}
