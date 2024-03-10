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

void *_terrau_malloc(
    terra_app_t *app, size_t size, const char *filename, int line
);
void *_terrau_realloc(
    terra_app_t *app, void *ptr, size_t new_size, const char *filename, int line
);
void *_terrau_calloc(
    terra_app_t *app, size_t num, size_t size, const char *filename, int line
);
void _terrau_free(terra_app_t *app, void *ptr, const char *filename, int line);

  #define terrau_malloc(app, x) _terrau_malloc(app, x, __FILENAME__, __LINE__)
  #define terrau_realloc(app, x, y)                                            \
    _terrau_realloc(app, x, y, __FILENAME__, __LINE__)
  #define terrau_calloc(app, x, y)                                             \
    _terrau_calloc(app, x, y, __FILENAME__, __LINE__)
  #define terrau_free(app, x) _terrau_free(app, x, __FILENAME__, __LINE__)

#endif

#ifdef __cplusplus
}
#endif
