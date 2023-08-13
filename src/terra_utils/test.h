#pragma once
#include "vendor/log.h"
#include <string.h>

#define TEST_FAIL 1
#define TEST_PASS 0

#define terra_assert(cond)                                                                         \
    if (!(cond))                                                                                   \
    return TEST_FAIL
#define terra_assert_false(cond)                                                                   \
    if (cond)                                                                                      \
    return TEST_FAIL
#define terra_assert_eqi(v1, v2)                                                                   \
    if ((v1) != (v2)) {                                                                            \
        logi_debug("TEST_FAIL: Expected %i, found %i", v2, v1);                                     \
        return TEST_FAIL;                                                                          \
    }
#define terra_assert_nei(v1, v2)                                                                   \
    if ((v1) == (v2)) {                                                                            \
        logi_debug("TEST_FAIL: Expected different than %i, found %i", v2, v1);                      \
        return TEST_FAIL;                                                                          \
    }
#define terra_assert_streq(v1, v2)                                                                 \
    if (strcmp((v1), (v2))) {                                                                      \
        logi_debug("TEST_FAIL: Expected %s, found %s", v2, v1);                                     \
        return TEST_FAIL;                                                                          \
    }
#define terra_assert_strne(v1, v2)                                                                 \
    if (!strcmp((v1), (v2))) {                                                                     \
        logi_debug("TEST_FAIL: Expected different than %s, found %s", v2, v1);                      \
        return TEST_FAIL;                                                                          \
    }
#define terra_assert_chromeq(v1, v2)                                                               \
    if (!terra_ch_equal(&(v1), &(v2)))                                                             \
    return TEST_FAIL
#define terra_assert_chromne(v1, v2)                                                               \
    if (terra_ch_equal(&(v1), &(v2)))                                                              \
    return TEST_FAIL
