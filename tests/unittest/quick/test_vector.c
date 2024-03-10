#include <stddef.h>
#include <stdint.h>
#include <terra/terra.h>
#include <terra_utils/test.h>
#include <terrau/log.h>
#include <terrau/macros.h>

terra_status_t test_creation_new(terra_app_t *app) {
  terra_vector_t vec;

  log_debug("Creating vector");
  TERRA_CALL(
      terra_vector_new(app, sizeof(uint32_t), &vec), "Failed creating vector"
  );

  log_debug("Checking creation result");
  TERRA_ASSERT_EQI(vec.data, NULL);
  TERRA_ASSERT_EQI(vec.capacity, 0);
  TERRA_ASSERT_EQI(vec.len, 0);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t test_creation_zero(terra_app_t *app) {
  terra_vector_t vec;
  size_t len = 10;

  log_info("Creating vector");
  TERRA_CALL(
      terra_vector_zero(app, len, sizeof(uint32_t), &vec),
      "Failed creating vector"
  );

  log_debug("Checking creation result");
  for (uint32_t i = 0; i < len; i++) {
    TERRA_ASSERT_EQI(*(uint32_t *)((char *)vec.data + i * sizeof(uint32_t)), 0);
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t test_creation_with_capacity(terra_app_t *app) {
  terra_vector_t vec;

  log_debug("Creating vector");
  TERRA_CALL(
      terra_vector_with_capacity(app, 6, sizeof(uint32_t), &vec),
      "Failed creating vector"
  );

  log_debug("Checking creation result");
  TERRA_ASSERT_NEI(vec.data, NULL);
  TERRA_ASSERT_NEI(vec.capacity, 0);
  TERRA_ASSERT_EQI(vec.data_size, sizeof(uint32_t));
  TERRA_ASSERT_EQI(vec.len, 0);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t test_creation_from_array(terra_app_t *app) {
  terra_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len      = 8;

  log_info("Creating vector");
  TERRA_CALL(
      terra_vector_from_array(app, data, len, sizeof(uint32_t), &vec),
      "Failed creating vector"
  );

  log_debug("Checking creation result");
  for (uint32_t i = 0; i < len; i++) {
    TERRA_ASSERT_EQI(
        *(uint32_t *)((char *)vec.data + i * sizeof(uint32_t)), data[i]
    );
  }

  return TERRA_STATUS_SUCCESS;
}

terra_status_t test_methods_get(terra_app_t *app) {
  terra_vector_t vec;
  uint32_t data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t len      = 8;

  log_debug("Creating vector");
  TERRA_CALL(
      terra_vector_from_array(app, data, len, sizeof(uint32_t), &vec),
      "Failed creating vector"
  );

  log_debug("Checking all the elements");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    TERRA_CALL(
        terra_vector_get(app, &vec, i, &val), "Failed getting vec[%d]", i
    );
    TERRA_ASSERT_EQI(val, data[i]);
  }

  log_debug("Checking border scenarios");
  TERRA_ASSERT_FAILS(terra_vector_get(app, &vec, len, &val));

  // Check that the failed call to terra_vector_get did not accidentally
  // modify val
  TERRA_ASSERT_EQI(val, data[len - 1]);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t test_methods_set(terra_app_t *app) {
  terra_vector_t vec;
  uint32_t data[] = {7, 6, 5, 4, 3, 2, 1, 0};
  size_t len      = 8;

  log_debug("Creating vector");
  TERRA_CALL(
      terra_vector_from_array(app, data, len, sizeof(uint32_t), &vec),
      "Failed creating vector"
  );

  log_debug("Checking all the elements before setting");
  uint32_t val;
  for (uint32_t i = 0; i < len; i++) {
    TERRA_CALL(
        terra_vector_get(app, &vec, i, &val), "Failed getting vec[%d]", i
    );
    TERRA_ASSERT_EQI(val, data[i]);
  }

  log_debug("Changing two elements");
  size_t i0     = 2;
  size_t i1     = 6;
  uint32_t new0 = 20;
  uint32_t new1 = 1881;
  TERRA_CALL(terra_vector_set(app, &vec, i0, &new0), "Failed setting elements");
  TERRA_CALL(terra_vector_set(app, &vec, i1, &new1), "Failed setting elements");

  log_debug("Checking elements after setting");
  for (uint32_t i = 0; i < len; i++) {
    TERRA_CALL(
        terra_vector_get(app, &vec, i, &val), "Failed getting vec[%d]", i
    );
    if (i == i0) {
      TERRA_ASSERT_EQI(val, new0);
    } else if (i == i1) {
      TERRA_ASSERT_EQI(val, new1);
    } else {
      TERRA_ASSERT_EQI(val, data[i]);
    }
  }

  log_debug("Checking border scenarios");
  TERRA_ASSERT_FAILS(terra_vector_set(app, &vec, len - 1, NULL));

  // Check that the failed call to terra_vector_get did not accidentally
  // modify val
  TERRA_ASSERT_EQI(val, data[len - 1]);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t test_modification_push1(terra_app_t *app) {
  terra_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len      = sizeof(data) / sizeof(uint32_t);

  log_debug("Creating vector");
  TERRA_CALL(
      terra_vector_from_array(app, data, len, sizeof(uint32_t), &vec),
      "Failed creating vector"
  );
  TERRA_ASSERT_EQI(vec.capacity, 8);

  log_debug("Pushing element");
  uint32_t val = 20;
  TERRA_CALL(terra_vector_push(app, &vec, &val), "Failed pushing element");

  log_debug("Checking that the dimensions changed properly");
  TERRA_ASSERT_EQI(vec.len, 9);
  TERRA_ASSERT_EQI(vec.capacity, 16);

  log_debug("Checking border scenario");
  TERRA_ASSERT_FAILS(terra_vector_push(app, &vec, NULL));
  log_debug("Checking that the failed push does not modify the vector");
  TERRA_ASSERT_EQI(vec.len, 9);
  TERRA_ASSERT_EQI(vec.capacity, 16);

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  TERRA_CALL(
      terra_vector_get(app, &vec, 8, &contained_val), "Failed getting vec[8]"
  );
  TERRA_ASSERT_EQI(contained_val, val);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t test_modification_push2(terra_app_t *app) {
  terra_vector_t vec;
  size_t capacity = 8;

  log_debug("Creating vector");
  TERRA_CALL(
      terra_vector_with_capacity(app, capacity, sizeof(uint32_t), &vec),
      "Failed creating vector"
  );

  log_debug("Pushing element");
  uint32_t val = 20;
  TERRA_CALL(terra_vector_push(app, &vec, &val), "Failed pushing element");

  log_debug("Checking that the dimensions changed properly");
  TERRA_ASSERT_EQI(vec.len, 1);
  TERRA_ASSERT_EQI(vec.capacity, capacity);

  log_debug("Checking that the push happened fine");
  uint32_t contained_val;
  TERRA_CALL(
      terra_vector_get(app, &vec, 0, &contained_val), "Failed getting vec[0]"
  );
  TERRA_ASSERT_EQI(contained_val, val);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t test_modification_extend_array(terra_app_t *app) {
  terra_vector_t vec;
  uint32_t data[] = {0, 1, 2, 3, 4, 5, 6, 7};
  size_t len      = 8;

  log_debug("Creating vector");
  TERRA_CALL(
      terra_vector_from_array(app, data, len, sizeof(uint32_t), &vec),
      "Failed creating vector"
  );

  log_debug("Extending vector");
  uint32_t values[] = {20, 19, 15};
  size_t values_len = 3;
  TERRA_CALL(
      terra_vector_extend_array(app, &vec, &values, values_len),
      "Failed extending array"
  );

  log_debug("Checking that the dimensions changed properly");
  TERRA_ASSERT_EQI(vec.len, 11);
  TERRA_ASSERT_EQI(vec.capacity, 11);

  log_debug("Checking border scenario");
  TERRA_ASSERT_FAILS(terra_vector_extend_array(app, &vec, NULL, 10010));
  log_debug("Checking that the failed extend does not modify the vector");
  TERRA_ASSERT_EQI(vec.len, 11);
  TERRA_ASSERT_EQI(vec.capacity, 11);

  log_debug("Checking that the extend happened fine");
  uint32_t contained_val;
  for (uint32_t i = 0; i < len; i++) {
    TERRA_CALL(
        terra_vector_get(app, &vec, i, &contained_val),
        "Failed getting vec[%d]",
        i
    );
    TERRA_ASSERT_EQI(contained_val, data[i]);
  }
  for (uint32_t i = 0; i < values_len; i++) {
    TERRA_CALL(
        terra_vector_get(app, &vec, i + len, &contained_val),
        "Failed getting vec[%d]",
        i + len
    );
    TERRA_ASSERT_EQI(contained_val, values[i]);
  }
  log_debug("Found no issues");
  return TERRA_STATUS_SUCCESS;
}

terra_status_t start(terra_app_t *app) {
  TERRA_CALL(terra_init(app, NULL), "Failed initializing app");
  TERRA_CALL(terra_vk_framebuffer_new(app), "Failed creating framebuffers");
  TERRA_CALL(terra_vk_command_pool_new(app), "Failed creating command pool");
  TERRA_CALL(terra_vk_create_sync_objects(app), "Failed creating sync objects");

  TERRA_CALL_TEST(app, test_creation_new);
  TERRA_CALL_TEST(app, test_creation_zero);
  TERRA_CALL_TEST(app, test_creation_with_capacity);
  TERRA_CALL_TEST(app, test_creation_from_array);

  TERRA_CALL_TEST(app, test_methods_get);
  TERRA_CALL_TEST(app, test_methods_set);

  TERRA_CALL_TEST(app, test_modification_push1);
  TERRA_CALL_TEST(app, test_modification_push2);
  TERRA_CALL_TEST(app, test_modification_extend_array);
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terra_app_metadata_t meta = terra_app_metadata_default();
  meta.app_name             = "Terra (test)";
  terra_app_config_t conf   = terra_app_config_default();

  terra_app_t app;
  terra_app_new(&start, NULL, NULL, &meta, &conf, &app);
  TERRA_CALL(terra_app_run(&app), "Failed test");

  return TEST_PASS;
}
