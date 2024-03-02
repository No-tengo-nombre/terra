#pragma once

#include <stdint.h>
#include <terra/app.h>
#include <terra/status.h>

#ifdef __cplusplus
extern "C" {
#endif

terra_status_t terrau_read_binary_file(
    terra_app_t *app, const char *filename, int64_t *out_size, char **out
);
terra_status_t terrau_visualize_binary_file(
    terra_app_t *app, const char *contents, const int64_t size
);

#ifdef __cplusplus
}
#endif
