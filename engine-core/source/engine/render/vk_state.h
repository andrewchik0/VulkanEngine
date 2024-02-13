#pragma once

#include "engine/types.h"

namespace VKEngine {
  
  struct VkState
  {
    bool useValidationLayers
    {
#ifndef NDEBUG
      true
#else
      false
#endif
    };
    std::vector<VkExtensionProperties> availableExtentions;
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
    
    bool is_extention_available(const std::string& extName);
  };
  
}
