#include <cglm/cglm.h>
#include <string.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>

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
  TERRA_VK_CALL_I(
      vmaMapMemory(app->vma_alloc, out->alloc, &out->data),
      "Failed mapping memory"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_ubo_bind(
    terra_app_t *app, VkCommandBuffer cmd_buffer, terra_buffer_t *buf
) {
  return TERRA_STATUS_SUCCESS;
}
