#pragma once

#include <cglm/cglm.h>
#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

#define TERRA_VERTEX2_BIND_DESC_SIZE 1
#define TERRA_VERTEX2_ATTR_DESC_SIZE 2

#ifdef __cplusplus
extern "C" {
#endif

extern const VkVertexInputBindingDescription
    TERRA_VERTEX2_BIND_DESC[TERRA_VERTEX2_BIND_DESC_SIZE];

extern const VkVertexInputAttributeDescription
    TERRA_VERTEX2_ATTR_DESC[TERRA_VERTEX2_ATTR_DESC_SIZE];

typedef struct terra_vertex2 {
  vec2 position;
  vec4 color;
} terra_vertex2_t;

#ifdef __cplusplus
}
#endif
