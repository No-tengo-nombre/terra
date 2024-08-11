#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <terra/app.h>
#include <terra/data/vector.h>
#include <terra/status.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>
#include <terrau/files.h>
#include <terrau/formats/descriptors.h>
#include <terrau/formats/off.h>
#include <terrau/mem.h>

terra_status_t _load_off_file(
    terra_app_t *app,
    FILE *f,
    char *line_buffer,
    terra_vector_t *verts,
    terra_vector_t *idx
);

terra_status_t terrau_load_off(
    terra_app_t *app, const char *filename, terrau_mesh_descriptor_t *desc
) {
  FILE *f           = fopen(filename, "r");
  char *line_buffer = terrau_malloc(app, sizeof(char) * 128);
  if (f == NULL) {
    logi_error("Could not open file '%s'", filename);
    return TERRA_STATUS_FAILURE;
  }

  // TODO: Implement different vertex formats
  // terrau_mesh_descriptor_t out;
  // if (_load_off_file(app, f, line_buffer, out.verts, out.idx) !=
  // TERRA_STATUS_SUCCESS) {
  //   fclose(f);
  //   terrau_free(app, line_buffer);
  //   logi_error("Failed reading file");
  //   return TERRA_STATUS_FAILURE;
  // }

  // *desc = out;
  fclose(f);
  terrau_free(app, line_buffer);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _load_off_file(
    terra_app_t *app,
    FILE *f,
    char *line_buffer,
    terra_vector_t *verts,
    terra_vector_t *idx
) {
  // NOTE: This function assumes verts and idx have not been initialized
  char *buffer;

  // Ensure valid header
  TERRA_CALL_I(
      terrau_readline(app, f, line_buffer, &buffer), "Failed reading header"
  );
  if (strncmp(buffer, "OFF", 3) != 0) {
    terrau_free(app, line_buffer);
    logi_error("Invalid file format");
    return TERRA_STATUS_FAILURE;
  }

  // TERRA_CALL_I(terrau_readline(app, f, &buffer))
  // while (strncmp(buffer, "#", 1) == 0) {

  // }

  // TODO: Finish implementing importing OFF files
  terrau_free(app, line_buffer);
  logi_error("Importing OFF files not implemented");
  return TERRA_STATUS_FAILURE;
}
