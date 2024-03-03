#pragma once

#ifdef __cplusplus
extern "C" {
#endif

// TODO: Evaluate removal of runtime error detection on release builds

typedef enum terra_status {
  TERRA_STATUS_SUCCESS = 0,
  TERRA_STATUS_FAILURE = 1,
  TERRA_STATUS_EXIT    = 2,
} terra_status_t;

#ifdef __cplusplus
}
#endif
