#include "vendor/log.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <terra/status.h>
#include <terrau/binfile.h>

const char _HEX_MAPPING[16] = {
    '0', '1', '2', '3', '4', '5', '6', '7',
    '8', '9', 'A', 'B', 'C', 'D', 'E', 'F',
};

int64_t get_file_size(FILE *file) {
  fseek(file, 0, SEEK_END);
  return ftell(file);
}

terra_status_t terrau_read_binary_file(const char *filename, int64_t *out_size,
                                       char **out) {
  logi_debug("Reading file '%s'", filename);
  FILE *file = fopen(filename, "rb");
  if (file == NULL) {
    logi_error("Could not open file '%s'", filename);
    fclose(file);
    return TERRA_STATUS_FAILURE;
  }
  int64_t file_size = get_file_size(file);
  logi_debug("File size is %lli B, reading contents", file_size);
  fseek(file, 0, SEEK_SET);
  char *contents = malloc(file_size * sizeof(char)); // Dont forget to free
  if (contents == NULL) {
    logi_error("Could not allocate enough memory for file contents");
    fclose(file);
    return TERRA_STATUS_FAILURE;
  }

  size_t ret = fread(contents, sizeof(char), file_size, file);
  if (ret != file_size) {
    logi_error("Only read %lli/%lli B from file", ret, file_size);
    if (feof(file)) {
      logi_error("Found unexpected EOF");
    } else if (ferror(file)) {
      logi_error("Found unexpected error");
    }
    fclose(file);
    return TERRA_STATUS_FAILURE;
  }

  logi_debug("Storing contents in final buffer");
  *out_size = file_size;
  *out = contents;
  return TERRA_STATUS_SUCCESS;
}

void _uint_to_hex_str(uint8_t num, char *out) {
  out[2] = '\0';
  out[0] = _HEX_MAPPING[num / 16];
  out[1] = _HEX_MAPPING[num % 16];
}

terra_status_t terrau_visualize_binary_file(const char *contents,
                                            const int64_t size) {
  char hex[3];
  printf(
      "                | 00 01 02 03 04 05 06 07 08 09 0A 0B 0C 0D 0E 0F |\n");
  printf(
      "                +-------------------------------------------------+\n");
  for (int i = 0; i < size; i++) {
    if (i % 16 == 0) {
      if (i != 0) {
        printf("|\n%15i | ", i);
      } else {
        printf("%15i | ", i);
      }
    }
    _uint_to_hex_str((uint8_t)(*contents++), hex);
    printf("%s ", hex);
  }
  printf("\n\n");
  return TERRA_STATUS_SUCCESS;
}
