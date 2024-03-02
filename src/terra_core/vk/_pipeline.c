#include "_pipeline.h"

#include <stdint.h>
#include <terra/status.h>
#include <terra/vk/shader.h>
#include <terra_utils/macros.h>

terra_status_t _terra_vk_pipeline_vert(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    terra_vk_shader_t *shader,
    VkPipelineShaderStageCreateInfo *out
) {
  if (shader != NULL) {
    VkPipelineShaderStageCreateInfo info = {VK_FALSE};
    info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.stage  = VK_SHADER_STAGE_VERTEX_BIT;
    info.module = shader->mod;
    info.pName  = params->vert_entrypoint;
    info.pSpecializationInfo =
        NULL; // TODO: Implement specialization for vertex shaders
    *out = info;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_frag(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    terra_vk_shader_t *shader,
    VkPipelineShaderStageCreateInfo *out
) {
  if (shader != NULL) {
    VkPipelineShaderStageCreateInfo info = {VK_FALSE};
    info.sType  = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    info.stage  = VK_SHADER_STAGE_FRAGMENT_BIT;
    info.module = shader->mod;
    info.pName  = params->frag_entrypoint;
    info.pSpecializationInfo =
        NULL; // TODO: Implement specialization for fragment shaders
    *out = info;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_dyn_state_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineDynamicStateCreateInfo *out
) {
  if (params->dyn_state != NULL) {
    out->sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO;
    out->dynamicStateCount = params->dyn_state_count;
    out->pDynamicStates    = params->dyn_state;
  }
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_vertex_input_state_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineVertexInputStateCreateInfo *out
) {
  // TODO: Add the option for the user to specify the binding and
  // attribute descriptions
  out->sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO;
  out->vertexBindingDescriptionCount   = 0;
  out->pVertexBindingDescriptions      = NULL;
  out->vertexAttributeDescriptionCount = 0;
  out->pVertexAttributeDescriptions    = NULL;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_input_asm_state_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineInputAssemblyStateCreateInfo *out
) {
  out->sType    = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO;
  out->topology = params->topology;
  out->primitiveRestartEnable = params->primitive_restart;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_new_viewport(
    terra_app_t *app, terra_vk_pipeline_params_t *params, VkViewport *out
) {
  out->x        = 0.0f;
  out->y        = 0.0f;
  out->width    = (float)app->vk_extent.width;
  out->height   = (float)app->vk_extent.height;
  out->minDepth = 0.0f;
  out->maxDepth = 1.0f;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_new_scissor(
    terra_app_t *app, terra_vk_pipeline_params_t *params, VkRect2D *out
) {
  out->offset = params->scissor_offset;
  out->extent = app->vk_extent;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_rasterizer_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineRasterizationStateCreateInfo *out
) {
  out->sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO;
  out->depthClampEnable        = params->depth_clamp;
  out->rasterizerDiscardEnable = VK_FALSE;
  out->polygonMode             = params->polygon_mode;
  out->lineWidth               = 1.0f;
  out->cullMode                = params->cull_mode;
  out->frontFace               = VK_FRONT_FACE_CLOCKWISE;
  out->depthBiasEnable         = params->depth_bias;
  out->depthBiasConstantFactor = params->depth_bias_factor;
  out->depthBiasClamp          = params->depth_bias_clamp;
  out->depthBiasSlopeFactor    = params->depth_bias_slope;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_multisample_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineMultisampleStateCreateInfo *out
) {
  // TODO: Implement multisampling configuration
  out->sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO;
  out->sampleShadingEnable   = VK_FALSE;
  out->rasterizationSamples  = VK_SAMPLE_COUNT_1_BIT;
  out->minSampleShading      = 1.0f;
  out->pSampleMask           = NULL;
  out->alphaToCoverageEnable = VK_FALSE;
  out->alphaToOneEnable      = VK_FALSE;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_colorblend_attachment(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineColorBlendAttachmentState *out
) {
  // TODO: Implement local configuration of color blending
  // TODO: Implement global configuration of color blending
  out->colorWriteMask = VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT |
                        VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT;
  out->blendEnable         = VK_TRUE; // Performs alpha blending
  out->srcColorBlendFactor = VK_BLEND_FACTOR_SRC_ALPHA;
  out->dstColorBlendFactor = VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA;
  out->colorBlendOp        = VK_BLEND_OP_ADD;
  out->srcAlphaBlendFactor = VK_BLEND_FACTOR_ONE;
  out->dstAlphaBlendFactor = VK_BLEND_FACTOR_ZERO;
  out->alphaBlendOp        = VK_BLEND_OP_ADD;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_colorblend_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineColorBlendAttachmentState *cb_attachments,
    uint32_t cb_attachments_count,
    VkPipelineColorBlendStateCreateInfo *out
) {
  out->sType         = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
  out->logicOpEnable = VK_FALSE;
  out->logicOp       = VK_LOGIC_OP_COPY;
  out->attachmentCount   = cb_attachments_count;
  out->pAttachments      = cb_attachments;
  out->blendConstants[0] = 0.0f;
  out->blendConstants[1] = 0.0f;
  out->blendConstants[2] = 0.0f;
  out->blendConstants[3] = 0.0f;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t _terra_vk_pipeline_layout(
    terra_app_t *app, terra_vk_pipeline_params_t *params, VkPipelineLayout *out
) {
  VkPipelineLayoutCreateInfo info = {VK_FALSE};
  info.sType                  = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
  info.setLayoutCount         = 0;
  info.pSetLayouts            = NULL;
  info.pushConstantRangeCount = 0;
  info.pPushConstantRanges    = NULL;

  TERRA_VK_CALL_I(
      vkCreatePipelineLayout(app->vk_ldevice, &info, NULL, out),
      "Failed to create pipeline layout"
  );
  return TERRA_STATUS_SUCCESS;
}
