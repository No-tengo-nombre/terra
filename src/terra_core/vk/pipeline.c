#include <terra/status.h>
#include <terra/vk/pipeline.h>
#include <terra/vulkan.h>
#include <terra_utils/macros.h>

const VkDynamicState _DYN_STATE_DEFAULT[] = {
    VK_DYNAMIC_STATE_VIEWPORT,
    VK_DYNAMIC_STATE_SCISSOR,
};
const uint32_t _DYN_STATE_DEFAULT_COUNT =
    sizeof(_DYN_STATE_DEFAULT) / sizeof(VkDynamicState);

terra_vk_pipeline_params_t terra_vk_pipeline_params_default(void) {
  terra_vk_pipeline_params_t result = {
      .vert_entrypoint   = "main",
      .frag_entrypoint   = "main",
      .dyn_state         = _DYN_STATE_DEFAULT,
      .dyn_state_count   = _DYN_STATE_DEFAULT_COUNT,
      .topology          = VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST,
      .primitive_restart = VK_TRUE,
      .depth_clamp       = VK_FALSE,
      .polygon_mode      = VK_POLYGON_MODE_FILL,
      .cull_mode         = VK_CULL_MODE_BACK_BIT,
      .depth_bias        = VK_FALSE,
      .depth_bias_factor = 0.0f,
      .depth_bias_clamp  = 0.0f,
      .depth_bias_slope  = 0.0f,
  };
  return result;
}

