#include <stddef.h>
#include <string.h>
#include <terra/terra.h>

// TODO: Add option for the user to specify a custom descriptor

const VkVertexInputBindingDescription
    TERRA_VERTEX2_BIND_DESC[TERRA_VERTEX2_BIND_DESC_SIZE] = {
        {0, sizeof(terra_vertex2_t), VK_VERTEX_INPUT_RATE_VERTEX}
};

const VkVertexInputAttributeDescription
    TERRA_VERTEX2_ATTR_DESC[TERRA_VERTEX2_ATTR_DESC_SIZE] = {
        // (location, binding, format, offset)
        {0, 0,       VK_FORMAT_R32G32_SFLOAT, offsetof(terra_vertex2_t, position)},
        {1, 0, VK_FORMAT_R32G32B32A32_SFLOAT, offsetof(terra_vertex2_t,    color)}
};
