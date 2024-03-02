#include "_pipeline.h"

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
      .scissor_offset    = {0, 0},
  };
  return result;
}

terra_status_t terra_vk_pipeline_new(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    terra_vk_shader_t *vert,
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
  TERRA_CALL_I(
      _terra_vk_pipeline_vert(app, &p, vert, shader_stages),
      "Failed creating vertex shader"
  );
  TERRA_CALL_I(
      _terra_vk_pipeline_frag(app, &p, frag, shader_stages + 1),
      "Failed creating fragment shader"
  );

  // Determine dynamic state
  VkPipelineDynamicStateCreateInfo dyn_info = {VK_FALSE};
  TERRA_CALL_I(
      _terra_vk_pipeline_dyn_state_info(app, &p, &dyn_info),
      "Failed creating dynamic state info"
  );

  // Vertex input state
  VkPipelineVertexInputStateCreateInfo vert_state_info = {VK_FALSE};
  VkPipelineInputAssemblyStateCreateInfo input_info    = {VK_FALSE};
  TERRA_CALL_I(
      _terra_vk_pipeline_vertex_input_state_info(app, &p, &vert_state_info),
      "Failed creating vertex input state info"
  );
  TERRA_CALL_I(
      _terra_vk_pipeline_input_asm_state_info(app, &p, &input_info),
      "Failed creating input assembly state info"
  );

  int dyn_viewport_found          = 0;
  int dyn_scissor_found           = 0;
  const VkDynamicState *dyn_state = p.dyn_state;
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
  VkViewport viewport = {VK_FALSE};
  VkRect2D scissor    = {VK_FALSE};
  if (!dyn_viewport_found) {
    TERRA_CALL_I(
        _terra_vk_pipeline_new_viewport(app, &p, &viewport),
        "Failed creating viewport"
    )
    viewport_info.pViewports = &viewport;
  }
  if (!dyn_scissor_found) {
    TERRA_CALL_I(
        _terra_vk_pipeline_new_scissor(app, &p, &scissor),
        "Failed creating viewport"
    )
    viewport_info.pScissors = &scissor;
  }

  VkPipelineRasterizationStateCreateInfo rast_info = {VK_FALSE};
  VkPipelineMultisampleStateCreateInfo ms_info     = {VK_FALSE};
  TERRA_CALL_I(
      _terra_vk_pipeline_rasterizer_info(app, &p, &rast_info),
      "Failed creating rasterizer info"
  );
  TERRA_CALL_I(
      _terra_vk_pipeline_multisample_info(app, &p, &ms_info),
      "Failed creating multisample info"
  );

  // Color blending
  VkPipelineColorBlendAttachmentState cb_attachment = {VK_FALSE};
  VkPipelineColorBlendStateCreateInfo cb_info       = {VK_FALSE};
  TERRA_CALL_I(
      _terra_vk_pipeline_colorblend_attachment(app, &p, &cb_attachment),
      "Failed creating colorblend attachment"
  );
  TERRA_CALL_I(
      _terra_vk_pipeline_colorblend_info(app, &p, &cb_attachment, 1, &cb_info),
      "Failed creating colorblend create info"
  );
  TERRA_CALL_I(
      _terra_vk_pipeline_layout(app, &p, &app->vk_layout),
      "Failed creating pipeline layout"
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
  logi_debug("Creating pipeline from files");

  // Vertex shader
  terra_vk_shader_t vert_shader;
  terra_vk_shader_t *vert_p = NULL;
  if (vertex != NULL) {
    logi_debug("Reading vertex shader in '%s'", vertex);
    TERRA_CALL_I(
        terra_vk_shader_from_spv(app, vertex, &vert_shader),
        "Failed creating vertex shader"
    );
    vert_p = &vert_shader;
  }

  // Fragment shader
  terra_vk_shader_t frag_shader;
  terra_vk_shader_t *frag_p = NULL;
  if (frag != NULL) {
    logi_debug("Reading fragment shader in '%s'", frag);
    TERRA_CALL_I(
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
  // vkDestroyPipelineLayout(app->vk_ldevice, pipe->layout, NULL);
  return TERRA_STATUS_SUCCESS;
}
