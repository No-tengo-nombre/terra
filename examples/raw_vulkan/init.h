#pragma once

#include <stdlib.h>

#include <terra/status.h>
#include <terrar/app.h>
#include <terrar/vulkan.h>

typedef struct queue_indices_t {
    uint32_t family;
    int found;
} queue_indices_t;

queue_indices_t find_queue_families(VkPhysicalDevice);

status_t init_window(terrar_app_t *);
status_t init_instance(terrar_app_t *);
status_t choose_pdevice(terrar_app_t *);
status_t create_ldevice(terrar_app_t *);
