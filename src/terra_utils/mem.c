#include "vendor/log.h"

#include <stdlib.h>
#include <terra/app.h>
#include <terra_utils/macros.h>
#include <terrau/mem.h>

#ifndef NDEBUG

// These functions only get defined and called if in debug mode

extern inline void *_terrau_malloc(
    terra_app_t *app, size_t size, const char *filename, int line
) {
  logi_debug(
      "Allocating %i B in heap with 'malloc' @ %s:%d", size, filename, line
  );
  void *ret = malloc(size);
  if (ret != NULL) {
    heapinfo_push(app, ret, size, filename, line);
    app->_idebug_malloced_total++;
    app->_idebug_malloced_size += size;
    logi_debug(
        "%d elements (%d B) in heap",
        app->_idebug_malloced_total,
        app->_idebug_malloced_size
    );
  }
  return ret;
}

extern inline void *_terrau_realloc(
    terra_app_t *app, void *ptr, size_t new_size, const char *filename, int line
) {
  logi_debug(
      "Allocating %i B in heap with 'realloc' @ %s:%d", new_size, filename, line
  );
  if (ptr == NULL) {
    logi_debug("'realloc' will allocate brand new memory");
  }
  size_t addr = (size_t)ptr;
  void *ret   = realloc(ptr, new_size);
  if (ret != NULL) {
    if (addr != 0) {
      app->_idebug_malloced_size -= heapinfo_popaddr(app, (void *)addr);
    } else {
      app->_idebug_malloced_total++;
    }
    heapinfo_push(app, ret, new_size, filename, line);
    app->_idebug_malloced_size += new_size;
    logi_debug(
        "%d elements (%d B) in heap",
        app->_idebug_malloced_total,
        app->_idebug_malloced_size
    );
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
    heapinfo_push(app, ret, num * size, filename, line);
    app->_idebug_malloced_size += num * size;
    app->_idebug_malloced_total++;
    logi_debug(
        "%d elements (%d B) in heap",
        app->_idebug_malloced_total,
        app->_idebug_malloced_size
    );
  }
  return ret;
}

extern inline void _terrau_free(
    terra_app_t *app, void *ptr, const char *filename, int line
) {
  if (ptr != NULL) {
    logi_debug(
        "Freeing memory in heap at address %#p @ %s:%d", ptr, filename, line
    );
    size_t size                 = heapinfo_popaddr(app, ptr);
    app->_idebug_malloced_size -= size;
    app->_idebug_malloced_total--;
    logi_debug(
        "%d elements (%d B) in heap",
        app->_idebug_malloced_total,
        app->_idebug_malloced_size
    );
  }
  free(ptr);
}

#endif
