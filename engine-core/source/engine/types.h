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
  
  class Engine;
  class Render;
  
  struct CommandLineArgs
  {
    size_t count;
    char** vector;
  };
  
  struct RawBuffer
  {
    size_t size = 0;
    void* data = nullptr;
    
    ~RawBuffer() { free(); }
    void free() { if (data != nullptr) ::free(data); }
  };
  
  struct AllocatedBuffer
  {
    VkBuffer buffer;
    VmaAllocation allocation;
    
    void map(void* data, size_t size);
    void map(const RawBuffer& memory) { map(memory.data, memory.size); }
  };
}
