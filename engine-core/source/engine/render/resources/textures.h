#pragma once

#include "resource_manager.h"

namespace VKEngine {
  
  struct Texture : public IResource
  {
    VkDescriptorSet descriptorsSet;
    VkImage image;
    VmaAllocation allocation;
    VkImageView imageView;
    VkSampler blockySampler;
    
    void destroy(Render *render) override;
  };
  
  class Textures : public ResourceManager<std::string, Texture>
  {
  private:
    Texture load_from_file(const std::string& filename);
  public:
    Texture* create(const std::string& name, const std::string& filename) { return &(_resources[name] = load_from_file(filename)); }
  };
  
}
