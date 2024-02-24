#pragma once

#include <terra/status.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

typedef struct terrar_vk_sc_details {
  VkSurfaceCapabilitiesKHR capabilities;
  VkSurfaceFormatKHR *formats;
  VkPresentModeKHR *modes;
  uint32_t format_count;
  uint32_t mode_count;
} terrar_vk_sc_details_t;

terra_status_t terrar_vk_check_sc_support(VkPhysicalDevice device,
                                          VkSurfaceKHR surface,
                                          terrar_vk_sc_details_t *out);
terra_status_t terrar_vk_create_sc(terrar_app_t *out);
