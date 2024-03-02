#pragma once

#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_vk_shader {
  VkShaderModule mod;
} terra_vk_shader_t;

terra_status_t terra_vk_shader_from_spv(
    terra_app_t *app, const char *filename, terra_vk_shader_t *out
);

terra_status_t terra_vk_shader_cleanup(
    terra_app_t *app, terra_vk_shader_t *shader
);

#ifdef __cplusplus
}
#endif
