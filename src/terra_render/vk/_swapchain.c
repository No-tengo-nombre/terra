#include <terra/status.h>
#include <terra_utils/vendor/log.h>
#include <terrar/app.h>
#include <terrar/vk/name_mappings.h>
#include <terrar/vk/swapchain.h>
#include <terrar/vulkan.h>

terra_status_t _terrar_vk_choose_sc_format(terrar_app_t *app,
                                           terrar_vk_sc_details_t *sc_details,
                                           VkSurfaceFormatKHR *out) {
  logi_debug("Desired swapchain format is %u:'%s'", app->conf->surface_format,
             terrar_vk_format_name(app->conf->surface_format));
  logi_debug("Desired swapchain colorspace is %u:'%s'", app->conf->color_space,
             terrar_vk_colorspace_name(app->conf->color_space));
  for (int i = 0; i < sc_details->format_count; i++) {
    VkSurfaceFormatKHR f = sc_details->formats[i];
    if (f.format == app->conf->surface_format &&
        f.colorSpace == app->conf->color_space) {
      *out = f;
      return TERRA_STATUS_SUCCESS;
    }
  }
  logi_warn("Desired surface format not found, defaulting to first");
  *out = sc_details->formats[0];
  return TERRA_STATUS_SUCCESS;
}

terra_status_t
_terrar_vk_choose_sc_present_mode(terrar_app_t *app,
                                  terrar_vk_sc_details_t *sc_details,
                                  VkPresentModeKHR *out) {
  for (int i = 0; i < sc_details->mode_count; i++) {
    VkPresentModeKHR m = sc_details->modes[i];
    if (m == app->conf->present_mode) {
      *out = m;
      return TERRA_STATUS_SUCCESS;
    }
  }
  logi_warn("Desired present mode not found, defaulting to FIFO");
  *out = VK_PRESENT_MODE_FIFO_KHR;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terrar_vk_choose_sc_swap_extent(
    terrar_app_t *app, terrar_vk_sc_details_t *sc_details, VkExtent2D *out) {
  VkSurfaceCapabilitiesKHR c = sc_details->capabilities;
  if (c.currentExtent.width != UINT32_MAX) {
    *out = c.currentExtent;
    return TERRA_STATUS_SUCCESS;
  } else {
    int w, h;
    glfwGetFramebufferSize(app->glfw_window, &w, &h);
    VkExtent2D extent = {
        terrau_clamp_u32((uint32_t)w, c.minImageExtent.width,
                         c.maxImageExtent.width),
        terrau_clamp_u32((uint32_t)h, c.minImageExtent.height,
                         c.maxImageExtent.height),
    };
    *out = extent;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terrar_vk_choose_sc_image_count(
    terrar_app_t *app, terrar_vk_sc_details_t *sc_details, uint32_t *out) {
  uint32_t count = sc_details->capabilities.minImageCount + 1;
  if (sc_details->capabilities.maxImageCount > 0 &&
      count > sc_details->capabilities.maxImageCount) {
    count = sc_details->capabilities.maxImageCount;
  }
  *out = count;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t
_terrar_vk_choose_sc_sharing_mode(terrar_app_t *app,
                                  terrar_vk_sc_details_t *sc_details,
                                  VkSwapchainCreateInfoKHR *sc_info) {
  if (app->vk_qinfo.gfamily != app->vk_qinfo.pfamily) {
    // TODO: Implement ownership when using different graphics and present
    // queues
    logi_debug("Using concurrent sharing mode with two queues");
    sc_info->imageSharingMode = VK_SHARING_MODE_CONCURRENT;
    sc_info->queueFamilyIndexCount = 2;
    // This relies on struct alignment, so I might change it
    sc_info->pQueueFamilyIndices = &app->vk_qinfo.gfamily;
  } else {
    logi_debug("Using exclusive sharing mode with single queue");
    sc_info->imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
    sc_info->queueFamilyIndexCount = 0;
    sc_info->pQueueFamilyIndices = NULL;
  }

  return TERRA_STATUS_SUCCESS;
}
