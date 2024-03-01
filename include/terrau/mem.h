#include <stdint.h>
#include <terra/app.h>

inline void *terrau_malloc(terra_app_t *app, size_t size);
inline void *terrau_realloc(terra_app_t *app, void *ptr, size_t new_size);
inline void *terrau_calloc(terra_app_t *app, size_t num, size_t size);
inline void terrau_free(terra_app_t *app, void *ptr);
