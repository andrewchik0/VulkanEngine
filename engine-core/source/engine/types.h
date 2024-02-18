#pragma once

#include "engine/pch.h"

#ifdef _DEBUG
  #define VK_CHECK(x)                                               \
    do                                                              \
    {                                                               \
      VkResult err = x;                                             \
      if (err)                                                      \
      {                                                             \
        std::cout << "Detected Vulkan error: " << err << std::endl; \
        abort();                                                    \
      }                                                             \
    } while (0)

  #define MARK_ZONE(name) (Engine::get()->time().mark_zone(name))
#else
  #define VK_CHECK(x) (x)
  #define MARK_ZONE(name)
#endif // _DEBUG

namespace VKEngine {

  struct VkState;
  struct ApplicationSpecs;
  struct RenderObject;
  
  class Engine;
  class Render;
  
  struct CommandLineArgs
  {
    size_t count;
    char** vector;
  };
  
  struct Point
  {
    float x, y;
    
    Point(float val1, float val2) : x(val1), y(val2) {}
    Point(float val) : x(val), y(val) {}
  };
  
  struct Rectangle
  {
    float x1, y1, x2, y2;
  };
  
  struct RawBuffer
  {
    size_t size = 0;
    void* data = nullptr;
    
    ~RawBuffer() { free(); }
    void free()
    {
      if (data != nullptr)
        ::free(data);
      data = nullptr;
    }
  };
  
  struct RawTexture : public RawBuffer
  {
    uint32_t width, height;
    
    ~RawTexture() { RawBuffer::~RawBuffer(); }
    
    static RawTexture plain(uint32_t color)
    {
      RawTexture tex;
      tex.width = 1;
      tex.height = 1;
      tex.size = 4;
      tex.data = new uint32_t[1];
      *((uint32_t*)tex.data) = color;
      return tex;
    }
    
    void add_texture(void* mem, uint32_t imgWidth, uint32_t imgHeight, uint32_t pixelSize, uint32_t posx = 0, uint32_t posy = 0)
    {
      uint32_t maxHeight = fmin(height - posy, imgHeight);
      for (uint32_t i = 0; i < maxHeight; i++)
        for (uint32_t j = 0; j < imgWidth * pixelSize; j++)
          *((int8_t*)(data) + (i + posy) * width * pixelSize + posx * pixelSize + j) += *((int8_t*)(mem) + i * imgWidth * pixelSize + j);
    }
  };
  
  struct AllocatedBuffer
  {
    VkBuffer buffer;
    VmaAllocation allocation;
    
    void map(void* data, size_t size);
    void map(const RawBuffer& memory) { map(memory.data, memory.size); }
  };
}
