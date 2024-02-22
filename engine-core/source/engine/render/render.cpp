#include "engine/render/render.h"
#include "engine/engine.h"

namespace VKEngine {
  
  void Render::init(uint32_t* width, uint32_t* height)
  {
    _width = width;
    _height = height;
    
    _vkState.init();
    _swapchainHandler.init(&_vkState, *width, *height);
    _renderPasses.init(_vkState.device, &_swapchainHandler, *width, *height);
    _frames.init(&_vkState, &_swapchainHandler, &_renderPasses);
    _uploadContext.init(&_vkState);
    _descriptors.init(&_vkState, &_frames);
    _pipelines.init(this);
    _textures.init(this);
    _meshes.init(this);
    _gui.init();
  }
  
  void Render::draw()
  {
    _frames.start();
    MARK_ZONE("frame started");
    
    send_data(_renderables.data(), (uint32_t)_renderables.size());
    MARK_ZONE("sended_data");
    
    draw_objects(_frames.current().mainCommandBuffer, _renderables.data(), (uint32_t)_renderables.size());
    MARK_ZONE("objects rendered");
    
    _gui.render(_frames.current().mainCommandBuffer);
    MARK_ZONE("GUI rendered");
    
    _frames.end();
    _frames++;
    MARK_ZONE("frame ended");
  }
  
  void Render::recreate()
  {
    _vkState.wait();
    
    _gui.resize();
    
    _frames.destroy();
    _renderPasses.destroy();
    _swapchainHandler.destroy();
    
    _swapchainHandler.init(*_width, *_height);
    _renderPasses.init(*_width, *_height);
    _frames.init();
    
    _pipelines.reload();
  }
  
  void Render::cleanup()
  {
    _vkState.wait();
    
    _fontHandler.free();
    _pipelines.destroy();
    _descriptors.destroy();
    _uploadContext.destroy();
    _frames.destroy();
    _renderPasses.destroy();
    _swapchainHandler.destroy();
    _textures.destroy();
    _meshes.destroy();
    _vkState.destroy();
  }
  
  void Render::send_data(RenderObject* first, uint32_t count)
  {
    GPUCameraData camData;
    
    Camera& cam = Engine::get()->camera();
    camData.proj = cam.proj();
    camData.view = cam.view();
    camData.viewproj = cam.viewProj();
    camData.pos = glm::vec4(cam.pos(), 1.0);
    camData.lookAt = glm::vec4(cam.lookAt(), 1.0);
    camData.up = glm::vec4(cam.up(), 1.0);
    camData.screenSize = glm::vec4(Engine::get()->window().width(), Engine::get()->window().height(), 0.0f, 0.0f);
    
    GPUSceneData scData = {};
    scData.sunlightColor = glm::vec4(1, 1, 1, 1);
    
    _frames.current().descriptorSets.cameraBuffer.map(&camData, sizeof(GPUCameraData));
    _frames.current().descriptorSets.sceneBuffer.map(&scData, sizeof(GPUSceneData));
    
    void* objectData;
    vmaMapMemory(_vkState.allocator, _frames.current().descriptorSets.objectBuffer.allocation, &objectData);
    GPUObjectData* objectSSBO = (GPUObjectData*)objectData;
    for (size_t i = 0; i < count; i++)
    {
      RenderObject& object = first[i];
      objectSSBO[i].modelMatrix = object.transformMatrix;
    }
    vmaUnmapMemory(_vkState.allocator, _frames.current().descriptorSets.objectBuffer.allocation);
  }
  
  void Render::draw_objects(VkCommandBuffer cmd, RenderObject* first, uint32_t count)
  {
    Pipeline* pipeline = nullptr;
    
    for (uint32_t i = 0; i < count; i++)
    {
      RenderObject& object = first[i];
      
      //if (pipeline != object.material->pipeline)
      {
        pipeline = object.material->pipeline;
        object.material->pipeline->bind(cmd);
        
        object.material->pipeline->bind_descriptor(cmd, 0, _frames.current().descriptorSets.globalSet);
        object.material->pipeline->bind_descriptor(cmd, 1, _frames.current().descriptorSets.cameraSet);
        object.material->pipeline->bind_descriptor(cmd, 2, _frames.current().descriptorSets.objectSet);
        
        if (object.material->texture != nullptr)
        {
          object.material->pipeline->bind_descriptor(cmd, 3, object.material->texture->descriptorsSet);
        }
      }
      
      VkDeviceSize offset = 0;
      vkCmdBindVertexBuffers(cmd, 0, 1, &object.mesh->vertexBuffer.buffer, &offset);
      
      vkCmdDraw(cmd, object.mesh->vertices.size(), 1, 0, i);
      
    }
  }
  
  size_t Render::pad_uniform_buffer_size(size_t originalSize)
  {
    // Calculate required alignment based on minimum device offset alignment
    size_t minUboAlignment = _vkState.gpuProperties.limits.minUniformBufferOffsetAlignment;
    size_t alignedSize = originalSize;
    if (minUboAlignment > 0) {
      alignedSize = (alignedSize + minUboAlignment - 1) & ~(minUboAlignment - 1);
    }
    return alignedSize;
  }
}
