#pragma once

#include <terra/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

const char *terra_vk_format_name(VkFormat format);
const char *terra_vk_colorspace_name(VkColorSpaceKHR color_space);

#ifdef __cplusplus
}
#endif
