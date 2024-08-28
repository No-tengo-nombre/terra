#include <cglm/cglm.h>
#include <string.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>

const terra_ubo_descriptor_t TERRA_UBO_DESCRIPTOR_DEFAULT = {
    .count = 1,
    .stage = VK_SHADER_STAGE_VERTEX_BIT,
};

terra_ubo_descriptor_t terra_ubo_descriptor_default(void) {
  return TERRA_UBO_DESCRIPTOR_DEFAULT;
}

terra_status_t terra_ubo_new(
    terra_app_t *app, uint64_t size, terra_buffer_t *out
) {
  logi_debug("Creating uniform buffer");
  TERRA_CALL_I(
      terra_buffer_new(
          app,
          size,
          VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
          VK_SHARING_MODE_EXCLUSIVE,
          VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT |
              VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
          out
      ),
      "Failed creating uniform buffer"
  );

  logi_debug("Mapping data to uniform buffer");
  TERRA_CALL_I(terra_buffer_map(app, out), "Failed mapping memory");

  return TERRA_STATUS_SUCCESS;
}
