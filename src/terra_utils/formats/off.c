#include <stdio.h>
#include <terra/status.h>
#include <terra_utils/vendor/log.h>
#include <terrau/formats/descriptors.h>
#include <terrau/formats/off.h>

terra_status_t terrau_load_off(
    const char *filename, terrau_mesh_descriptor_t *desc
) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    logi_error("Could not open file '%s'", filename);
    return TERRA_STATUS_FAILURE;
  }

  // TODO: Implement importing OFF files
  logi_error("Importing OFF files not implemented");
  return TERRA_STATUS_FAILURE;
}
