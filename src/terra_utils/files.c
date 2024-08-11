#include "vendor/log.h"

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <terra/app.h>
#include <terrau/files.h>
#include <terrau/mem.h>

terra_status_t terrau_readline(
    terra_app_t *app, FILE *file, char *buffer, char **out
) {
  int alloced       = 0;
  size_t max_length = 128;
  if (buffer == NULL) {
    alloced = 1;
    logi_info("No preallocated buffer was passed, allocating one");
    buffer = terrau_malloc(app, sizeof(char) * max_length);
  }

  if (buffer == NULL) {
    logi_error("Could not allocate memory for line buffer");
    return TERRA_STATUS_FAILURE;
  }

  logi_debug("Reading first character");
  char ch      = (char)getc(file);
  size_t count = 0;

  logi_debug("Iterating");
  while ((ch != '\n') && (ch != EOF)) {
    if (count == max_length) {
      if (alloced) {
        logi_debug("Encountered max length, reallocating");
        max_length += 128;
        buffer      = terrau_realloc(app, buffer, max_length);
        if (buffer == NULL) {
          logi_error("Error reallocating space for line buffer");
          return TERRA_STATUS_FAILURE;
        }
      } else {
        logi_error("Encountered max length, failing");
        return TERRA_STATUS_FAILURE;
      }
    }
    buffer[count] = ch;
    count++;

    ch = (char)getc(file);
  }

  logi_debug("Finished reading line");
  buffer[count] = '\0';
  char line[count + 1];
  strncpy(line, buffer, count + 1);
  if (alloced)
    terrau_free(app, buffer);
  *out = line;
  return TERRA_STATUS_SUCCESS;
}
