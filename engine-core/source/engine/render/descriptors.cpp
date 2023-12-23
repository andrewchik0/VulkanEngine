#include "descriptors.h"
#include "frames.h"

namespace VKEngine {
  
  void Descriptors::init(VkState* state, Frames* frames)
  {
    _vkState = state;
    _frames = frames;
    
    VkDescriptorPoolCreateInfo pool_info = {};
    pool_info.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    pool_info.flags = 0;
    pool_info.maxSets = 1000;
    pool_info.poolSizeCount = (uint32_t)_sizes.size();
    pool_info.pPoolSizes = _sizes.data();
    
    vkCreateDescriptorPool(_vkState->device, &pool_info, nullptr, &_descriptorPool);
    
    VkDescriptorSetLayoutBinding sceneBind = VKInit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, SCENE_DATA_BINDING);
    VkDescriptorSetLayoutBinding cameraBind = VKInit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, CAMERA_DATA_BINDING);
    VkDescriptorSetLayoutBinding objectBind = VKInit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, VK_SHADER_STAGE_VERTEX_BIT | VK_SHADER_STAGE_FRAGMENT_BIT, OBJECT_DATA_BINDING);
    VkDescriptorSetLayoutBinding textureBind = VKInit::descriptorset_layout_binding(VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT, 0);
    
    VkDescriptorSetLayoutCreateInfo globalSetInfo = {};
    globalSetInfo.bindingCount = 1;
    globalSetInfo.pBindings = &sceneBind;
    globalSetInfo.flags = 0;
    globalSetInfo.pNext = nullptr;
    globalSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    vkCreateDescriptorSetLayout(_vkState->device, &globalSetInfo, nullptr, &_globalLayout);
    
    VkDescriptorSetLayoutCreateInfo cameraSetInfo = {};
    cameraSetInfo.bindingCount = 1;
    cameraSetInfo.pBindings = &cameraBind;
    cameraSetInfo.flags = 0;
    cameraSetInfo.pNext = nullptr;
    cameraSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    vkCreateDescriptorSetLayout(_vkState->device, &cameraSetInfo, nullptr, &_cameraLayout);
    
    VkDescriptorSetLayoutCreateInfo objectSetInfo = {};
    objectSetInfo.bindingCount = 1;
    objectSetInfo.pBindings = &objectBind;
    objectSetInfo.flags = 0;
    objectSetInfo.pNext = nullptr;
    objectSetInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    vkCreateDescriptorSetLayout(_vkState->device, &objectSetInfo, nullptr, &_objectLayout);
    
    VkDescriptorSetLayoutCreateInfo texturesSetinfo = {};
    texturesSetinfo.bindingCount = 1;
    texturesSetinfo.pBindings = &textureBind;
    texturesSetinfo.flags = 0;
    texturesSetinfo.pNext = nullptr;
    texturesSetinfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    vkCreateDescriptorSetLayout(_vkState->device, &texturesSetinfo, nullptr, &_textureLayout);
    
    for (size_t i = 0; i < FRAME_OVERLAP; i++)
    {
      const uint32_t MAX_OBJECTS = 10000;
      
      (*_frames)[i].descriptorSets.sceneBuffer = create_buffer(sizeof(GPUSceneData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
      (*_frames)[i].descriptorSets.cameraBuffer = create_buffer(sizeof(GPUCameraData), VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
      (*_frames)[i].descriptorSets.objectBuffer = create_buffer(sizeof(GPUObjectData) * MAX_OBJECTS, VK_BUFFER_USAGE_STORAGE_BUFFER_BIT, VMA_MEMORY_USAGE_CPU_TO_GPU);
      
      VkDescriptorSetAllocateInfo allocInfo = {};
      VkDescriptorSet sets[] =
      {
        (*_frames)[i].descriptorSets.globalSet,
        (*_frames)[i].descriptorSets.cameraSet,
        (*_frames)[i].descriptorSets.objectSet
      };
      VkDescriptorSetLayout layouts[] =
      {
        _globalLayout,
        _cameraLayout,
        _objectLayout,
      };
      allocInfo.pNext = nullptr;
      allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
      allocInfo.descriptorPool = _descriptorPool;
      allocInfo.descriptorSetCount = 3;
      allocInfo.pSetLayouts = layouts;
      VK_CHECK(vkAllocateDescriptorSets(_vkState->device, &allocInfo, sets));
      
      (*_frames)[i].descriptorSets.globalSet = sets[0];
      (*_frames)[i].descriptorSets.cameraSet = sets[1];
      (*_frames)[i].descriptorSets.objectSet = sets[2];
      
      VkDescriptorBufferInfo cameraInfo;
      cameraInfo.buffer = (*_frames)[i].descriptorSets.cameraBuffer.buffer;
      cameraInfo.offset = 0;
      cameraInfo.range = sizeof(GPUCameraData);
      
      VkDescriptorBufferInfo sceneInfo;
      sceneInfo.buffer = (*_frames)[i].descriptorSets.sceneBuffer.buffer;
      sceneInfo.offset = 0;
      sceneInfo.range = sizeof(GPUSceneData);
      
      VkDescriptorBufferInfo objectBufferInfo;
      objectBufferInfo.buffer = (*_frames)[i].descriptorSets.objectBuffer.buffer;
      objectBufferInfo.offset = 0;
      objectBufferInfo.range = sizeof(GPUObjectData) * MAX_OBJECTS;
      
      VkWriteDescriptorSet sceneWrite = VKInit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, (*_frames)[i].descriptorSets.globalSet, &sceneInfo, SCENE_DATA_BINDING);
      VkWriteDescriptorSet cameraWrite = VKInit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, (*_frames)[i].descriptorSets.cameraSet, &cameraInfo, CAMERA_DATA_BINDING);
      VkWriteDescriptorSet objectWrite = VKInit::write_descriptor_buffer(VK_DESCRIPTOR_TYPE_STORAGE_BUFFER, (*_frames)[i].descriptorSets.objectSet, &objectBufferInfo, OBJECT_DATA_BINDING);
      VkWriteDescriptorSet setWrites[] = { sceneWrite, cameraWrite, objectWrite };
      
      vkUpdateDescriptorSets(_vkState->device, 3, setWrites, 0, nullptr);
    }
  }
  
  void Descriptors::destroy()
  {
    vkDestroyDescriptorSetLayout(_vkState->device, _globalLayout, nullptr);
    vkDestroyDescriptorSetLayout(_vkState->device, _cameraLayout, nullptr);
    vkDestroyDescriptorSetLayout(_vkState->device, _objectLayout, nullptr);
    vkDestroyDescriptorSetLayout(_vkState->device, _textureLayout, nullptr);
    
    vkDestroyDescriptorPool(_vkState->device, _descriptorPool, nullptr);
    
    for (size_t i = 0; i < FRAME_OVERLAP; i++)
    {
      vmaDestroyBuffer(_vkState->allocator, (*_frames)[i].descriptorSets.sceneBuffer.buffer, (*_frames)[i].descriptorSets.sceneBuffer.allocation);
      vmaDestroyBuffer(_vkState->allocator, (*_frames)[i].descriptorSets.cameraBuffer.buffer, (*_frames)[i].descriptorSets.cameraBuffer.allocation);
      vmaDestroyBuffer(_vkState->allocator, (*_frames)[i].descriptorSets.objectBuffer.buffer, (*_frames)[i].descriptorSets.objectBuffer.allocation);
    }
  }
  
  AllocatedBuffer Descriptors::create_buffer(size_t allocSize, VkBufferUsageFlags usage, VmaMemoryUsage memoryUsage)
  {
    VkBufferCreateInfo bufferInfo = {};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.pNext = nullptr;
    bufferInfo.size = allocSize;
    bufferInfo.usage = usage;
    
    VmaAllocationCreateInfo vmaallocInfo = {};
    vmaallocInfo.usage = memoryUsage;
    
    AllocatedBuffer newBuffer;
    
    VK_CHECK(vmaCreateBuffer(_vkState->allocator, &bufferInfo, &vmaallocInfo,
                             &newBuffer.buffer,
                             &newBuffer.allocation,
                             nullptr));
    
    return newBuffer;
  }
  
}
