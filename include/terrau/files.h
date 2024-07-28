#pragma once

#include <stdio.h>
#include <terra/status.h>

terra_status_t terrau_readline(terra_app_t *app, FILE *file, char **out);
