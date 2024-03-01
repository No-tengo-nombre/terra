#include "vendor/log.h"

#include <terra/status.h>

#define TERRA_CALL_I(x, msg, ...)                                              \
  if (x != TERRA_STATUS_SUCCESS) {                                             \
    logi_error(msg, #__VA_ARGS__);                                             \
    return TERRA_STATUS_FAILURE;                                               \
  }

#define TERRA_VK_CALL_I(x, msg, ...)                                           \
  if (x != VK_SUCCESS) {                                                       \
    logi_error(msg, #__VA_ARGS__);                                             \
    return TERRA_STATUS_FAILURE;                                               \
  }
