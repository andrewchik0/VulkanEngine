#pragma once

#include "pch.h"

#include <vulkan/vulkan.h>
#include <vk_mem_alloc/vk_mem_alloc.h>

#ifdef DEBUG
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
#endif // DEBUG

namespace VKEngine {

  struct VkState;

  class Engine;
  class Render;
  
  struct AllocatedBuffer
  {
    VkBuffer buffer;
    VmaAllocation allocation;
  };
  
}
