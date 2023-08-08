#pragma once

#include <stdlib.h>

#include <terra/status.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

terrar_queue_t find_queue_families(VkPhysicalDevice, VkSurfaceKHR);

status_t init_window(terrar_app_t *);
status_t init_instance(terrar_app_t *);
status_t create_render_surface(terrar_app_t *);
status_t choose_pdevice(terrar_app_t *);
status_t create_ldevice(terrar_app_t *);
status_t retrieve_device_queue(terrar_app_t *);
