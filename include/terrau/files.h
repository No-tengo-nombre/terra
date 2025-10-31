#pragma once

#include <stdio.h>
#include <terra/app.h>
#include <terra/status.h>

#ifdef __cplusplus
extern "C" {
#endif

terra_status_t terrau_readline(
    terra_app_t *app, FILE *file, char *buffer
);

#ifdef __cplusplus
}
#endif