terra_status_t terra_vk_pipeline_new(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    terra_vk_shader_t *vertex,
    terra_vk_shader_t *frag,
    terra_vk_pipeline_t *out
) {
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
    vert_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vert_info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
    vert_info.module = vertex->mod;
    vert_info.pName  = p.vert_entrypoint;
    vert_info.pSpecializationInfo = NULL; // TODO: Implement specialization info
    shader_stages[0]              = vert_info;
  }
  if (frag != NULL) {
    VkPipelineShaderStageCreateInfo frag_info = {VK_FALSE};
    frag_info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    frag_info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    frag_info.module = frag->mod;
    frag_info.pName  = p.frag_entrypoint;
    frag_info.pSpecializationInfo = NULL; // TODO: Implement specialization info
    shader_stages[1]              = frag_info;
  }

  // Determine dynamic state
  VkPipelineDynamicStateCreateInfo dyn_info = {VK_FALSE};
  if (p.dyn_state != NULL) {
    dyn_info.sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    dyn_info.dynamicStateCount = p.dyn_state_count;
    dyn_info.pDynamicStates    = p.dyn_state;
  }

  // Vertex input state
  // TODO: Add the option for the user to specify the binding and
  // attribute descriptions
  VkPipelineVertexInputStateCreateInfo vert_state_info = {VK_FALSE};
  vert_state_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  vert_state_info.vertexBindingDescriptionCount   = 0;
  vert_state_info.pVertexBindingDescriptions      = NULL;
  vert_state_info.vertexAttributeDescriptionCount = 0;
  vert_state_info.pVertexAttributeDescriptions    = NULL;

  // Input assembly
  VkPipelineInputAssemblyStateCreateInfo input_info = {VK_FALSE};
  input_info.sType =
      VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  input_info.topology               = p.topology;
  input_info.primitiveRestartEnable = p.primitive_restart;

  // Viewport
  VkViewport viewport = {VK_FALSE};
  viewport.x          = 0.0f;
  viewport.y          = 0.0f;
  viewport.width      = (float)app->vk_extent.width;
  viewport.height     = (float)app->vk_extent.height;
  viewport.minDepth   = 0.0f;
  viewport.maxDepth   = 1.0f;

  VkRect2D scissor  = {VK_FALSE};
  VkOffset2D offset = {0, 0};
  scissor.offset    = offset;
  scissor.extent    = app->vk_extent;

  int dyn_viewport_found    = 0;
  int dyn_scissor_found     = 0;
  VkDynamicState *dyn_state = p.dyn_state;
  for (int i = 0; i < p.dyn_state_count; i++, dyn_state++) {
    switch (*dyn_state) {
    case VK_DYNAMIC_STATE_VIEWPORT:
      dyn_viewport_found = 1;
      break;

    case VK_DYNAMIC_STATE_SCISSOR:
      dyn_scissor_found = 1;
      break;

    default:
      break;
    }

    if (dyn_viewport_found && dyn_scissor_found) {
      break;
    }
  }
  VkPipelineViewportStateCreateInfo viewport_info = {VK_FALSE};
  viewport_info.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
  viewport_info.viewportCount = 1;
  viewport_info.scissorCount  = 1;

  // Add them if they are not dynamic state
  if (!dyn_viewport_found) {
    viewport_info.pViewports = &viewport;
  }
  if (!dyn_scissor_found) {
    viewport_info.pScissors = &scissor;
  }

  // Rasterizer
  VkPipelineRasterizationStateCreateInfo rast_info = {VK_FALSE};
  rast_info.sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  rast_info.depthClampEnable        = p.depth_clamp;
  rast_info.rasterizerDiscardEnable = VK_FALSE;
  rast_info.polygonMode             = p.polygon_mode;
  rast_info.lineWidth               = 1.0f;
  rast_info.cullMode                = p.cull_mode;
  rast_info.frontFace               = VK_FRONT_FACE_CLOCKWISE;
  rast_info.depthBiasEnable         = p.depth_bias;
  rast_info.depthBiasConstantFactor = p.depth_bias_factor;
  rast_info.depthBiasClamp          = p.depth_bias_clamp;
  rast_info.depthBiasSlopeFactor    = p.depth_bias_slope;

  // Multisampling
  // TODO: Implement multisampling configuration
  VkPipelineMultisampleStateCreateInfo ms_info = {VK_FALSE};
  ms_info.sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  ms_info.sampleShadingEnable   = VK_FALSE;
  ms_info.rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
  ms_info.minSampleShading      = 1.0f;
  ms_info.pSampleMask           = NULL;
  ms_info.alphaToCoverageEnable = VK_FALSE;
  ms_info.alphaToOneEnable      = VK_FALSE;

  // Color blending
  // TODO: Implement configuration of color blending
  // TODO: Implement global configuration of color blending
  VkPipelineColorBlendAttachmentState cb_attachment = {VK_FALSE};
  cb_attachment.colorWriteMask =
      VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
      VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  cb_attachment.blendEnable         = VK_TRUE; // Performs alpha blending
  cb_attachment.srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  cb_attachment.dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  cb_attachment.colorBlendOp        = VK_BLEND_OP_ADD;
  cb_attachment.srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  cb_attachment.dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  cb_attachment.alphaBlendOp        = VK_BLEND_OP_ADD;

  VkPipelineColorBlendStateCreateInfo cb_info = {VK_FALSE};
  cb_info.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  cb_info.logicOpEnable     = VK_FALSE;
  cb_info.logicOp           = VK_LOGIC_OP_COPY;
  cb_info.attachmentCount   = 1;
  cb_info.pAttachments      = &cb_attachment;
  cb_info.blendConstants[0] = 0.0f;
  cb_info.blendConstants[1] = 0.0f;
  cb_info.blendConstants[2] = 0.0f;
  cb_info.blendConstants[3] = 0.0f;

  VkPipelineLayoutCreateInfo layout_info = {VK_FALSE};
  layout_info.sType          = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  layout_info.setLayoutCount = 0;
  layout_info.pSetLayouts    = NULL;
  layout_info.pushConstantRangeCount = 0;
  layout_info.pPushConstantRanges    = NULL;

  TERRA_VK_CALL_I(
      vkCreatePipelineLayout(app->vk_ldevice, &layout_info, NULL, out->layout),
      "Failed to create pipeline layout"
  );

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_pipeline_from_filenames(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    const char *vertex,
    const char *frag,
    terra_vk_pipeline_t *out
) {
  // Vertex shader
  terra_vk_shader_t vert_shader;
  terra_vk_shader_t *vert_p = NULL;
  if (vertex != NULL) {
    TERRA_CALL(
        terra_vk_shader_from_spv(app, vertex, &vert_shader),
        "Failed creating vertex shader"
    );
    vert_p = &vert_shader;
  }

  // Fragment shader
  terra_vk_shader_t frag_shader;
  terra_vk_shader_t *frag_p = NULL;
  if (frag != NULL) {
    TERRA_CALL(
        terra_vk_shader_from_spv(app, frag, &frag_shader),
        "Failed creating fragment shader"
    );
    frag_p = &frag_shader;
  }

  return terra_vk_pipeline_new(app, params, vert_p, frag_p, out);
}

terra_status_t terra_vk_pipeline_cleanup(
    terra_app_t *app, terra_vk_pipeline_t *pipe
) {
  vkDestroyPipelineLayout(app->vk_ldevice, pipe->layout, NULL);
  return TERRA_STATUS_SUCCESS;
}
