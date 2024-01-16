#pragma once

#include "resources/resources.h"

#include "vk_state.h"
#include "frames.h"
#include "swapchain_handler.h"
#include "render_passes.h"
#include "upload_context.h"
#include "descriptors.h"

namespace VKEngine {
  
  struct RenderObject
  {
    Mesh* mesh;
    Material* material;
    glm::mat4 transformMatrix;
  };
  
  class Render
  {
  public:
    
    void add_object(RenderObject& object) { _renderables.push_back(object); }
    VkState& get_vk_state() { return _vkState; }
    uint32_t& width() { return *_width; }
    uint32_t& height() { return *_height; }
    
    SwapchainHandler _swapchainHandler;
    RenderPasses _renderPasses;
    Descriptors _descriptors;
    Pipelines _pipelines;
    Materials _materials;
    Textures _textures;
    Meshes _meshes;
    Frames _frames;
    UploadContext _uploadContext;
    
    void init(uint32_t *width, uint32_t* height);
    void cleanup();
    void recreate();
    void draw();

  private:
    uint32_t *_width, *_height;
    VkState _vkState;
    
    std::vector<RenderObject> _renderables;

    void draw_objects(VkCommandBuffer cmd, RenderObject* first, uint32_t count);
    
    size_t pad_uniform_buffer_size(size_t originalSize);
  };
  
}
