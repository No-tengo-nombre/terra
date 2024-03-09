#pragma once

#include "vendor/log.h"

#include <terra/status.h>
#include <terra/vk/name_mappings.h>

#define TERRA_CALL_I(x, msg, ...)                                              \
  if (x != TERRA_STATUS_SUCCESS) {                                             \
    logi_error(msg, #__VA_ARGS__);                                             \
    return TERRA_STATUS_FAILURE;                                               \
  }

#define TERRA_VK_CALL_I(x, msg, ...)                                           \
  {                                                                            \
    VkResult err_ = x;                                                         \
    if (err_ != VK_SUCCESS) {                                                  \
      const char *err_str = terra_vk_result_name(err_);                        \
      logi_error(msg, #__VA_ARGS__);                                           \
      logi_error("Vulkan error [%d]'%s'", err_, err_str);                      \
      return TERRA_STATUS_FAILURE;                                             \
    }                                                                          \
  }
