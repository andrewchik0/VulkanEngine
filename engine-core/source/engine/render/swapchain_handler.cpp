#include "VkBootstrap/VkBootstrap.h"

#include "swapchain_handler.h"
#include "../engine.h"
#include "vk_info_initializers.h"

namespace VKEngine {
  
  void SwapchainHandler::init(VkState* vkState, uint32_t width, uint32_t height)
  {
    _vkState = vkState;
    
    vkb::SwapchainBuilder swapchainBuilder{ _vkState->chosenGPU, _vkState->device, _vkState->surface };
    
    vkb::Swapchain vkbSwapchain = swapchainBuilder
      .use_default_format_selection()
      .set_desired_present_mode(_mode)
      .set_desired_extent(width, height)
      .build()
      .value();
    
    _swapchain = vkbSwapchain.swapchain;
    _swapchainImages = vkbSwapchain.get_images().value();
    _swapchainImageViews = vkbSwapchain.get_image_views().value();
    
    _swachainImageFormat = vkbSwapchain.image_format;
    
    VkExtent3D depthImageExtent = {
      width,
      height,
      1
    };
    
    VkImageCreateInfo dimg_info = VKInit::image_create_info(_depthFormat, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, depthImageExtent);
    
    VmaAllocationCreateInfo dimg_allocinfo = {};
    dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    dimg_allocinfo.requiredFlags = VkMemoryPropertyFlags(VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT);
    
    vmaCreateImage(_vkState->allocator, &dimg_info, &dimg_allocinfo, &_depthImage, &_depthImageAllocation, nullptr);
    
    VkImageViewCreateInfo dview_info = VKInit::imageview_create_info(_depthFormat, _depthImage, VK_IMAGE_ASPECT_DEPTH_BIT);;
    
    VK_CHECK(vkCreateImageView(_vkState->device, &dview_info, nullptr, &_depthImageView));
  }
  
  void SwapchainHandler::destroy()
  {
    vkDestroySwapchainKHR(_vkState->device, _swapchain, nullptr);
    
    vkDestroyImageView(_vkState->device, _depthImageView, nullptr);
    vmaDestroyImage(_vkState->allocator, _depthImage, _depthImageAllocation);
  }
  
}
