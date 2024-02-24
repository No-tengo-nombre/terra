#include <stdlib.h>

#include <terra/status.h>
#include <terra_utils/vendor/log.h>
#include <terrar/app.h>
#include <terrar/vk/swapchain.h>

terra_status_t terrar_vk_check_sc_support(VkPhysicalDevice device,
                                        VkSurfaceKHR surface,
                                        terrar_vk_sc_details_t *out) {
  logi_debug("Querying surface capabilities");
  vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,
                                            &out->capabilities);

  logi_debug("Querying surface formats");
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &out->format_count,
                                       NULL);
  out->formats = malloc(out->format_count * sizeof(VkSurfaceFormatKHR));
  if (out->formats == NULL) {
    logi_error("Could not allocate memory for surface formats");
    return TERRA_STATUS_FAILURE;
  }
  vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &out->format_count,
                                       out->formats);

  logi_debug("Querying surface present modes");
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &out->mode_count,
                                            NULL);
  out->modes = malloc(out->mode_count * sizeof(VkPresentModeKHR));
  if (out->modes == NULL) {
    logi_error("Could not allocate memory for surface present modes");
    return TERRA_STATUS_FAILURE;
  }
  vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &out->mode_count,
                                            out->modes);

  free(out->formats);
  free(out->modes);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_vk_choose_sc_format(terrar_app_t *app,
                                        terrar_vk_sc_details_t *sc_details,
                                        VkSurfaceFormatKHR *out) {
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
terrar_vk_choose_sc_present_mode(terrar_app_t *app,
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

terra_status_t terrar_vk_choose_sc_swap_extent(
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
