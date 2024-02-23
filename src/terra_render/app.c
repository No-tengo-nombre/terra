#include <stdlib.h>

#include <terra/status.h>
#include <terra_utils/vendor/log.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

const char *DEFAULT_VALIDATION_LAYERS[] = {"VK_LAYER_KHRONOS_validation"};
const char *DEFAULT_DEVICE_EXTENSIONS[] = {VK_KHR_SWAPCHAIN_EXTENSION_NAME};

terrar_app_state terrar_state_default(void) {
  terrar_app_state s;
  s.i = 0;
  s.should_close = 0;
  return s;
}

terrar_app_metadata terrar_metadata_default(void) {
  terrar_app_metadata meta = {
      .vmajor = 1,
      .vminor = 0,
      .vpatch = 0,
      .app_name = "Terrar - Default application",
      .window_title = NULL,
      .window_width = 800,
      .window_height = 600,
  };
  return meta;
}

terrar_app_config terrar_config_new(const char **validation_layers,
                                    const char **device_extensions,
                                    uint32_t validation_layers_total,
                                    uint32_t device_extensions_total) {
  terrar_app_config conf = {
      .validation_layers_total = validation_layers_total,
      .device_extensions_total = device_extensions_total,
      .validation_layers = validation_layers,
      .device_extensions = device_extensions,
      .surface_format = VK_FORMAT_B8G8R8_SRGB,
      .color_space = VK_COLOR_SPACE_SRGB_NONLINEAR_KHR,
      .present_mode = VK_PRESENT_MODE_MAILBOX_KHR,
  };
  return conf;
}

terrar_app_config terrar_config_default(void) {
  return terrar_config_new(DEFAULT_VALIDATION_LAYERS, DEFAULT_DEVICE_EXTENSIONS,
                           1, 1);
}

terrar_app terrar_app_new(void *start, void *loop, void *cleanup,
                          terrar_app_metadata *meta, terrar_app_config *conf) {
  terrar_app app = {
      .start = start,
      .loop = loop,
      .cleanup = cleanup,
      .state = terrar_state_default(),
      .meta = meta,
      .conf = conf,
  };
  return app;
}

terrar_app terrar_app_new_wstate(terrar_app_state state, void *start,
                                 void *loop, void *cleanup,
                                 terrar_app_metadata *meta,
                                 terrar_app_config *conf) {
  terrar_app app = terrar_app_new(start, loop, cleanup, meta, conf);
  app.state = state;
  return app;
}

terra_status terrar_app_run(terrar_app *app) {
  logi_info("Application start");
  terra_status start_status = app->start(app);
  terra_status loop_status = TERRA_STATUS_SUCCESS;
  terra_status cleanup_status = TERRA_STATUS_SUCCESS;
  if (start_status == TERRA_STATUS_EXIT) {
    return TERRA_STATUS_SUCCESS;
  }
  if (start_status == TERRA_STATUS_FAILURE) {
    logi_fatal("Application failed on startup");
    return TERRA_STATUS_FAILURE;
  }

  if ((void *)app->loop != NULL) {
    logi_info("Application loop");
    while (loop_status == TERRA_STATUS_SUCCESS) {
      loop_status = app->loop(app);
      app->state.i++;
    }
  }
  if ((void *)app->cleanup != NULL) {
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

terra_status terrar_app_cleanup(terrar_app *app) {
  logi_debug("Cleaning Vulkan objects");
  vkDestroySurfaceKHR(app->vk_instance, app->vk_surface, NULL);
  vkDestroyDevice(app->vk_ldevice, NULL);

  logi_debug("Destroying contexts");
  vkDestroyInstance(app->vk_instance, NULL);
  glfwDestroyWindow(app->glfw_window);

  logi_debug("Terminating GLFW");
  glfwTerminate();

  return TERRA_STATUS_SUCCESS;
}

int terrar_app_should_close(terrar_app *app) {
  return glfwWindowShouldClose(app->glfw_window) || app->state.should_close;
}
