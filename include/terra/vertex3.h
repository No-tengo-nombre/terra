#pragma once

#include "app.h"
#include "status.h"
#include "vulkan.h"

#include <cglm/cglm.h>
#include <stdint.h>

#define TERRA_VERTEX3_BIND_DESC_SIZE 1
#define TERRA_VERTEX3_ATTR_DESC_SIZE 2

extern const VkVertexInputBindingDescription
    TERRA_VERTEX3_BIND_DESC[TERRA_VERTEX3_BIND_DESC_SIZE];

extern const VkVertexInputAttributeDescription
    TERRA_VERTEX3_ATTR_DESC[TERRA_VERTEX3_ATTR_DESC_SIZE];

typedef struct terra_vertex3 {
  vec3 position;
  vec4 color;
} terra_vertex3_t;

terra_status_t terra_vertex3_get_binding_description(
    terra_app_t *app, VkVertexInputBindingDescription *out
);
terra_status_t terra_vertex3_get_attribute_descriptions(
    terra_app_t *app, VkVertexInputAttributeDescription *out
);
