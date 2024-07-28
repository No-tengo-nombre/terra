#include <stdio.h>

#include <terra/status.h>
#include <terrau/formats/descriptors.h>
#include <terrau/formats/obj.h>
#include <terra_utils/vendor/log.h>

terra_status_t terrau_load_obj(const char *filename, terrau_mesh_descriptor_t *desc) {
  FILE *f = fopen(filename, "r");
  if (f == NULL) {
    logi_error("Could not open file '%s'", filename);
    return TERRA_STATUS_FAILURE;
  }

  return TERRA_STATUS_SUCCESS;
}

