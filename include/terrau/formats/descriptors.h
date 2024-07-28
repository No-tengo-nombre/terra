#pragma once

#include <terra/data/vector.h>

typedef struct terrau_mesh_descriptor {
  terra_vector_t *verts;
  terra_vector_t *idx;
} terrau_mesh_descriptor_t;
