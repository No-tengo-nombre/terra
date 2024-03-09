#include "vendor/log.h"

#include <stdlib.h>
#include <terra/app.h>
#include <terrau/mem.h>

#ifndef NDEBUG

extern inline void *_terrau_malloc(
    terra_app_t *app, size_t size, const char *filename, int line
) {
  logi_debug(
      "Allocating %i B in heap with malloc @ %s:%d", size, filename, line
  );
  void *ret = malloc(size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
}

extern inline void *_terrau_realloc(
    terra_app_t *app, void *ptr, size_t new_size, const char *filename, int line
) {
  logi_debug(
      "Allocating %i B in heap with 'realloc' @ %s:%d", new_size, filename, line
  );
  void *ret = realloc(ptr, new_size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
}

extern inline void *_terrau_calloc(
    terra_app_t *app, size_t num, size_t size, const char *filename, int line
) {
  logi_debug(
      "Allocating %ix(%i B) in heap with 'calloc' @ %s:%d",
      num,
      size,
      filename,
      line
  );
  void *ret = calloc(num, size);
  if (ret != NULL) {
    app->_idebug_malloced_total++;
  }
  return ret;
}

extern inline void _terrau_free(
    terra_app_t *app, void *ptr, const char *filename, int line
) {
  logi_debug(
      "Freeing memory in heap at address %#10x @ %s:%d", ptr, filename, line
  );
  if (ptr != NULL) {
    app->_idebug_malloced_total--;
  }
  free(ptr);
}

#endif
