#pragma once

#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

terra_status_t terra_vb_new(
    terra_app_t *app, uint64_t size, terra_buffer_t *out
);

terra_status_t terra_vb_bind(
    terra_app_t *app, VkCommandBuffer cmd_buffer, terra_buffer_t *buf
);
