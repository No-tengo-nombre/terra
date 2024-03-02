#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vk/pipeline.h>
#include <terra/vulkan.h>

terra_status_t _terra_vk_pipeline_vert(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    terra_vk_shader_t *shader,
    VkPipelineShaderStageCreateInfo *out
);
terra_status_t _terra_vk_pipeline_frag(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    terra_vk_shader_t *shader,
    VkPipelineShaderStageCreateInfo *out
);
terra_status_t _terra_vk_pipeline_dyn_state_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineDynamicStateCreateInfo *out
);
terra_status_t _terra_vk_pipeline_vertex_input_state_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineVertexInputStateCreateInfo *out
);
terra_status_t _terra_vk_pipeline_input_asm_state_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineInputAssemblyStateCreateInfo *out
);
terra_status_t _terra_vk_pipeline_new_viewport(
    terra_app_t *app, terra_vk_pipeline_params_t *params, VkViewport *out
);
terra_status_t _terra_vk_pipeline_new_scissor(
    terra_app_t *app, terra_vk_pipeline_params_t *params, VkRect2D *out
);
terra_status_t _terra_vk_pipeline_rasterizer_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineRasterizationStateCreateInfo *out
);
terra_status_t _terra_vk_pipeline_multisample_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineMultisampleStateCreateInfo *out
);
terra_status_t _terra_vk_pipeline_colorblend_attachment(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineColorBlendAttachmentState *out
);
terra_status_t _terra_vk_pipeline_colorblend_info(
    terra_app_t *app,
    terra_vk_pipeline_params_t *params,
    VkPipelineColorBlendAttachmentState *cb_attachments,
    uint32_t cb_attachments_count,
    VkPipelineColorBlendStateCreateInfo *out
);
terra_status_t _terra_vk_pipeline_layout(
    terra_app_t *app, terra_vk_pipeline_params_t *params, VkPipelineLayout *out
);
