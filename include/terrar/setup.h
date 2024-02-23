#pragma once

#include "app.h"
#include <terra/status.h>

terra_status terrar_init_window(terrar_app *);
terra_status terrar_init_instance(terrar_app *);
terra_status terrar_create_render_surface(terrar_app *);
terra_status terrar_choose_pdevice(terrar_app *);
terra_status terrar_create_ldevice(terrar_app *);
terra_status terrar_retrieve_device_queue(terrar_app *);
