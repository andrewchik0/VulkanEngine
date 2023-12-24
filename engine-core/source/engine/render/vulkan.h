#pragma once

#include <VkBootstrap/VkBootstrap.h>
#include "../types.h"

namespace VKEngine {
  
  struct VkState
  {
    bool useValidationLayers{ true };
    VkInstance instance;
    VkDebugUtilsMessengerEXT debug_messenger;
    VkPhysicalDeviceProperties gpuProperties;
    VkQueue graphicsQueue;
    uint32_t graphicsQueueFamily;
    VkDevice device;
    VkPhysicalDevice chosenGPU;
    VkSurfaceKHR surface;
    VmaAllocator allocator;
    
    void init();
    void destroy();
    void wait();
  };
  
}
