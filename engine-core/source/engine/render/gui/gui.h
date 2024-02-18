#pragma once

#include "engine/render/gui/gui_image.h"
#include "engine/render/gui/gui_text.h"
#include "engine/render/resources/pipeline.h"

namespace VKEngine {
  
  class GUI
  {
  public:
    void init();
    
    void render(VkCommandBuffer cmd)
    {
      _imagesPipeline->bind(cmd);
      for (auto& image : _images)
        image.render(cmd);
      _textsPipeline->bind(cmd);
      for (auto& text : _texts)
        text.render(cmd);
    }
    
    void image(const GUIImageCreateInfo& info) { _images.push_back(GUIImage(info)); }
    void text(const GUITextCreateInfo& info) { _texts.push_back(GUIText(info)); }
  private:
    std::vector<GUIImage> _images;
    std::vector<GUIText> _texts;
    Pipeline* _imagesPipeline;
    Pipeline* _textsPipeline;
  };
}
