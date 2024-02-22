#pragma once

#include "engine/types.h"
#include "engine/render/vk_state.h"

namespace VKEngine {
  
  class UploadContext {
  public:
    VkFence _uploadFence{ VK_NULL_HANDLE };
    VkCommandPool _commandPool{ VK_NULL_HANDLE };
    VkCommandBuffer _commandBuffer{ VK_NULL_HANDLE };

    UploadContext()
      {}
    
    void init(VkState *state);
    void destroy();
    void immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function);

  private:
    VkState* _vkState = nullptr;
  };
  
}
