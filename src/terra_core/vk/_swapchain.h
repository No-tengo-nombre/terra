#pragma once

#include <terra/app.h>
#include <terra/vk/swapchain.h>
#include <terra/vulkan.h>

terra_status_t _terra_vk_choose_sc_format(terra_app_t *app,
                                           terra_vk_sc_details_t *sc_details,
                                           VkSurfaceFormatKHR *out);
terra_status_t
_terra_vk_choose_sc_present_mode(terra_app_t *app,
                                  terra_vk_sc_details_t *sc_details,
                                  VkPresentModeKHR *out);
terra_status_t _terra_vk_choose_sc_swap_extent(
    terra_app_t *app, terra_vk_sc_details_t *sc_details, VkExtent2D *out);
terra_status_t _terra_vk_choose_sc_image_count(
    terra_app_t *app, terra_vk_sc_details_t *sc_details, uint32_t *out);
terra_status_t
_terra_vk_choose_sc_sharing_mode(terra_app_t *app,
                                  terra_vk_sc_details_t *sc_details,
                                  VkSwapchainCreateInfoKHR *sc_info);
