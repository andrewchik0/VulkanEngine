#pragma once

#include "engine/render/gui/gui_element.h"

namespace VKEngine {
  
  struct GUITextCreateInfo
  {
    Rectangle pos{-1, -1, -1, -1};
    std::string text;
  };
  
  class GUIText : public IGUIElement
  {
  public:
    GUIText(const GUITextCreateInfo& info);
    
    void render(VkCommandBuffer cmd) override;
  private:
    std::unique_ptr<RenderObject> _text;
  };
}
