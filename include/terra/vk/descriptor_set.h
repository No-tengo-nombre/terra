#pragma once

#include <stdint.h>
#include <terra/app.h>
#include <terra/buffers/ubo.h>
#include <terra/status.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_vk_descriptor_set {
  VkDescriptorSetLayoutBinding binding;
} terra_vk_descriptor_set_t;

terra_status_t terra_vk_create_descriptor_set_layout(
    terra_app_t *app,
    uint32_t binding,
    terra_ubo_descriptor_t *desc,
    VkDescriptorSetLayoutBinding *out
);

#ifdef __cplusplus
}
#endif
