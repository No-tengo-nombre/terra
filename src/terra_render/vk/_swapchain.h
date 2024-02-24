#pragma once

#include <terrar/app.h>
#include <terrar/vk/swapchain.h>
#include <terrar/vulkan.h>

terra_status_t _terrar_vk_choose_sc_format(terrar_app_t *app,
                                           terrar_vk_sc_details_t *sc_details,
                                           VkSurfaceFormatKHR *out);
terra_status_t
_terrar_vk_choose_sc_present_mode(terrar_app_t *app,
                                  terrar_vk_sc_details_t *sc_details,
                                  VkPresentModeKHR *out);
terra_status_t _terrar_vk_choose_sc_swap_extent(
    terrar_app_t *app, terrar_vk_sc_details_t *sc_details, VkExtent2D *out);
terra_status_t _terrar_vk_choose_sc_image_count(
    terrar_app_t *app, terrar_vk_sc_details_t *sc_details, uint32_t *out);
