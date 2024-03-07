#include <stddef.h>
#include <string.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vertex3.h>
#include <terra/vulkan.h>

// TODO: Add option for the user to specify a custom descriptor

const VkVertexInputBindingDescription
    TERRA_VERTEX3_BIND_DESC[TERRA_VERTEX3_BIND_DESC_SIZE] = {
        {0, sizeof(terra_vertex3_t), VK_VERTEX_INPUT_RATE_VERTEX}
};

const VkVertexInputAttributeDescription
    TERRA_VERTEX3_ATTR_DESC[TERRA_VERTEX3_ATTR_DESC_SIZE] = {
  // (location, binding, format, offset)
        {0, 0,    VK_FORMAT_R32G32B32_SFLOAT, offsetof(terra_vertex3_t, position)},
        {1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(terra_vertex3_t,    color)}
};

terra_status_t terra_vertex3_get_binding_description(
    terra_app_t *app, VkVertexInputBindingDescription *out
) {
  VkVertexInputBindingDescription desc = {VK_FALSE};

  // TODO: Add option to use instanced rendering
  desc.binding   = 0;
  desc.stride    = sizeof(terra_vertex3_t);
  desc.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;

  *out = desc;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vertex3_get_attribute_descriptions(
    terra_app_t *app, VkVertexInputAttributeDescription *out
) {
  // This assumes the out array is already allocated but not necessarily
  // initialized
  VkVertexInputAttributeDescription descs[TERRA_VERTEX3_ATTR_DESC_SIZE] = {
      VK_FALSE
  };

  descs[0].binding  = 0;
  descs[0].location = 0;
  descs[0].format   = VK_FORMAT_R32G32B32_SFLOAT;
  descs[0].offset   = offsetof(terra_vertex3_t, position);

  descs[1].binding  = 0;
  descs[1].location = 1;
  descs[1].format   = VK_FORMAT_R32G32B32A32_SFLOAT;
  descs[1].offset   = offsetof(terra_vertex3_t, color);

  memcpy(
      out,
      descs,
      TERRA_VERTEX3_ATTR_DESC_SIZE * sizeof(VkVertexInputAttributeDescription)
  );
  return TERRA_STATUS_SUCCESS;
}
