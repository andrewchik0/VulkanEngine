#include "frames.h"
#include "../engine.h"

namespace VKEngine {
  
  void Frames::init(VkState* state, SwapchainHandler* swapchain, RenderPasses *passes)
  {
    _vkState = state;
    _swapchainHandler = swapchain;
    _renderPasses = passes;
    
    VkCommandPoolCreateInfo commandPoolInfo = VKInit::command_pool_create_info(_vkState->graphicsQueueFamily, VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT);
    VkFenceCreateInfo fenceCreateInfo = VKInit::fence_create_info(VK_FENCE_CREATE_SIGNALED_BIT);
    VkSemaphoreCreateInfo semaphoreCreateInfo = VKInit::semaphore_create_info();
    
    for (size_t i = 0; i < FRAME_OVERLAP; i++) {
      VK_CHECK(vkCreateCommandPool(_vkState->device, &commandPoolInfo, nullptr, &_frames[i].commandPool));
      
      VkCommandBufferAllocateInfo cmdAllocInfo = VKInit::command_buffer_allocate_info(_frames[i].commandPool, 1);
      
      VK_CHECK(vkAllocateCommandBuffers(_vkState->device, &cmdAllocInfo, &_frames[i].mainCommandBuffer));
      VK_CHECK(vkCreateFence(_vkState->device, &fenceCreateInfo, nullptr, &_frames[i].renderFence));
      VK_CHECK(vkCreateSemaphore(_vkState->device, &semaphoreCreateInfo, nullptr, &_frames[i].presentSemaphore));
      VK_CHECK(vkCreateSemaphore(_vkState->device, &semaphoreCreateInfo, nullptr, &_frames[i].renderSemaphore));
    }
  }
  
  void Frames::destroy()
  {
    VK_CHECK(vkWaitForFences(_vkState->device, 1, &current().renderFence, true, _timeout));
    
    for (size_t i = 0; i < FRAME_OVERLAP; i++) {
      vkDestroyCommandPool(_vkState->device, _frames[i].commandPool, nullptr);
      vkDestroyFence(_vkState->device, _frames[i].renderFence, nullptr);
      vkDestroySemaphore(_vkState->device, _frames[i].presentSemaphore, nullptr);
      vkDestroySemaphore(_vkState->device, _frames[i].renderSemaphore, nullptr);
    }
  }
  
  FrameData& Frames::current()
  {
    return _frames[_frameNumber % FRAME_OVERLAP];
  }
  
  FrameData& Frames::last()
  {
    return _frames[(_frameNumber - 1) % 2];
  }
  
  uint64_t Frames::operator++(int32_t i)
  {
    return _frameNumber++;
  }
  
  FrameData& Frames::operator[](size_t index)
  {
    return _frames[index];
  }
  
  void Frames::start()
  {
    VK_CHECK(vkWaitForFences(_vkState->device, 1, &current().renderFence, true, _timeout));
    VK_CHECK(vkResetFences(_vkState->device, 1, &current().renderFence));
    
    VK_CHECK(vkResetCommandBuffer(current().mainCommandBuffer, 0));
    
    VK_CHECK(vkAcquireNextImageKHR(_vkState->device, _swapchainHandler->_swapchain, _timeout, current().presentSemaphore, nullptr, &swapchainImageIndex));
    
    VkCommandBufferBeginInfo cmdBeginInfo = VKInit::command_buffer_begin_info(VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT);
    VK_CHECK(vkBeginCommandBuffer(current().mainCommandBuffer, &cmdBeginInfo));
    
    VkRenderPassBeginInfo rpInfo = VKInit::renderpass_begin_info(_renderPasses->_default, Engine::get()->window().ext(), _renderPasses->_framebuffers[swapchainImageIndex]);
    
    VkClearValue clearValue;
    clearValue.color = _clearValue;
    VkClearValue depthClear;
    depthClear.depthStencil.depth = 1.f;
    rpInfo.clearValueCount = 2;
    
    VkClearValue clearValues[] = { clearValue, depthClear };
    rpInfo.pClearValues = &clearValues[0];
    
    vkCmdBeginRenderPass(current().mainCommandBuffer, &rpInfo, VK_SUBPASS_CONTENTS_INLINE);
  }
  
  void Frames::end()
  {
    vkCmdEndRenderPass(current().mainCommandBuffer);
    VK_CHECK(vkEndCommandBuffer(current().mainCommandBuffer));
    
    VkSubmitInfo submit = VKInit::submit_info(&current().mainCommandBuffer);
    VkPipelineStageFlags waitStage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    
    submit.pWaitDstStageMask = &waitStage;
    submit.waitSemaphoreCount = 1;
    submit.pWaitSemaphores = &current().presentSemaphore;
    submit.signalSemaphoreCount = 1;
    submit.pSignalSemaphores = &current().renderSemaphore;
    
    VK_CHECK(vkQueueSubmit(_vkState->graphicsQueue, 1, &submit, current().renderFence));
    
    VkPresentInfoKHR presentInfo = VKInit::present_info();
    
    presentInfo.pSwapchains = &_swapchainHandler->_swapchain;
    presentInfo.swapchainCount = 1;
    presentInfo.pWaitSemaphores = &current().renderSemaphore;
    presentInfo.waitSemaphoreCount = 1;
    presentInfo.pImageIndices = &swapchainImageIndex;
    
    VK_CHECK(vkQueuePresentKHR(_vkState->graphicsQueue, &presentInfo));
  }
  
}
