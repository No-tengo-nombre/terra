#pragma once

#include <stdint.h>
#include <terra/app.h>

#ifdef __cplusplus
extern "C" {
#endif

#ifdef NDEBUG

  #define terrau_malloc(app, x)     malloc(x)
  #define terrau_realloc(app, x, y) realloc(x, y)
  #define terrau_calloc(app, x, y)  calloc(x, y)
  #define terrau_free(app, x)       free(x)

#else

void *terrau_malloc(terra_app_t *app, size_t size);
void *terrau_realloc(terra_app_t *app, void *ptr, size_t new_size);
void *terrau_calloc(terra_app_t *app, size_t num, size_t size);
void terrau_free(terra_app_t *app, void *ptr);

#endif

#ifdef __cplusplus
}
#endif
