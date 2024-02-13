#pragma once

#include "engine/render/resources/resource_manager.h"

namespace VKEngine {
  
  class ImageType
  {
  public:
    enum Type
    {
      RGB,
      RGBA,
    };
    
    ImageType(Type type)
      : _type(type)
    {}
    
    uint32_t to_stb_enum();
    VkFormat to_vk_enum();
    uint8_t get_pixel_size();
    
  private:
    Type _type;
  };
  
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
    Texture* create_from_file(const std::string& name, const std::string& filename, ImageType type = ImageType::RGBA);
    Texture* create_from_memory(const std::string& name, const RawBuffer& buffer, uint32_t width, uint32_t height, ImageType type = ImageType::RGBA);
    
  private:
  };
  
}
