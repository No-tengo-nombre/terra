#pragma once

#include <stdint.h>
#include <terra/status.h>

terra_status_t terrau_read_binary_file(
    const char *filename, int64_t *out_size, char **out
);
terra_status_t terrau_visualize_binary_file(
    const char *contents, const int64_t size
);
