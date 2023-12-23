#pragma once

#include "../types.h"
#include "vulkan.h"

namespace VKEngine {
  
  class UploadContext {
  private:
    VkState* _vkState;
    
  public:
    VkFence _uploadFence;
    VkCommandPool _commandPool;
    VkCommandBuffer _commandBuffer;
    
    void init(VkState *state);
    void destroy();
    void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);
  };
  
}
