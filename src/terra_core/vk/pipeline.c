#include <terra/status.h>
#include <terra/vk/pipeline.h>
#include <terra/vulkan.h>

terra_vk_pipeline_params_t terra_vk_pipeline_params_default(void) {
  terra_vk_pipeline_params_t result = {
      .vert_entrypoint = "main",
      .frag_entrypoint = "main",
      .dyn_state = NULL,
      .dyn_state_count = 0,
  };
  return result;
}

terra_status_t terra_vk_pipeline_new(terra_app_t *app,
                                     terra_vk_pipeline_params_t *params,
                                     terra_vk_shader_t *vertex,
                                     terra_vk_shader_t *frag,
                                     terra_vk_pipeline_t *out) {
  terra_vk_pipeline_params_t p;
  if (params == NULL) {
    p = terra_vk_pipeline_params_default();
  } else {
    p = *params;
  }

  // Create the shader stage creation info
  VkPipelineShaderStageCreateInfo shader_stages[2] = {0};
  if (vertex != NULL) {
    VkPipelineShaderStageCreateInfo vert_info = {VK_FALSE};
    vert_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_info.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vert_info.module = vertex->mod;
    vert_info.pName = p.vert_entrypoint;
    vert_info.pSpecializationInfo = NULL; // TODO: Implement specialization info
    shader_stages[0] = vert_info;
  }
  if (frag != NULL) {
    VkPipelineShaderStageCreateInfo frag_info = {VK_FALSE};
    frag_info.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_info.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_info.module = frag->mod;
    frag_info.pName = p.frag_entrypoint;
    frag_info.pSpecializationInfo = NULL; // TODO: Implement specialization info
    shader_stages[1] = frag_info;
  }

  // Determine dynamic state
  if (p.dyn_state != NULL) {
    VkPipelineDynamicStateCreateInfo dyn_info = {VK_FALSE};
    dyn_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dyn_info.dynamicStateCount = p.dyn_state_count;
    dyn_info.pDynamicStates = p.dyn_state;
  }

  // Vertex input state
  // TODO: Add the option for the user to specify the binding and
  // attribute descriptions
  VkPipelineVertexInputStateCreateInfo vert_state_info = {VK_FALSE};
  vert_state_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vert_state_info.vertexBindingDescriptionCount = 0;
  vert_state_info.pVertexBindingDescriptions = NULL;
  vert_state_info.vertexAttributeDescriptionCount = 0;
  vert_state_info.pVertexAttributeDescriptions = NULL;

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_pipeline_from_filenames(
    terra_app_t *app, terra_vk_pipeline_params_t *params, const char *vertex,
    const char *frag, terra_vk_pipeline_t *out) {
  // Vertex shader
  terra_vk_shader_t vert_shader;
  terra_vk_shader_t *vert_p = NULL;
  if (vertex != NULL) {
    TERRA_CALL(terra_vk_shader_from_spv(app, vertex, &vert_shader),
               "Failed creating vertex shader");
    vert_p = &vert_shader;
  }

  // Fragment shader
  terra_vk_shader_t frag_shader;
  terra_vk_shader_t *frag_p = NULL;
  if (frag != NULL) {
    TERRA_CALL(terra_vk_shader_from_spv(app, frag, &frag_shader),
               "Failed creating fragment shader");
    frag_p = &frag_shader;
  }

  return terra_vk_pipeline_new(app, params, vert_p, frag_p, out);
}

terra_status_t terra_vk_pipeline_cleanup(terra_app_t *app,
                                         terra_vk_pipeline_t *pipe) {
  return TERRA_STATUS_SUCCESS;
}
