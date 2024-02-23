#pragma once

#include "app.h"
#include <terra/status.h>

terra_status terrar_setup_init_window(terrar_app *app);
terra_status terrar_setup_init_instance(terrar_app *app);
terra_status terrar_setup_create_render_surface(terrar_app *app);
terra_status terrar_setup_choose_pdevice(terrar_app *app);
terra_status terrar_setup_create_ldevice(terrar_app *app);
terra_status terrar_setup_retrieve_device_queue(terrar_app *app);
