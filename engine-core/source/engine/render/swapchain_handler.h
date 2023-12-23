#pragma once

#include "../types.h"

namespace VKEngine {
  
  class SwapchainHandler
  {
  private:
    VkState *_vkState;
    
  public:
    VkPresentModeKHR _mode = VK_PRESENT_MODE_MAILBOX_KHR;
    VkSwapchainKHR _swapchain;
    VkFormat _swachainImageFormat;
    
    std::vector<VkImage> _swapchainImages;
    std::vector<VkImageView> _swapchainImageViews;
    
    VkImageView _depthImageView;
    VkImage _depthImage;
    VmaAllocation _depthImageAllocation;
    VkFormat _depthFormat = VK_FORMAT_D32_SFLOAT;
    
    void init(VkState* vkState, uint32_t width, uint32_t height);
    void destroy();
    
    void init(uint32_t width, uint32_t height) { init(_vkState, width, height); }
  };
  
}
