#pragma once

#include <terra/vk/shader.h>

#include "material.h"
#include "status.h"

typedef struct terra_mesh {
  terra_material_t *material;
} terra_mesh_t;
