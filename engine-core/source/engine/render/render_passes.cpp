#include "engine/render/render_passes.h"
#include "engine/engine.h"

namespace VKEngine {
  
  void RenderPasses::init(VkDevice device, SwapchainHandler* swapchain, uint32_t width, uint32_t height)
  {
    _device = device;
    _swapchainHandler = swapchain;
    
    init_default();
    init_framebuffers(width, height);
  }
  
  void RenderPasses::init_default()
  {
    VkAttachmentDescription color_attachment = {};
    color_attachment.format = _swapchainHandler->_swachainImageFormat;
    color_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    color_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    color_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    color_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_DONT_CARE;
    color_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    color_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    color_attachment.finalLayout = VK_IMAGE_LAYOUT_PRESENT_SRC_KHR;
    
    VkAttachmentReference color_attachment_ref = {};
    color_attachment_ref.attachment = 0;
    color_attachment_ref.layout = VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL;
    
    VkAttachmentDescription depth_attachment = {};
    depth_attachment.flags = 0;
    depth_attachment.format = _swapchainHandler->_depthFormat;
    depth_attachment.samples = VK_SAMPLE_COUNT_1_BIT;
    depth_attachment.loadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.storeOp = VK_ATTACHMENT_STORE_OP_STORE;
    depth_attachment.stencilLoadOp = VK_ATTACHMENT_LOAD_OP_CLEAR;
    depth_attachment.stencilStoreOp = VK_ATTACHMENT_STORE_OP_DONT_CARE;
    depth_attachment.initialLayout = VK_IMAGE_LAYOUT_UNDEFINED;
    depth_attachment.finalLayout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    VkAttachmentReference depth_attachment_ref = {};
    depth_attachment_ref.attachment = 1;
    depth_attachment_ref.layout = VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL;
    
    VkSubpassDescription subpass = {};
    subpass.pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS;
    subpass.colorAttachmentCount = 1;
    subpass.pColorAttachments = &color_attachment_ref;
    subpass.pDepthStencilAttachment = &depth_attachment_ref;
    
    VkSubpassDependency dependency = {};
    dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    dependency.dstSubpass = 0;
    dependency.srcStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.srcAccessMask = 0;
    dependency.dstStageMask = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    dependency.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
    
    VkSubpassDependency depth_dependency = {};
    depth_dependency.srcSubpass = VK_SUBPASS_EXTERNAL;
    depth_dependency.dstSubpass = 0;
    depth_dependency.srcStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depth_dependency.srcAccessMask = 0;
    depth_dependency.dstStageMask = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT | VK_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT;
    depth_dependency.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;
    
    VkSubpassDependency dependencies[2] = { dependency, depth_dependency };
    
    VkAttachmentDescription attachments[2] = { color_attachment, depth_attachment };
    
    VkRenderPassCreateInfo render_pass_info = {};
    render_pass_info.sType = VK_STRUCTURE_TYPE_RENDER_PASS_CREATE_INFO;
    render_pass_info.attachmentCount = 2;
    render_pass_info.pAttachments = &attachments[0];
    render_pass_info.subpassCount = 1;
    render_pass_info.pSubpasses = &subpass;
    render_pass_info.dependencyCount = 2;
    render_pass_info.pDependencies = &dependencies[0];
    
    VK_CHECK(vkCreateRenderPass(_device, &render_pass_info, nullptr, &_default));
  }
  
  void RenderPasses::init_framebuffers(uint32_t width, uint32_t height)
  {
    VkFramebufferCreateInfo fb_info = VKInit::framebuffer_create_info(_default, { width, height });
    
    const uint32_t swapchain_imagecount = (uint32_t)_swapchainHandler->_swapchainImages.size();
    _framebuffers = std::vector<VkFramebuffer>(swapchain_imagecount);
    
    for (size_t i = 0; i < swapchain_imagecount; i++) {
      
      VkImageView attachments[2];
      attachments[0] = _swapchainHandler->_swapchainImageViews[i];
      attachments[1] = _swapchainHandler->_depthImageView;
      
      fb_info.pAttachments = attachments;
      fb_info.attachmentCount = 2;
      VK_CHECK(vkCreateFramebuffer(_device, &fb_info, nullptr, &_framebuffers[i]));
    }
  }
  
  void RenderPasses::destroy()
  {
    destroy_framebuffers();
    destroy_default();
  }
  
  void RenderPasses::destroy_default()
  {
    vkDestroyRenderPass(_device, _default, nullptr);
  }
  
  void RenderPasses::destroy_framebuffers()
  {
    const uint32_t swapchain_imagecount = (uint32_t)_swapchainHandler->_swapchainImages.size();
    
    for (size_t i = 0; i < swapchain_imagecount; i++) {
      vkDestroyFramebuffer(_device, _framebuffers[i], nullptr);
      vkDestroyImageView(_device, _swapchainHandler->_swapchainImageViews[i], nullptr);
    }
  }
  
}
