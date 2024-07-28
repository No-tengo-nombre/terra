#include <stdio.h>

#include <terra/terra.h>
#include <terra_utils/test.h>
#include <terrau/log.h>
#include <terrau/macros.h>
#include <terrau/files.h>

terra_status_t test_readline(terra_app_t *app) {
  log_debug("Opening file");
  const char *FILENAME = TERRA_PROJECT_ROOT_DIR "resources/tests/test_files.txt";
  FILE *f = fopen(FILENAME, "r");
  if (f == NULL) {
    logi_error("Could not open file '%s'", FILENAME);
    return TERRA_STATUS_FAILURE;
  }
  char *line;

  log_debug("Reading line");
  TERRA_CALL(terrau_readline(app, f, &line), "Failed reading line");
  TERRA_ASSERT_STREQ(line, "Hello world!");
  log_debug("Reading line");
  TERRA_CALL(terrau_readline(app, f, &line), "Failed reading line");
  TERRA_ASSERT_STREQ(line, "This is a file for testing purposes");
  log_debug("Reading line");
  TERRA_CALL(terrau_readline(app, f, &line), "Failed reading line");
  TERRA_ASSERT_STREQ(line, "Lorem ipsum and stuff");
  log_debug("Reading line");
  TERRA_CALL(terrau_readline(app, f, &line), "Failed reading line");
  TERRA_ASSERT_STREQ(line, "");

  return TERRA_STATUS_SUCCESS;
}

terra_status_t start(terra_app_t *app) {
  TERRA_CALL_TEST(app, test_readline);
  return TERRA_STATUS_SUCCESS;
}

int main(void) {
  terra_app_metadata_t meta = terra_app_metadata_default();
  meta.app_name             = "Terra (test)";

  terra_app_config_t conf = terra_app_config_default();
  conf.log_dir            = "logs";
  conf.log_stdlvl         = LOG_DEBUG;

  terra_app_t app;
  terra_app_new(&start, NULL, NULL, &meta, &conf, &app);
  TERRA_CALL(terra_app_run(&app), "Failed test");

  return TEST_PASS;
}

