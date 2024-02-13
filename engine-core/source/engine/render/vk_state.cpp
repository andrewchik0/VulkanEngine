#define VMA_IMPLEMENTATION
#include <vk_mem_alloc/vk_mem_alloc.h>

#include <VkBootstrap/VkBootstrap.h>

#include "engine/render/vk_state.h"
#include "engine/engine.h"

namespace VKEngine {
  void VkState::init()
  {
    vkb::InstanceBuilder builder;
    
    auto inst_ret = builder.set_app_name("Vulkan Engine")
      .request_validation_layers(useValidationLayers)
      .use_default_debug_messenger()
      .require_api_version(1, 1, 0)
      .build();
    
    vkb::Instance vkb_inst = inst_ret.value();
    
    instance = vkb_inst.instance;
    debug_messenger = vkb_inst.debug_messenger;
    
    Engine::get()->window().create_surface(instance, &surface);
    
    vkb::PhysicalDeviceSelector selector{ vkb_inst };
    vkb::PhysicalDevice physicalDevice = selector
      .set_minimum_version(1, 1)
      .set_surface(surface)
      .select()
      .value();
    
    vkb::DeviceBuilder deviceBuilder{ physicalDevice };
    
    VkPhysicalDeviceShaderDrawParametersFeatures shaderDrawParametersFeatures = {};
    shaderDrawParametersFeatures.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES;
    shaderDrawParametersFeatures.pNext = nullptr;
    shaderDrawParametersFeatures.shaderDrawParameters = VK_TRUE;
    
    vkb::Device vkbDevice = deviceBuilder
        .add_pNext(&shaderDrawParametersFeatures)
        .build()
        .value();
    
    device = vkbDevice.device;
    chosenGPU = physicalDevice.physical_device;
    
    // Extensions check
    uint32_t extensionsCount = 0;
    VK_CHECK(vkEnumerateDeviceExtensionProperties(chosenGPU, nullptr, &extensionsCount, nullptr));
    availableExtentions.resize(extensionsCount);
    VK_CHECK(vkEnumerateDeviceExtensionProperties(chosenGPU, nullptr, &extensionsCount, &availableExtentions[0]));
    
    // Get queue
    graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
    
    // Create VMA Allocator
    VmaAllocatorCreateInfo allocatorInfo = {};
    allocatorInfo.physicalDevice = chosenGPU;
    allocatorInfo.device = device;
    allocatorInfo.instance = instance;
    
    vmaCreateAllocator(&allocatorInfo, &allocator);
    
    vkGetPhysicalDeviceProperties(chosenGPU, &gpuProperties);
  }
  
  void VkState::destroy()
  {
    vmaDestroyAllocator(allocator);
    
    vkDestroySurfaceKHR(instance, surface, nullptr);
    
    vkDestroyDevice(device, nullptr);
    vkb::destroy_debug_utils_messenger(instance, debug_messenger);
    vkDestroyInstance(instance, nullptr);
  }
  
  void VkState::wait() { vkDeviceWaitIdle(device); }
  
  bool VkState::is_extention_available(const std::string& extName)
  {
    for (auto& ext : availableExtentions)
      if (ext.extensionName == extName)
        return true;
    return false;
  }
  
  void AllocatedBuffer::map(void* data, size_t size)
  {
    void *newData;
    vmaMapMemory(Engine::get()->render().get_vk_state().allocator, allocation, &newData);
    memcpy(newData, data, size);
    vmaUnmapMemory(Engine::get()->render().get_vk_state().allocator, allocation);
  }
}
