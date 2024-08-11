#pragma once

#include <terra/data/vector.h>
#include <terra/data/vertex3.h>

#define TERRA_DEFAULT_DATA_SIZE sizeof(terra_vertex3_t)

typedef struct terrau_mesh_descriptor {
  terra_vector_t *verts;
  terra_vector_t *idx;
} terrau_mesh_descriptor_t;
