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
  terrau_mesh_descriptor_t out;
  if (_load_off_file(app, f, line_buffer, out.verts, out.idx) !=
      TERRA_STATUS_SUCCESS) {
    fclose(f);
    terrau_free(app, line_buffer);
    logi_error("Failed reading file");
    return TERRA_STATUS_FAILURE;
  }

  *desc = out;
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
  // TODO: Consider comments within the file
  char *buffer;

  // Ensure valid header
  TERRA_CALL_I(
      terrau_readline(app, f, line_buffer, &buffer), "Failed reading header"
  );
  if (strncmp(buffer, "OFF", 3) != 0) {
    logi_error("Invalid file format");
    return TERRA_STATUS_FAILURE;
  }

  // Read number of vertices and faces
  size_t num_verts, num_faces, num_edges;
  TERRA_CALL_I(terrau_readline(app, f, line_buffer, &buffer), "Failed reading line");
  if (sscanf(buffer, "%zu %zu %zu", &num_verts, &num_faces, &num_edges) != 3) {
    logi_error("Failed parsing number of attributes in file");
    return TERRA_STATUS_FAILURE;
  }
  if (num_edges != 0) {
    logi_error("Non-zero number of edges not supported");
    return TERRA_STATUS_FAILURE;
  }

  // Allocate associated vectors
  terra_vector_t out_verts;
  terra_vector_t out_idx;
  TERRA_CALL_I(terra_vector_with_capacity(app, num_verts, TERRA_DEFAULT_DATA_SIZE, &out_verts), "Failed allocating vertices");
  TERRA_CALL_I(terra_vector_with_capacity(app, num_faces, TERRA_DEFAULT_DATA_SIZE, &out_idx), "Failed allocating indices");

  // Read vertices
  terra_vertex3_t vert;
  for (size_t i = 0; i < num_verts; i++) {
    // TODO: Consider different formats in files
    TERRA_CALL_I(terrau_readline(app, f, line_buffer, &buffer), "Failed reading vertex line");
    if (sscanf(buffer, "%f %f %f", &vert.position[0], &vert.position[1], &vert.position[2]) != 3) {
      logi_error("Found failure at %zu-th vertex", i);
      return TERRA_STATUS_FAILURE;
    }

    // TODO: Implement automatically calculating normals
    TERRA_CALL_I(terra_vector_push(app, &out_verts, &vert), "Failed pushing vector");
  }

  // Read the faces
  uint32_t face[3];
  for (size_t i = 0; i < num_faces; i++) {
    TERRA_CALL_I(terrau_readline(app, f, line_buffer, &buffer), "Failed reading face line");
    if (sscanf(buffer, "%d %s", &vert.position[0], &vert.position[1], &vert.position[2]) != 3) {
      logi_error("Found failure at %zu-th vertex", i);
      return TERRA_STATUS_FAILURE;
    }

    // TODO: Implement automatically calculating normals
    TERRA_CALL_I(terra_vector_push(app, &out_verts, &vert), "Failed pushing vector");
  }

  // TODO: Finish implementing importing OFF files
  logi_error("Importing OFF files not implemented");
  return TERRA_STATUS_FAILURE;
}
