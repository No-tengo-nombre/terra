#pragma once

#include <terra/status.h>
#include <terra/app.h>
#include <terra/vulkan.h>

typedef struct terra_vk_sc_details {
  VkSurfaceCapabilitiesKHR capabilities;
  VkSurfaceFormatKHR *formats;
  VkPresentModeKHR *modes;
  uint32_t format_count;
  uint32_t mode_count;
} terra_vk_sc_details_t;

terra_status_t terra_vk_check_sc_support(VkPhysicalDevice device,
                                          VkSurfaceKHR surface,
                                          terra_vk_sc_details_t *out);
terra_status_t terra_vk_create_sc(terra_app_t *app, VkImageUsageFlags usage,
                                   VkSwapchainKHR base);
terra_status_t terra_vk_create_image_views(terra_app_t *app,
                                            VkImageViewType view_type);

terra_status_t terra_vk_sc_details_cleanup(terra_vk_sc_details_t *dets);
