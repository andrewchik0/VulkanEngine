#pragma once

#include "engine/render/swapchain_handler.h"

namespace VKEngine {
  
  class RenderPasses
  {
  public:
    VkRenderPass _default;
    std::vector<VkFramebuffer> _framebuffers;
    
    void init(VkDevice device, SwapchainHandler* swapchain, uint32_t width, uint32_t height);
    void init(uint32_t width, uint32_t height) { init(_device, _swapchainHandler, width, height); }
    void destroy();

  private:
    VkDevice _device;
    SwapchainHandler* _swapchainHandler;
    
    void init_default();
    void init_framebuffers(uint32_t width, uint32_t height);
    void destroy_default();
    void destroy_framebuffers();
  };
  
}
