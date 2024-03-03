#include <terra/setup.h>
#include <terra/status.h>
#include <terra/vk/devices.h>
#include <terra/vk/swapchain.h>
#include <terra/vulkan.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>

terra_status_t terra_init_params_default(terra_init_params_t *out) {
  out->image_usage      = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;
  out->view_type        = VK_IMAGE_VIEW_TYPE_2D;
  out->samples          = VK_SAMPLE_COUNT_1_BIT;
  out->load_op          = VK_ATTACHMENT_LOAD_OP_CLEAR;
  out->store_op         = VK_ATTACHMENT_STORE_OP_STORE;
  out->stencil_load_op  = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
  out->stencil_store_op = VK_ATTACHMENT_STORE_OP_DONT_CARE;
  out->initial_layout   = VK_IMAGE_LAYOUT_UNDEFINED;
  out->final_layout     = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_init(terra_app_t *app, terra_init_params_t *params) {
  terra_init_params_t p;
  if (params == NULL) {
    TERRA_CALL_I(
        terra_init_params_default(&p), "Failed creating default init parameters"
    );
  } else {
    p = *params;
  }

  TERRA_CALL_I(terra_init_window(app), "Failed initializing window");
  TERRA_CALL_I(terra_init_instance(app), "Failed initializing instance");
  TERRA_CALL_I(
      terra_create_render_surface(app), "Failed creating render surface"
  );
  TERRA_CALL_I(terra_choose_pdevice(app), "Failed choosing physical device");
  TERRA_CALL_I(terra_create_ldevice(app), "Failed creating logical device");
  TERRA_CALL_I(
      terra_retrieve_device_queue(app), "Failed retrieving device queue"
  );
  TERRA_CALL_I(
      terra_vk_create_sc(app, p.image_usage, NULL), "Failed creating swapchain"
  );
  TERRA_CALL_I(
      terra_vk_create_image_views(app, p.view_type),
      "Failed creating image views"
  );
  TERRA_CALL_I(
      terra_create_render_pass(app, &p), "Failed creating render pass"
  );
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_init_window(terra_app_t *app) {
  logi_debug("Initializing GLFW and window");
  glfwInit();
  glfwWindowHint(GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);
  const char *title = app->meta->app_name;
  if (app->meta->window_title != NULL) {
    title = app->meta->window_title;
  }
  GLFWwindow *window = glfwCreateWindow(
      app->meta->window_width, app->meta->window_height, title, NULL, NULL
  );
  app->glfw_window = window;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_init_instance(terra_app_t *app) {
  // Initialize Vulkan
#ifndef NDEBUG
  logi_debug("Checking validation layers support");
  if (!terra_vk_check_validation_layer_support(app)) {
    logi_error("Validation layers required but not supported");
    return TERRA_STATUS_FAILURE;
  }
#endif
  logi_debug("Creating app info");
  VkApplicationInfo app_info;
  TERRA_CALL_I(
      terra_vk_create_application_info(app, &app_info),
      "Failed creating app info"
  );
  logi_debug("Creating instance info");
  VkInstanceCreateInfo instance_info;
  TERRA_CALL_I(
      terra_vk_create_instance_info(app, &app_info, &instance_info),
      "Failed creating instance info"
  );
  logi_debug("Creating instance");
  TERRA_VK_CALL_I(
      vkCreateInstance(&instance_info, NULL, &app->vk_instance),
      "Failed to create Vulkan instance"
  );
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_init_debug_callback(terra_app_t *app) {
#ifndef NDEBUG
  if (app->conf->validation_layers_total == 0) {
    return TERRA_STATUS_SUCCESS;
  }

  VkDebugUtilsMessengerCreateInfoEXT info = {VK_FALSE};
  info.sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT;
  info.messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_VERBOSE_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT |
                         VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT;
  info.messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT |
                     VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT;
  info.pfnUserCallback                    = terra_app_debug_callback;
  PFN_vkCreateDebugUtilsMessengerEXT func = (PFN_vkCreateDebugUtilsMessengerEXT
  )vkGetInstanceProcAddr(app->vk_instance, "vkCreateDebugUtilsMessengerEXT");
  if (func == NULL) {
    return TERRA_STATUS_FAILURE;
  }
  TERRA_VK_CALL_I(
      func(app->vk_instance, &info, NULL, &app->_idebug_messenger),
      "Failed setting up messenger"
  );

#endif
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_create_render_surface(terra_app_t *app) {
  logi_debug("Creating render surface");
  TERRA_VK_CALL_I(
      glfwCreateWindowSurface(
          app->vk_instance, app->glfw_window, NULL, &app->vk_surface
      ),
      "Failed to create render surface"
  );
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_choose_pdevice(terra_app_t *app) {
  logi_debug("Choosing physical device");
  terra_result_t result;
  TERRA_CALL_I(
      terra_vk_get_physical_device(app, &result),
      "Failed getting physical device"
  );
  app->vk_pdevice = result.value;
  app->vk_qinfo   = result.queue;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_create_ldevice(terra_app_t *app) {
  logi_debug("Creating device queue info");
  float queue_prio = 1.0f;
  VkDeviceQueueCreateInfo queue_infos[2];
  uint32_t queue_count = 1;
  // TODO: Figure out a better way to store the queue info
  if (app->vk_qinfo.gfamily == app->vk_qinfo.pfamily) {
    logi_info("Creating single device queue info for graphics and presentation"
    );
    TERRA_CALL_I(
        terra_vk_create_device_queue_info(
            app->vk_qinfo.gfamily, &queue_prio, queue_infos
        ),
        "Failed creating device queue info"
    );
  } else {
    logi_info("Creating device queue info for graphics");
    TERRA_CALL_I(
        terra_vk_create_device_queue_info(
            app->vk_qinfo.gfamily, &queue_prio, queue_infos
        ),
        "Failed creating device graphics queue info"
    );
    logi_info("Creating device queue info for presentation");
    TERRA_CALL_I(
        terra_vk_create_device_queue_info(
            app->vk_qinfo.pfamily, &queue_prio, queue_infos + 1
        ),
        "Failed creating device presentation queue info"
    );
    queue_count = 2;
  }
  logi_debug("Using %i queues", queue_count);
  logi_debug("Creating device features");
  VkPhysicalDeviceFeatures device_features = {VK_FALSE};
  TERRA_CALL_I(
      terra_vk_create_device_features(&device_features),
      "Failed creating device features"
  );
  logi_debug("Creating logical device info");
  VkDeviceCreateInfo device_info;
  TERRA_CALL_I(
      terra_vk_create_device_info(
          queue_infos,
          queue_count,
          &device_features,
          app->conf->device_extensions,
          app->conf->device_extensions_total,
          &device_info
      ),
      "Failed creating device info"
  );
  if (vkCreateDevice(app->vk_pdevice, &device_info, NULL, &app->vk_ldevice) !=
      VK_SUCCESS) {
    logi_error("Could not create logical device");
    return TERRA_STATUS_FAILURE;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_retrieve_device_queue(terra_app_t *app) {
  logi_debug(
      "Retrieving graphics queue, graphics family %i, presentation family %i",
      app->vk_qinfo.gfamily,
      app->vk_qinfo.pfamily
  );
  vkGetDeviceQueue(app->vk_ldevice, app->vk_qinfo.gfamily, 0, &app->vk_gqueue);
  vkGetDeviceQueue(app->vk_ldevice, app->vk_qinfo.pfamily, 0, &app->vk_pqueue);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_create_render_pass(
    terra_app_t *app, terra_init_params_t *params
) {
  logi_debug("Creating render passes");
  VkAttachmentDescription color_att = {VK_FALSE};
  color_att.format                  = app->vk_format;
  color_att.samples                 = params->samples;
  color_att.loadOp                  = params->load_op;
  color_att.storeOp                 = params->store_op;
  color_att.stencilLoadOp           = params->stencil_load_op;
  color_att.stencilStoreOp          = params->stencil_store_op;
  color_att.initialLayout           = params->initial_layout;
  color_att.finalLayout             = params->final_layout;

  VkAttachmentReference color_att_ref = {VK_FALSE};
  color_att_ref.attachment            = 0;
  color_att_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;

  VkSubpassDescription subpass = {VK_FALSE};
  subpass.pipelineBindPoint    = VK_PIPELINE_BIND_POINT_GRAPHICS;
  subpass.colorAttachmentCount = 1;
  subpass.pColorAttachments    = &color_att_ref;
  // TODO: Implement more types of attachments

  VkSubpassDependency dependency = {VK_FALSE};
  dependency.srcSubpass          = VK_SUBPASS_EXTERNAL;
  dependency.dstSubpass          = 0;
  dependency.srcStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.srcAccessMask = 0;
  dependency.dstStageMask  = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
  dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

  // TODO: Implement the option to specify more than one attachment and more
  // than one subpass
  VkRenderPassCreateInfo info = {VK_FALSE};
  info.sType                  = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
  info.attachmentCount        = 1;
  info.pAttachments           = &color_att;
  info.subpassCount           = 1;
  info.pSubpasses             = &subpass;
  info.dependencyCount        = 1;
  info.pDependencies          = &dependency;

  logi_debug("Vulkan call to create render pass");
  TERRA_VK_CALL_I(
      vkCreateRenderPass(app->vk_ldevice, &info, NULL, &app->vk_render_pass),
      "Failed to create the render pass"
  );

  return TERRA_STATUS_SUCCESS;
}
