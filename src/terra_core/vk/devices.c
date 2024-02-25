#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <terra/app.h>
#include <terra/status.h>
#include <terra/vulkan.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>
#include <terrau/math/clamp.h>

#include <terra/vk/devices.h>
#include <terra/vk/swapchain.h>

terra_status_t terra_vk_create_application_info(terra_app_t *app,
                                                VkApplicationInfo *out) {
  out->sType = VK_STRUCTURE_TYPE_APPLICATION_INFO;
  out->apiVersion = VK_API_VERSION_1_3;
  out->applicationVersion = VK_MAKE_API_VERSION(
      1, app->meta->vmajor, app->meta->vminor, app->meta->vpatch);
  out->engineVersion = VK_MAKE_API_VERSION(1, TERRA_ENGINE_VERSION_MAJOR,
                                           TERRA_ENGINE_VERSION_MINOR,
                                           TERRA_ENGINE_VERSION_PATCH);
  out->pApplicationName = app->meta->app_name;
  out->pEngineName = "Terra";
  out->pNext = NULL;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_create_instance_info(terra_app_t *app,
                                             VkApplicationInfo *app_info,
                                             VkInstanceCreateInfo *out) {
  uint32_t extension_count = 0;
  const char **extensions = glfwGetRequiredInstanceExtensions(&extension_count);
  out->sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO;
  out->pApplicationInfo = app_info;
  out->enabledExtensionCount = extension_count;
  out->ppEnabledExtensionNames = extensions;
  out->flags = 0;
  out->pNext = NULL;
#ifndef NDEBUG
  out->enabledLayerCount = app->conf->validation_layers_total;
  out->ppEnabledLayerNames = app->conf->validation_layers;
#else
  out->enabledLayerCount = 0;
#endif
  return TERRA_STATUS_SUCCESS;
}

int terra_vk_check_validation_layer_support(terra_app_t *app) {
  uint32_t layers;
  vkEnumerateInstanceLayerProperties(&layers, NULL);
  VkLayerProperties *properties = malloc(layers * sizeof(VkLayerProperties));
  if (properties == NULL) {
    return 0;
  }
  vkEnumerateInstanceLayerProperties(&layers, properties);

  for (int i = 0; i < app->conf->validation_layers_total; i++) {
    int found = 0;
    for (int j = 0; j < layers; j++) {
      if (strcmp(app->conf->validation_layers[i], properties[j].layerName) ==
          0) {
        found = 1;
        break;
      }
    }
    if (!found) {
      return 0;
    }
  }
  free(properties);
  return 1;
}

terra_status_t terra_vk_find_queue_families(VkPhysicalDevice device,
                                            VkSurfaceKHR surface,
                                            terra_queue_t *out) {
  out->gfound = 0;
  out->pfound = 0;
  VkPhysicalDeviceProperties dev_props;
  vkGetPhysicalDeviceProperties(device, &dev_props);

  uint32_t count = 0;
  vkGetPhysicalDeviceQueueFamilyProperties(device, &count, NULL);
  VkQueueFamilyProperties *qprops =
      malloc(count * sizeof(VkQueueFamilyProperties));
  if (qprops == NULL) {
    return TERRA_STATUS_FAILURE;
  }
  vkGetPhysicalDeviceQueueFamilyProperties(device, &count, qprops);

  VkBool32 pfound = 0;
  for (int i = 0; i < count; i++) {
    vkGetPhysicalDeviceSurfaceSupportKHR(device, i, surface, &pfound);
    unsigned char qflags_bin[5];
    for (int b = 0; b < 4; b++) {
      qflags_bin[b] = (qprops[i].queueFlags >> (3 - b)) & 1 ? '1' : '0';
    }
    qflags_bin[4] = '\0';
    logi_debug("Queues %s %ix%s", dev_props.deviceName, qprops[i].queueCount,
               qflags_bin);
    if (pfound && !out->pfound) {
      out->pfamily = i;
      out->pfound = 1;
      logi_debug("Setting %i as processing queue", i);
    }
    if (qprops[i].queueFlags & VK_QUEUE_GRAPHICS_BIT && !out->gfound) {
      out->gfamily = i;
      out->gfound = 1;
      logi_debug("Setting %i as graphics queue", i);
    }
  }
  free(qprops);
  return TERRA_STATUS_SUCCESS;
}

int terra_check_device_extensions(VkPhysicalDevice device, const char **target,
                                  size_t target_total) {
  uint32_t ext_total;
  vkEnumerateDeviceExtensionProperties(device, NULL, &ext_total, NULL);
  VkExtensionProperties *ext_props =
      malloc(ext_total * sizeof(VkExtensionProperties));
  vkEnumerateDeviceExtensionProperties(device, NULL, &ext_total, ext_props);

  int all_found = 1;
  for (int i = 0; i < target_total; i++) {
    int found = 0;
    for (int j = 0; j < ext_total; j++) {
      if (strcmp(target[i], ext_props[j].extensionName) == 0) {
        found = 1;
        break;
      }
    }
    if (!found) {
      logi_warn("Extension %s was not found", target[i]);
      all_found = 0;
      break;
    }
    logi_debug("Extension %s was found", target[i]);
  }

  free(ext_props);
  return all_found;
}

terra_status_t terra_vk_rate_device(terra_app_t *app, VkPhysicalDevice device,
                                    VkSurfaceKHR surface, terra_queue_t *queue,
                                    uint32_t *out) {
  VkPhysicalDeviceProperties props;
  VkPhysicalDeviceFeatures feats;
  vkGetPhysicalDeviceProperties(device, &props);
  vkGetPhysicalDeviceFeatures(device, &feats);

  int extensions_supported = terra_check_device_extensions(
      device, app->conf->device_extensions, app->conf->device_extensions_total);
  int adequate_sc = 0;
  if (extensions_supported) {
    logi_debug("Device '%s' supports all extensions", props.deviceName);
    terra_vk_sc_details_t sc_details;
    TERRA_CALL_I(terra_vk_check_sc_support(device, surface, &sc_details),
                 "Failed checking for swapchain support");
    TERRA_CALL_I(terra_vk_sc_details_cleanup(&sc_details),
                 "Failed cleaning up the swapchain details");
    adequate_sc =
        (sc_details.format_count != 0) && (sc_details.mode_count != 0);
    if (adequate_sc) {
      logi_debug("Device '%s' has adequate swapchain", props.deviceName);
    }
  }

  if (!queue->gfound || !queue->pfound || !extensions_supported ||
      !adequate_sc) {
    logi_debug("Device '%s' not suitable", props.deviceName);
    return TERRA_STATUS_FAILURE;
  }

  uint32_t score = 0;
  if (props.deviceType == VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU) {
    score += 1000;
  }
  score += props.limits.maxImageDimension2D;
  logi_debug("Device '%s' score: %I32u", props.deviceName, score);
  *out = score;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_get_physical_device(terra_app_t *app,
                                            terra_result_t *out) {
  VkPhysicalDevice device = VK_NULL_HANDLE;
  terra_queue_t device_queue;
  uint32_t device_count = 0;
  vkEnumeratePhysicalDevices(app->vk_instance, &device_count, NULL);
  if (device_count == 0) {
    logi_error("Failed to find GPUs that support Vulkan");
    return TERRA_STATUS_FAILURE;
  }
  VkPhysicalDevice *devices = malloc(device_count * sizeof(VkPhysicalDevice));
  if (devices == NULL) {
    logi_error("Could not allocate enough memory for the devices");
    return TERRA_STATUS_FAILURE;
  }
  vkEnumeratePhysicalDevices(app->vk_instance, &device_count, devices);

  int32_t score = -2;
  uint32_t new_score = 0;
  for (int i = 0; i < device_count; i++) {
    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(devices[i], &props);
    logi_info("Evaluating device '%s'", props.deviceName);
    TERRA_CALL_I(terra_vk_find_queue_families(devices[i], app->vk_surface,
                                              &device_queue),
                 "Failed creating device queue");
    TERRA_CALL_I(terra_vk_rate_device(app, devices[i], app->vk_surface,
                                      &device_queue, &new_score),
                 "Failed rating device");
    if (score == -2) {
      device = devices[i];
      score = new_score;
      continue;
    }
    if (new_score > score) {
      device = devices[i];
      score = new_score;
    }
  }
  if (score == -1) {
    logi_error("No devices are suitable for the application");
    return TERRA_STATUS_FAILURE;
  } else {
    out->value = device;
    out->queue = device_queue;

    VkPhysicalDeviceProperties props;
    vkGetPhysicalDeviceProperties(device, &props);
    logi_info("Using device %s", props.deviceName);
  }
  free(devices);
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_create_device_queue_info(uint32_t index, float *prio,
                                                 VkDeviceQueueCreateInfo *out) {
  out->sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
  out->queueCount = 1;
  out->queueFamilyIndex = index;
  out->pQueuePriorities = prio;
  out->pNext = NULL;
  out->flags = 0;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_create_device_features(VkPhysicalDeviceFeatures *out) {
  out->geometryShader = VK_TRUE;
  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_create_device_info(
    VkDeviceQueueCreateInfo *queue_info, uint32_t queue_count,
    VkPhysicalDeviceFeatures *device_features, const char **device_extensions,
    uint32_t device_extension_count, VkDeviceCreateInfo *out) {
  out->sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
  out->queueCreateInfoCount = queue_count;
  out->pQueueCreateInfos = queue_info;
  out->pEnabledFeatures = device_features;

  out->pNext = NULL;
  out->flags = 0;

  // The validation checks are done, so it can be assumed that the requested
  // features are available
  out->enabledLayerCount = 0;
  out->ppEnabledLayerNames = NULL;

  out->enabledExtensionCount = device_extension_count;
  out->ppEnabledExtensionNames = device_extensions;
  return TERRA_STATUS_SUCCESS;
}