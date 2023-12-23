#include "upload_context.h"
#include "../engine.h"

namespace VKEngine {
  
  void UploadContext::init(VkState* state)
  {
    _vkState = state;
    
    VkCommandPoolCreateInfo uploadCommandPoolInfo = VKInit::command_pool_create_info(_vkState->graphicsQueueFamily);
    VK_CHECK(vkCreateCommandPool(_vkState->device, &uploadCommandPoolInfo, nullptr, &_commandPool));
    
    VkCommandBufferAllocateInfo cmdAllocInfo = VKInit::command_buffer_allocate_info(_commandPool, 1);
    VK_CHECK(vkAllocateCommandBuffers(_vkState->device, &cmdAllocInfo, &_commandBuffer));
    
    VkFenceCreateInfo uploadFenceCreateInfo = VKInit::fence_create_info();
    VK_CHECK(vkCreateFence(_vkState->device, &uploadFenceCreateInfo, nullptr, &_uploadFence));
  }
  
  void UploadContext::destroy()
  {
    vkDestroyCommandPool(_vkState->device, _commandPool, nullptr);
    vkDestroyFence(_vkState->device, _uploadFence, nullptr);
  }
  
  void UploadContext::immediate_submit(std::function<void(VkCommandBuffer cmd)>&& function)
  {
    VkCommandBuffer cmd = _commandBuffer;
    
    VkCommandBufferBeginInfo cmdBeginInfo = VKInit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    
    VK_CHECK(vkBeginCommandBuffer(cmd, &cmdBeginInfo));
    
    function(cmd);
    
    VK_CHECK(vkEndCommandBuffer(cmd));
    
    VkSubmitInfo submit = VKInit::submit_info(&cmd);
    
    VK_CHECK(vkQueueSubmit(_vkState->graphicsQueue, 1, &submit, _uploadFence));
    
    vkWaitForFences(_vkState->device, 1, &_uploadFence, true, 9999999999);
    vkResetFences(_vkState->device, 1, &_uploadFence);
    
    vkResetCommandPool(_vkState->device, _commandPool, 0);
  }
  
}
