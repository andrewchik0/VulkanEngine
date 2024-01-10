#pragma once

#include "../types.h"
#include "vulkan.h"
#include "gpu_data.h"

namespace VKEngine {
  
  class Frames;
  
  struct DescriptorSets
  {
    VkDescriptorSet globalSet;
    AllocatedBuffer sceneBuffer;
    
    VkDescriptorSet cameraSet;
    AllocatedBuffer cameraBuffer;
    
    VkDescriptorSet objectSet;
    AllocatedBuffer objectBuffer;
  };
  
  class Descriptors
  {
  public:
    VkDescriptorPool _descriptorPool{ VK_NULL_HANDLE };
    VkDescriptorSetLayout _globalLayout{ VK_NULL_HANDLE };
    VkDescriptorSetLayout _cameraLayout{ VK_NULL_HANDLE };
    VkDescriptorSetLayout _objectLayout{ VK_NULL_HANDLE };
    VkDescriptorSetLayout _textureLayout{ VK_NULL_HANDLE };
    
    void init(VkState* state, Frames* frames);
    void destroy();
    
    AllocatedBuffer create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage);

  private:
    VkState* _vkState;
    Frames* _frames;
    
    std::vector<VkDescriptorPoolSize> _sizes =
    {
      { VK_DESCRIPTOR_TYPE_SAMPLER, 1000 },
      { VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, 1000 },
      { VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_IMAGE, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_TEXEL_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_TEXEL_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, 1000 },
      { VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER_DYNAMIC, 1000 },
      { VK_DESCRIPTOR_TYPE_STORAGE_BUFFER_DYNAMIC, 1000 },
      { VK_DESCRIPTOR_TYPE_INPUT_ATTACHMENT, 1000 }
    };
  };
  
}
