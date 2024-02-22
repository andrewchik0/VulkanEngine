#include "engine/render/gui/gui.h"
#include "engine/engine.h"

namespace VKEngine {
  
  void GUI::init()
  {
    auto info = VertexInputDescriptionBuilder().float2().float2().build();
    
    std::vector<ShaderInfo> imageInfo =
    {
      { "gui/gui", SHADER_VERTEX, info },
      { "gui/gui-rgba", SHADER_FRAGMENT },
    };
    _imagesPipeline = Engine::get()->render()._pipelines.create("gui-image-pipeline", imageInfo, sizeof(glm::vec4));
    
    std::vector<ShaderInfo> textsInfo =
    {
      { "gui/gui", SHADER_VERTEX, info },
      { "gui/gui-font", SHADER_FRAGMENT },
    };
    _textsPipeline = Engine::get()->render()._pipelines.create("gui-text-pipeline", textsInfo, sizeof(glm::vec4));
  }
  
  void GUI::resize()
  {
    int32_t deltaWidth = Engine::get()->window().width() - Engine::get()->window().old_width();
    int32_t deltaHeight = Engine::get()->window().height() - Engine::get()->window().old_height();
    
    for (auto& image : _images)
      image.resize(deltaWidth, deltaHeight);
    for (auto& text : _texts)
      text.resize(deltaWidth, deltaHeight);
    
  }
}
