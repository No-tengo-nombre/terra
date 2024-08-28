#include <stdint.h>
#include <terra/terra.h>
#include <terra_utils/macros.h>

// TODO: Generalize descriptor set layouts

terra_status_t terra_vk_create_descriptor_set_layout(terra_app_t *app, uint32_t binding, terra_ubo_descriptor_t *desc, VkDescriptorSetLayoutBinding *out) {
  VkDescriptorSetLayoutBinding ubo_binding = {0};
  ubo_binding.binding = binding;
  ubo_binding.stageFlags = desc->stage;
  ubo_binding.descriptorCount = desc->count;
  ubo_binding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
  ubo_binding.pImmutableSamplers = NULL;

  *out = ubo_binding;
  return TERRA_STATUS_SUCCESS;
}

