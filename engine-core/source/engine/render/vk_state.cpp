#define VMA_IMPLEMENTATION
#include <vk_mem_alloc/vk_mem_alloc.h>

#include <VkBootstrap/VkBootstrap.h>

#include "vk_state.h"
#include "../engine.h"

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
    VkPhysicalDeviceShaderDrawParametersFeatures shader_draw_parameters_features = {};
    shader_draw_parameters_features.sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_SHADER_DRAW_PARAMETERS_FEATURES;
    shader_draw_parameters_features.pNext = nullptr;
    shader_draw_parameters_features.shaderDrawParameters = VK_TRUE;
    vkb::Device vkbDevice = deviceBuilder.add_pNext(&shader_draw_parameters_features).build().value();
    
    device = vkbDevice.device;
    chosenGPU = physicalDevice.physical_device;
    
    graphicsQueue = vkbDevice.get_queue(vkb::QueueType::graphics).value();
    graphicsQueueFamily = vkbDevice.get_queue_index(vkb::QueueType::graphics).value();
    
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
}
