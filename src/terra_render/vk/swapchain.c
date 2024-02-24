#include <stdlib.h>

#include "_swapchain.h"
#include <terra/status.h>
#include <terra_utils/macros.h>
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

terra_status_t terrar_vk_create_sc(terrar_app_t *app) {
  terrar_vk_sc_details_t sc_details;
  VkPresentModeKHR present_mode;
  VkSurfaceFormatKHR sf_format;
  VkExtent2D extent;
  TERRA_CALL_I(terrar_vk_check_sc_support(app->vk_pdevice, app->vk_surface, &sc_details), "Failed evaluating swapchain support");

  return TERRA_STATUS_SUCCESS;
}
