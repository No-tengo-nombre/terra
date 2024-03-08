#pragma once

#include <cglm/cglm.h>
#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>

#define TERRA_VERTEX3_BIND_DESC_SIZE 1
#define TERRA_VERTEX3_ATTR_DESC_SIZE 2

#ifdef __cplusplus
extern "C" {
#endif

extern const VkVertexInputBindingDescription
    TERRA_VERTEX3_BIND_DESC[TERRA_VERTEX3_BIND_DESC_SIZE];

extern const VkVertexInputAttributeDescription
    TERRA_VERTEX3_ATTR_DESC[TERRA_VERTEX3_ATTR_DESC_SIZE];

typedef struct terra_vertex3 {
  vec3 position;
  vec4 color;
} terra_vertex3_t;

#ifdef __cplusplus
}
#endif
