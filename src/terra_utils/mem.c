#include "vendor/log.h"

#include <stdlib.h>
#include <terra/app.h>
#include <terrau/mem.h>

#ifndef NDEBUG

inline void *terrau_malloc(terra_app_t *app, size_t size) {
  logi_debug("Allocating %i B in heap with malloc", size);
  void *ret = malloc(size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
}

inline void *terrau_realloc(terra_app_t *app, void *ptr, size_t new_size) {
  logi_debug("Allocating %i B in heap with 'realloc'", new_size);
  void *ret = realloc(ptr, new_size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
}

inline void *terrau_calloc(terra_app_t *app, size_t num, size_t size) {
  logi_debug("Allocating %ix(%i B) in heap with 'calloc'", num, size);
  void *ret = calloc(num, size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
}

inline void terrau_free(terra_app_t *app, void *ptr) {
  logi_debug("Freeing memory in heap at address %#10x", ptr);
  if (ptr != NULL) {
    app->_idebug_malloced_total--;
  }
  free(ptr);
}

#endif
