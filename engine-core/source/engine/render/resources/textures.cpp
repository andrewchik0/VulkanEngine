#define STB_IMAGE_IMPLEMENTATION
#include <stb_image/stb_image.h>

#include "engine/render/resources/textures.h"
#include "engine/engine.h"

namespace VKEngine {
  
  uint32_t ImageType::to_stb_enum()
  {
    switch (_type)
    {
    case RGB:
      return STBI_rgb;
      break;
    case RGBA:
      return STBI_rgb_alpha;
      break;
        
    default:
      return 0;
    }
  }
  
  VkFormat ImageType::to_vk_enum()
  {
    switch (_type)
    {
    case RGB:
      return VK_FORMAT_R8G8B8_SRGB;
    case RGBA:
      return VK_FORMAT_R8G8B8A8_SRGB;
    default:
      return VK_FORMAT_UNDEFINED;
    }
  }
  
  uint8_t ImageType::get_pixel_size()
  {
    switch (_type)
    {
    case RGB:
      return 3;
    case RGBA:
      return 4;
    default:
      return 0;
    }
  }
  
  Texture* Textures::create_from_memory(const std::string& name, const RawBuffer& buffer, uint32_t width, uint32_t height, ImageType type /* = ImageType::RGBA */)
  {
    if (buffer.size < width * height * type.get_pixel_size())
      assert_msg("Image buffer overflow");
    
    void* pixel_ptr = buffer.data;
    VkDeviceSize imageSize = buffer.size;
    
    VkFormat image_format = type.to_vk_enum();
    
    AllocatedBuffer stagingBuffer = _render->_descriptors.create_buffer(imageSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VMA_MEMORY_USAGE_CPU_ONLY);
    
    stagingBuffer.map(pixel_ptr, static_cast<size_t>(imageSize));
    
    VkExtent3D imageExtent;
    imageExtent.width = static_cast<uint32_t>(width);
    imageExtent.height = static_cast<uint32_t>(height);
    imageExtent.depth = 1;
    
    VkImageCreateInfo dimg_info = VKInit::image_create_info(image_format, VK_IMAGE_USAGE_SAMPLED_BIT | VK_IMAGE_USAGE_TRANSFER_DST_BIT, imageExtent);
    
    Texture texture;
    
    VmaAllocationCreateInfo dimg_allocinfo = {};
    dimg_allocinfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    
    // allocate and create the image
    vmaCreateImage(_render->get_vk_state().allocator, &dimg_info, &dimg_allocinfo, &texture.image, &texture.allocation, nullptr);
    
    // transition image to transfer-receiver
    _render->_uploadContext.immediate_submit([&](VkCommandBuffer cmd) {
      VkImageSubresourceRange range;
      range.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      range.baseMipLevel = 0;
      range.levelCount = 1;
      range.baseArrayLayer = 0;
      range.layerCount = 1;
      
      VkImageMemoryBarrier imageBarrier_toTransfer = {};
      imageBarrier_toTransfer.sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER;
      
      imageBarrier_toTransfer.oldLayout = VK_IMAGE_LAYOUT_UNDEFINED;
      imageBarrier_toTransfer.newLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      imageBarrier_toTransfer.image = texture.image;
      imageBarrier_toTransfer.subresourceRange = range;
      
      imageBarrier_toTransfer.srcAccessMask = 0;
      imageBarrier_toTransfer.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      
      // barrier the image into the transfer-receive layout
      vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT, VK_PIPELINE_STAGE_TRANSFER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toTransfer);
      
      VkBufferImageCopy copyRegion = {};
      copyRegion.bufferOffset = 0;
      copyRegion.bufferRowLength = 0;
      copyRegion.bufferImageHeight = 0;
      
      copyRegion.imageSubresource.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
      copyRegion.imageSubresource.mipLevel = 0;
      copyRegion.imageSubresource.baseArrayLayer = 0;
      copyRegion.imageSubresource.layerCount = 1;
      copyRegion.imageExtent = imageExtent;
      
      // copy the buffer into the image
      vkCmdCopyBufferToImage(cmd, stagingBuffer.buffer, texture.image, VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL, 1, &copyRegion);
      
      VkImageMemoryBarrier imageBarrier_toReadable = imageBarrier_toTransfer;
      
      imageBarrier_toReadable.oldLayout = VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL;
      imageBarrier_toReadable.newLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
      
      imageBarrier_toReadable.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
      imageBarrier_toReadable.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;
      
      // barrier the image into the shader readable layout
      vkCmdPipelineBarrier(cmd, VK_PIPELINE_STAGE_TRANSFER_BIT, VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT, 0, 0, nullptr, 0, nullptr, 1, &imageBarrier_toReadable);
    });
    
    vmaDestroyBuffer(_render->get_vk_state().allocator, stagingBuffer.buffer, stagingBuffer.allocation);
    
    VkImageViewCreateInfo imageinfo = VKInit::imageview_create_info(VK_FORMAT_R8G8B8A8_SRGB, texture.image, VK_IMAGE_ASPECT_COLOR_BIT);
    vkCreateImageView(_render->get_vk_state().device, &imageinfo, nullptr, &texture.imageView);
    
    VkDescriptorSetAllocateInfo allocInfo = {};
    allocInfo.pNext = nullptr;
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = _render->_descriptors._descriptorPool;
    allocInfo.descriptorSetCount = 1;
    allocInfo.pSetLayouts = &_render->_descriptors._textureLayout;
    
    vkAllocateDescriptorSets(_render->get_vk_state().device, &allocInfo, &texture.descriptorsSet);
    
    VkSamplerCreateInfo samplerInfo = VKInit::sampler_create_info(VK_FILTER_NEAREST);
    
    vkCreateSampler(_render->get_vk_state().device, &samplerInfo, nullptr, &texture.blockySampler);
    
    VkDescriptorImageInfo imageBufferInfo;
    imageBufferInfo.sampler = texture.blockySampler;
    imageBufferInfo.imageView = texture.imageView;
    imageBufferInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
    
    VkWriteDescriptorSet texture1 = VKInit::write_descriptor_image(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, texture.descriptorsSet, &imageBufferInfo, 0);
    
    vkUpdateDescriptorSets(_render->get_vk_state().device, 1, &texture1, 0, nullptr);
    
    return &(_resources[name] = texture);
  }
  
  Texture* Textures::create_from_file(const std::string& name, const std::string& filename, ImageType type /* = ImageType::RGBA */)
  {
    int32_t texWidth, texHeight, texChannels;
    
    stbi_uc* pixels = stbi_load(filename.c_str(), &texWidth, &texHeight, &texChannels, STBI_rgb_alpha);
    
    if (!pixels)
    {
      assert_msg("Filed to load a texture: " + filename);
      return nullptr;
    }
    RawBuffer buffer;
    buffer.size = type.get_pixel_size() * texWidth * texHeight;
    buffer.data = pixels;
    
    create_from_memory(name, buffer, texWidth, texHeight, type);
    
    stbi_image_free(pixels);
    buffer.data = nullptr;
    
    return &_resources[name];
  }
  
  void Texture::destroy(Render* render)
  {
    vmaDestroyImage(render->get_vk_state().allocator, image, allocation);
    vkDestroyImageView(render->get_vk_state().device, imageView, nullptr);
    vkDestroySampler(render->get_vk_state().device, blockySampler, nullptr);
  }
  
}
