#pragma once

#include "engine/utils/function_queue.h"
#include "engine/render/vk_info_initializers.h"
#include "engine/render/swapchain_handler.h"
#include "engine/render/render_passes.h"
#include "engine/render/descriptors.h"

namespace VKEngine {
  
  constexpr uint8_t FRAME_OVERLAP = 2;
  
  struct DescriptorSets;
  
  struct FrameData
  {
    VkSemaphore presentSemaphore, renderSemaphore;
    VkFence renderFence;
    
    Utils::FunctionQueue frameDeletionQueue;
    
    VkCommandPool commandPool{ VK_NULL_HANDLE };
    VkCommandBuffer mainCommandBuffer{ VK_NULL_HANDLE };
    
    DescriptorSets descriptorSets;
  };
  
  class Frames
  {
  public:
    void init(VkState* state, SwapchainHandler* swapchain, RenderPasses *passes);
    void init()
      { init(_vkState, _swapchainHandler, _renderPasses); }
    
    void destroy();
    
    FrameData& current();
    FrameData& last();
    
    uint64_t operator++(int32_t i);
    FrameData& operator[](size_t index);
    
    void start();
    void end();
    
    void set_clear_value(VkClearColorValue clearValue)
      { _clearValue = clearValue; }

  private:
    VkState* _vkState;
    SwapchainHandler* _swapchainHandler;
    RenderPasses *_renderPasses;
    
    const uint32_t _timeout = (uint32_t)1e9;
    FrameData _frames[FRAME_OVERLAP];
    uint64_t _frameNumber{0};
    
    VkClearColorValue _clearValue = {{ 0.2f, 0.2f, 0.2f, 1.f }};
    uint32_t swapchainImageIndex;
  };
  
}
