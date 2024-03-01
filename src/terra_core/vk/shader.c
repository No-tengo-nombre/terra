#include <stdlib.h>
#include <terra/app.h>
#include <terra/vk/shader.h>
#include <terra/vulkan.h>
#include <terra_utils/macros.h>
#include <terra_utils/vendor/log.h>
#include <terrau/binfile.h>

terra_status_t terra_vk_shader_from_spv(
    const terra_app_t *app, const char *filename, terra_vk_shader_t *out
) {
  char *contents;
  int64_t size;

  logi_debug("Reading binary file");
  TERRA_CALL_I(
      terrau_read_binary_file(filename, &size, &contents),
      "Failed reading shader '%s' from SPIR-V formatted binary file",
      filename
  );
#ifndef NDEBUG
  logi_debug("Visualizing binary file");
  terrau_visualize_binary_file(contents, size);
#endif

  VkShaderModuleCreateInfo info = {VK_FALSE};
  info.sType                    = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
  info.codeSize                 = size;
  info.pCode                    = (uint32_t *)contents;

  logi_debug("Creating shader module");
  VkShaderModule mod;
  TERRA_VK_CALL_I(
      vkCreateShaderModule(app->vk_ldevice, &info, NULL, &mod),
      "Failed creating shader module"
  );
  free(contents);

  logi_debug("Writing to out parameters");
  out->mod = mod;

  return TERRA_STATUS_SUCCESS;
}

terra_status_t terra_vk_shader_cleanup(
    const terra_app_t *app, terra_vk_shader_t *shader
) {
  vkDestroyShaderModule(app->vk_ldevice, shader->mod, NULL);
  return TERRA_STATUS_SUCCESS;
}
