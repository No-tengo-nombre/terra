#pragma once

#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

terra_status_t terra_buffer_new(
    terra_app_t *app,
    uint64_t size,
    VkBufferUsageFlags usage,
    VkSharingMode mode,
    VkBuffer *out
);
