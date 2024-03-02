#include <terra/vk/framebuffer.h>
#include <terra/vulkan.h>
#include <terra_utils/macros.h>

terra_status_t terra_vk_framebuffer_new(terra_app_t *app) {
  logi_debug("Creating framebuffers");
  // IMPORTANT: This function assumes the array for framebuffers in app is
  // already properly allocated

  if (app->vk_framebuffers == NULL) {
    logi_error("Why is this NULL???? :)");
    return TERRA_STATUS_FAILURE;
  }

  for (int i = 0; i < app->vk_images_count; i++) {
    VkFramebufferCreateInfo fb_info = {VK_FALSE};
    fb_info.sType                   = VK_STRUCTURE_TYPE_FRAMEBUFFER_CREATE_INFO;
    fb_info.renderPass              = app->vk_render_pass;
    fb_info.attachmentCount         = 1;
    fb_info.pAttachments            = app->vk_image_views + i;
    fb_info.width                   = app->vk_extent.width;
    fb_info.height                  = app->vk_extent.height;
    fb_info.layers                  = 1;

    TERRA_VK_CALL_I(
        vkCreateFramebuffer(
            app->vk_ldevice, &fb_info, NULL, app->vk_framebuffers + i
        ),
        "Failed creating %i-th framebuffer",
        i
    );
  }

  return TERRA_STATUS_SUCCESS;
}
