#include <stdint.h>
#include <stdlib.h>

#include "_swapchain.h"
#include <terra/status.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>
#include <terrar/app.h>
#include <terrar/vk/name_mappings.h>
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

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_vk_create_sc(terrar_app_t *app, VkImageUsageFlags usage,
                                   VkSwapchainKHR base) {
  terrar_vk_sc_details_t sc_details;
  VkSurfaceFormatKHR sf_format;
  VkPresentModeKHR present_mode;
  VkExtent2D extent;
  uint32_t image_count;

  TERRA_CALL_I(
      terrar_vk_check_sc_support(app->vk_pdevice, app->vk_surface, &sc_details),
      "Failed evaluating swapchain support");
  TERRA_CALL_I(_terrar_vk_choose_sc_format(app, &sc_details, &sf_format),
               "Failed choosing swapchain surface format");
  logi_info("Using %u:'%s' for swapchain surface format", sf_format.format,
            terrar_vk_format_name(sf_format.format));
  logi_info("Using %u:'%s' for swapchain surface color space",
            sf_format.colorSpace,
            terrar_vk_colorspace_name(sf_format.colorSpace));
  TERRA_CALL_I(
      _terrar_vk_choose_sc_present_mode(app, &sc_details, &present_mode),
      "Failed choosing swapchain presentation mode");
  TERRA_CALL_I(_terrar_vk_choose_sc_swap_extent(app, &sc_details, &extent),
               "Failed choosing swapchain swap extent");
  TERRA_CALL_I(_terrar_vk_choose_sc_image_count(app, &sc_details, &image_count),
               "Failed choosing swapchain image count");

  logi_debug("Creating swapchain creation info struct");
  VkSwapchainCreateInfoKHR sc_info = {VK_FALSE};
  sc_info.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
  sc_info.surface = app->vk_surface;
  sc_info.minImageCount = image_count;
  sc_info.imageFormat = sf_format.format;
  sc_info.imageColorSpace = sf_format.colorSpace;
  sc_info.imageExtent = extent;
  sc_info.imageArrayLayers = app->conf->image_array_layers;
  sc_info.imageUsage = usage;
  TERRA_CALL_I(_terrar_vk_choose_sc_sharing_mode(app, &sc_details, &sc_info),
               "Failed choosing queue sharing mode");
  sc_info.preTransform = sc_details.capabilities.currentTransform;
  sc_info.compositeAlpha = app->conf->composite_alpha;
  sc_info.presentMode = present_mode;
  sc_info.clipped = app->conf->clipped;
  sc_info.oldSwapchain = base;

  logi_debug("Creating swapchain");
  TERRA_VK_CALL_I(
      vkCreateSwapchainKHR(app->vk_ldevice, &sc_info, NULL, &app->vk_swapchain),
      "Failed creating swapchain");

  logi_debug("Cleaning swapchain details");
  TERRA_CALL_I(terrar_vk_sc_details_cleanup(&sc_details),
               "Failed cleaning up the swapchain details");

  logi_debug("Getting the images in the swapchain");
  TERRA_VK_CALL_I(vkGetSwapchainImagesKHR(app->vk_ldevice, app->vk_swapchain,
                                          &app->vk_images_count, NULL),
                  "Failed getting number of images");
  app->vk_images = malloc(app->vk_images_count * sizeof(VkImage));
  if (app->vk_images == NULL) {
    logi_error(
        "Could not allocate memory for %i vulkan images in the swapchain",
        app->vk_images_count);
    return TERRA_STATUS_FAILURE;
  }
  TERRA_VK_CALL_I(vkGetSwapchainImagesKHR(app->vk_ldevice, app->vk_swapchain,
                                          &app->vk_images_count,
                                          app->vk_images),
                  "Failed getting images");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terrar_vk_sc_details_cleanup(terrar_vk_sc_details_t *dets) {
  free(dets->formats);
  free(dets->modes);
  return TERRA_STATUS_SUCCESS;
}
