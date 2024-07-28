#include <stdint.h>

#include <terrau/files.h>
#include "vendor/log.h"

terra_status_t terrau_readline(FILE *f, char **out) {
  uint32_t max_length = 128;
  char *buffer = malloc(sizeof(char) * max_length);

  if (buffer == NULL) {
      printf("Error allocating memory for line buffer.");
      exit(1);
  }

  char ch = getc(file);
  int count = 0;

  while ((ch != '\n') && (ch != EOF)) {
    if (count == max_length) {
      max_length += 128;
      buffer = realloc(buffer, max_length);
      if (buffer == NULL) {
        logi_error("Error reallocating space for line buffer");
        return TERRA_STATUS_FAILURE;
      }
    }
    buffer[count] = ch;
    count++;

    ch = getc(file);
  }

  buffer[count] = '\0';
  char line[count + 1];
  strncpy(line, buffer, count + 1);
  free(buffer);
  *out = line;
  return TERRA_STATUS_SUCCESS;
}

