#pragma once
#include "vendor/log.h"

#include <string.h>
#include <terra/status.h>

#define TEST_FAIL 1
#define TEST_PASS 0

/* Assert macros */

#define TERRA_ASSERT(cond)                                                     \
  if (!(cond))                                                                 \
  return TERRA_STATUS_FAILURE
#define TERRA_ASSERT_FALSE(cond)                                               \
  if (cond)                                                                    \
  return TERRA_STATUS_FAILURE
#define TERRA_ASSERT_EQI(v1, v2)                                               \
  if ((v1) != (v2)) {                                                          \
    log_debug("TEST_FAIL: Expected %i, found %i", v2, v1);                     \
    return TERRA_STATUS_FAILURE;                                               \
  }
#define TERRA_ASSERT_NEI(v1, v2)                                               \
  if ((v1) == (v2)) {                                                          \
    log_debug("TEST_FAIL: Expected different than %i, found %i", v2, v1);      \
    return TERRA_STATUS_FAILURE;                                               \
  }
#define TERRA_ASSERT_STREQ(v1, v2)                                             \
  if (strcmp((v1), (v2))) {                                                    \
    log_debug("TEST_FAIL: Expected %s, found %s", v2, v1);                     \
    return TERRA_STATUS_FAILURE;                                               \
  }
#define TERRA_ASSERT_STRNE(v1, v2)                                             \
  if (!strcmp((v1), (v2))) {                                                   \
    log_debug("TEST_FAIL: Expected different than %s, found %s", v2, v1);      \
    return TERRA_STATUS_FAILURE;                                               \
  }
#define TERRA_ASSERT_FAILS(x)                                                  \
  if (x != TERRA_STATUS_FAILURE) {                                             \
    log_debug("TEST_FAIL: Expected failure, got success");                     \
    return TERRA_STATUS_FAILURE;                                               \
  }

/* Test utilities */

#define TERRA_CALL_TEST(app, test)                                             \
  log_info("TEST: *" #test "*");                                               \
  if (test(app) == TEST_FAIL) {                                                \
    log_error("Found failing test");                                           \
    return TEST_FAIL;                                                          \
  }
