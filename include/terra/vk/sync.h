#pragma once

#include <terra/app.h>
#include <terra/status.h>

#ifdef __cplusplus
extern "C" {
#endif

terra_status_t terra_vk_create_sync_objects(terra_app_t *app);

terra_status_t terra_vk_await_sync_objects(terra_app_t *app);
terra_status_t terra_vk_reset_sync_objects(terra_app_t *app);

terra_status_t terra_vk_detroy_sync_objects(terra_app_t *app);

#ifdef __cplusplus
}
#endif
