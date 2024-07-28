#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terrau/formats/descriptors.h>
#include <terrau/formats/obj.h>

terra_status_t terra_mesh_from_obj(
    terra_app_t *app, const char *filename, terra_mesh_t *out
) {
  terrau_mesh_descriptor_t desc;
  TERRA_CALL_I(terrau_load_obj(filename, &desc), "Failed loading OBJ file");
  return terra_mesh_from_descriptor(app, &desc, out);
}
