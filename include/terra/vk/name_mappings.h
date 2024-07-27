#pragma once

#include <terra/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *terra_vk_format_name(VkFormat format);
const char *terra_vk_colorspace_name(VkColorSpaceKHR color_space);
const char *terra_vk_present_mode_name(VkPresentModeKHR present_mode);
const char *terra_vk_result_name(VkResult res);
const char *terra_vk_version_name(uint32_t vk_version);

#ifdef __cplusplus
}
#endif
