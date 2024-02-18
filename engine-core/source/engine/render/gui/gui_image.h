#pragma once

#include "engine/render/gui/gui_element.h"
#include "engine/render/resources/textures.h"
#include "engine/render/resources/mesh.h"
#include "engine/render/resources/materials.h"

namespace VKEngine {
  struct GUIImageCreateInfo
  {
    Rectangle pos{-1, -1, -1, -1};
    uint32_t width = 0, height = 0;
    std::string textureFilename;
    RawTexture* textureBuf;
  };
  
  class GUIImage : public IGUIElement
  {
  public:
    GUIImage(const GUIImageCreateInfo& info);
    
    void render(VkCommandBuffer cmd) override;
  private:
    std::unique_ptr<RenderObject> _object;
  };
}
