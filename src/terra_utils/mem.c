#include "vendor/log.h"

#include <stdlib.h>
#include <terrau/mem.h>

inline void *terrau_malloc(terra_app_t *app, size_t size) {
#ifndef NDEBUG
  logi_debug("Allocating %i B in heap with malloc", size);
  void *ret = malloc(size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
#else
  return malloc(size);
#endif
}

inline void *terrau_realloc(terra_app_t *app, void *ptr, size_t new_size) {
#ifndef NDEBUG
  logi_debug("Allocating %i B in heap with 'realloc'", new_size);
  void *ret = realloc(ptr, new_size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
#else
  return realloc(ptr, new_size);
#endif
}

inline void *terrau_calloc(terra_app_t *app, size_t num, size_t size) {
#ifndef NDEBUG
  logi_debug("Allocating %ix(%i B) in heap with 'calloc'", num, size);
  void *ret = calloc(num, size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
#else
  return calloc(num, size);
#endif
}

inline void terrau_free(terra_app_t *app, void *ptr) {
#ifndef NDEBUG
  logi_debug("Freeing memory in heap at address %#18x", ptr);
  if (ptr != NULL) {
    app->_idebug_malloced_total--;
  }
#endif
  return free(ptr);
}
