#include "engine/render/gui/gui.h"
#include "engine/engine.h"

namespace VKEngine {
  
  void GUI::init()
  {
    auto info = VertexInputDescriptionBuilder().float2().float2().build();
    _imagesPipeline = Engine::get()->render()._pipelines.create("gui-image-pipeline", "gui/image", info);
    _textsPipeline = Engine::get()->render()._pipelines.create("gui-text-pipeline", "gui/text", info);
  }
}
