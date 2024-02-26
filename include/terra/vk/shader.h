#pragma once

#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

typedef struct terra_vk_shader {
  VkShaderModule mod;
} terra_vk_shader_t;

terra_status_t terra_vk_shader_from_spv(const terra_app_t *app,
                                        const char *filename,
                                        terra_vk_shader_t *out);

terra_status_t terra_vk_shader_cleanup(const terra_app_t *app,
                                       terra_vk_shader_t *shader);
