#pragma once
#include "log.h"

#include <terra/status.h>

#define TERRA_CALL(x, msg, ...)                                                \
  if (x != TERRA_STATUS_SUCCESS) {                                             \
    log_error(msg, #__VA_ARGS__);                                              \
    return TERRA_STATUS_FAILURE;                                               \
  }

#define TERRA_CALLDF(x, msg, ...)                                              \
  if (x != TERRA_STATUS_SUCCESS) {                                             \
    log_error(msg, #__VA_ARGS__);                                              \
  }
