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
terra_status_t terrar_vk_choose_sc_format(terrar_app_t *app,
                                        terrar_vk_sc_details_t *sc_details,
                                        VkSurfaceFormatKHR *out);
terra_status_t terrar_vk_choose_sc_present_mode(
    terrar_app_t *app, terrar_vk_sc_details_t *sc_details, VkPresentModeKHR *out);
terra_status_t terrar_vk_choose_sc_swap_extent(terrar_app_t *app,
                                             terrar_vk_sc_details_t *sc_details,
                                             VkExtent2D *out);
