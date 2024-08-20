#pragma once

#include <stdint.h>
#include <terra/data/vector.h>
#include <terra/data/vertex3.h>

#define TERRA_DEFAULT_VERTEX3_DSIZE sizeof(terra_vertex3_t)
#define TERRA_DEFAULT_FACE3_DSIZE   sizeof(uint32_t)

#ifdef __cplusplus
extern "C" {
#endif

typedef struct terrau_mesh_descriptor {
  terra_vector_t verts;
  terra_vector_t idx;
} terrau_mesh_descriptor_t;

#ifdef __cplusplus
}
#endif
