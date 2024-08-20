#pragma once

#include "descriptors.h"

#include <terra/app.h>
#include <terra/status.h>

#ifdef __cplusplus
extern "C" {
#endif

terra_status_t terrau_load_obj(
    terra_app_t *app, const char *filename, terrau_mesh_descriptor_t *out
);

#ifdef __cplusplus
}
#endif
