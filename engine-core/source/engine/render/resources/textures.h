#pragma once

#include "resource_manager.h"

namespace VKEngine {
  
  struct Texture : public IResource
  {
    VkDescriptorSet descriptorsSet{ VK_NULL_HANDLE };
    VkImage image{ VK_NULL_HANDLE };
    VmaAllocation allocation{ VK_NULL_HANDLE };
    VkImageView imageView{ VK_NULL_HANDLE };
    VkSampler blockySampler{ VK_NULL_HANDLE };
    
    void destroy(Render *render) override;
  };
  
  class Textures : public ResourceManager<std::string, Texture>
  {
  public:
    Texture* create(const std::string& name, const std::string& filename) { return &(_resources[name] = load_from_file(filename)); }

  private:
    Texture load_from_file(const std::string& filename);
  };
  
}
