#pragma once

#include "descriptors.h"

#include <terra/app.h>
#include <terra/status.h>

terra_status_t terrau_load_obj(
    terra_app_t *app, const char *filename, terrau_mesh_descriptor_t *out
);
