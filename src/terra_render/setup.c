#include <terra/status.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>
#include <terrar/setup.h>
#include <terrar/vk/devices.h>
#include <terrar/vk/swapchain.h>
#include <terrar/vulkan.h>

terra_status_t terrar_init_params_default(terrar_init_params_t *out) {
  out->image_usage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  out->view_type = VK_IMAGE_VIEW_TYPE_2D;

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_init(terrar_app_t *app, terrar_init_params_t *params) {
  terrar_init_params_t p;
  if (params == NULL) {
    TERRA_CALL_I(terrar_init_params_default(&p),
                 "Failed creating default init parameters");
  } else {
    p = *params;
  }

  TERRA_CALL_I(terrar_init_window(app), "Failed initializing window");
  TERRA_CALL_I(terrar_init_instance(app), "Failed initializing instance");
  TERRA_CALL_I(terrar_create_render_surface(app),
               "Failed creating render surface");
  TERRA_CALL_I(terrar_choose_pdevice(app), "Failed choosing physical device");
  TERRA_CALL_I(terrar_create_ldevice(app), "Failed creating logical device");
  TERRA_CALL_I(terrar_retrieve_device_queue(app),
               "Failed retrieving device queue");
  TERRA_CALL_I(terrar_vk_create_sc(app, p.image_usage, NULL),
               "Failed creating swapchain");
  TERRA_CALL_I(terrar_vk_create_image_views(app, p.view_type),
               "Failed creating image views");
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_init_window(terrar_app_t *app) {
  logi_debug("Initializing GLFW and window");
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  const char *title = app->meta->app_name;
  if (app->meta->window_title != NULL) {
    title = app->meta->window_title;
  }
  GLFWwindow *window = glfwCreateWindow(
      app->meta->window_width, app->meta->window_height, title, NULL, NULL);
  app->glfw_window = window;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_init_instance(terrar_app_t *app) {
  // Initialize Vulkan
#ifndef NDEBUG
  logi_debug("Checking validation layers support");
  if (!terrar_vk_check_validation_layer_support(app)) {
    logi_error("Validation layers required but not supported");
    return TERRA_STATUS_FAILURE;
  }
#endif
  logi_debug("Creating app info");
  VkApplicationInfo app_info;
  TERRA_CALL_I(terrar_vk_create_application_info(app, &app_info),
               "Failed creating app info");
  logi_debug("Creating instance info");
  VkInstanceCreateInfo instance_info;
  TERRA_CALL_I(terrar_vk_create_instance_info(app, &app_info, &instance_info),
               "Failed creating instance info");
  logi_debug("Creating instance");
  if (vkCreateInstance(&instance_info, NULL, &app->vk_instance) != VK_SUCCESS) {
    logi_error("Could not create Vulkan instance");
    return TERRA_STATUS_FAILURE;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_create_render_surface(terrar_app_t *app) {
  logi_debug("Creating render surface");
  if (glfwCreateWindowSurface(app->vk_instance, app->glfw_window, NULL,
                              &app->vk_surface) != VK_SUCCESS) {
    logi_error("Failed to create render surface");
    return TERRA_STATUS_FAILURE;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_choose_pdevice(terrar_app_t *app) {
  logi_debug("Choosing physical device");
  terrar_result_t result;
  TERRA_CALL_I(terrar_vk_get_physical_device(app, &result),
               "Failed getting physical device");
  app->vk_pdevice = result.value;
  app->vk_qinfo = result.queue;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_create_ldevice(terrar_app_t *app) {
  logi_debug("Creating device queue info");
  float queue_prio = 1.0f;
  VkDeviceQueueCreateInfo queue_infos[2];
  uint32_t queue_count = 1;
  // TODO: Figure out a better way to store the queue info
  if (app->vk_qinfo.gfamily == app->vk_qinfo.pfamily) {
    logi_info(
        "Creating single device queue info for graphics and presentation");
    TERRA_CALL_I(terrar_vk_create_device_queue_info(app->vk_qinfo.gfamily,
                                                    &queue_prio, queue_infos),
                 "Failed creating device queue info");
  } else {
    logi_info("Creating device queue info for graphics");
    TERRA_CALL_I(terrar_vk_create_device_queue_info(app->vk_qinfo.gfamily,
                                                    &queue_prio, queue_infos),
                 "Failed creating device graphics queue info");
    logi_info("Creating device queue info for presentation");
    TERRA_CALL_I(terrar_vk_create_device_queue_info(
                     app->vk_qinfo.pfamily, &queue_prio, queue_infos + 1),
                 "Failed creating device presentation queue info");
    queue_count = 2;
  }
  logi_debug("Using %i queues", queue_count);
  logi_debug("Creating device features");
  VkPhysicalDeviceFeatures device_features = {VK_FALSE};
  TERRA_CALL_I(terrar_vk_create_device_features(&device_features),
               "Failed creating device features");
  logi_debug("Creating logical device info");
  VkDeviceCreateInfo device_info;
  TERRA_CALL_I(terrar_vk_create_device_info(
                   queue_infos, queue_count, &device_features,
                   app->conf->device_extensions,
                   app->conf->device_extensions_total, &device_info),
               "Failed creating device info");
  if (vkCreateDevice(app->vk_pdevice, &device_info, NULL, &app->vk_ldevice) !=
      VK_SUCCESS) {
    logi_error("Could not create logical device");
    return TERRA_STATUS_FAILURE;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_retrieve_device_queue(terrar_app_t *app) {
  logi_debug(
      "Retrieving graphics queue, graphics family %i, presentation family %i",
      app->vk_qinfo.gfamily, app->vk_qinfo.pfamily);
  vkGetDeviceQueue(app->vk_ldevice, app->vk_qinfo.gfamily, 0, &app->vk_gqueue);
  vkGetDeviceQueue(app->vk_ldevice, app->vk_qinfo.pfamily, 0, &app->vk_pqueue);
  return TERRA_STATUS_SUCCESS;
}
