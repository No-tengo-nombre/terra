#pragma once

#include "descriptors.h"

#include <terra/status.h>

terra_status_t terrau_load_obj(
    const char *filename, terrau_mesh_descriptor_t *out
);
