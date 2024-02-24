#include <terra/status.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

typedef struct terrar_swapchain_details {
  VkSurfaceCapabilitiesKHR capabilities;
  VkSurfaceFormatKHR *formats;
  VkPresentModeKHR *modes;
  uint32_t format_count;
  uint32_t mode_count;
} terrar_swapchain_details;

terra_status terrar_vk_check_sc_support(VkPhysicalDevice device,
                                        VkSurfaceKHR surface,
                                        terrar_swapchain_details *out);
terra_status terrar_vk_choose_sc_format(terrar_app *app,
                                        terrar_swapchain_details *sc_details,
                                        VkSurfaceFormatKHR *out);
terra_status
terrar_vk_choose_sc_present_mode(terrar_app *app,
                                 terrar_swapchain_details *sc_details,
                                 VkPresentModeKHR *out);
terra_status terrar_vk_choose_sc_swap_extent(
    terrar_app *app, terrar_swapchain_details *sc_details, VkExtent2D *out);
