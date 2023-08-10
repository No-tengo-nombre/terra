#pragma once

#include <terra/status.h>
#include "app.h"

status_t terrar_init_window(terrar_app_t *);
status_t terrar_init_instance(terrar_app_t *);
status_t terrar_create_render_surface(terrar_app_t *);
status_t terrar_choose_pdevice(terrar_app_t *);
status_t terrar_create_ldevice(terrar_app_t *);
status_t terrar_retrieve_device_queue(terrar_app_t *);
