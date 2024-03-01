#pragma once

#include "material.h"
#include "status.h"

#include <terra/vk/shader.h>

typedef struct terra_mesh {
  terra_material_t *material;
} terra_mesh_t;
