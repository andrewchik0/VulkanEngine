#pragma once

#include "engine/render/resources/resources.h"
#include "engine/render/gui/gui.h"
#include "engine/render/vk_state.h"
#include "engine/render/frames.h"
#include "engine/render/swapchain_handler.h"
#include "engine/render/render_passes.h"
#include "engine/render/upload_context.h"
#include "engine/render/descriptors.h"
#include "engine/render/font.h"

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
    GUI _gui;
    FontHandler _fontHandler;
    
    void init(uint32_t *width, uint32_t* height);
    void cleanup();
    void recreate();
    void draw();

    void draw_objects(VkCommandBuffer cmd, RenderObject* first, uint32_t count);
  private:
    uint32_t *_width, *_height;
    VkState _vkState;
    
    std::vector<RenderObject> _renderables;
    
    void send_data(RenderObject* first, uint32_t count);
    size_t pad_uniform_buffer_size(size_t originalSize);
  };
  
}
