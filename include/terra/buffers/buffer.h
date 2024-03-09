#pragma once

#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terra_buffer {
  VkBuffer buffer;
  VmaAllocation alloc;
  void *data; // Pointer to section in memory where it gets mapped
} terra_buffer_t;

terra_status_t terra_buffer_new(
    terra_app_t *app,
    uint64_t size,
    VkBufferUsageFlags usage,
    VkSharingMode mode,
    VkMemoryPropertyFlags req_flags,
    terra_buffer_t *out
);
terra_status_t terra_buffer_cleanup(terra_app_t *app, terra_buffer_t *buf);

terra_status_t terra_buffer_copy(
    terra_app_t *app, terra_buffer_t *src, terra_buffer_t *dst, size_t size
);
terra_status_t terra_buffer_map(terra_app_t *app, terra_buffer_t *buf);
terra_status_t terra_buffer_unmap(terra_app_t *app, terra_buffer_t *buf);

#ifdef __cplusplus
}
#endif
