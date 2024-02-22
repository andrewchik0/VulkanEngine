#include "engine/render/gui/gui_text.h"

#include "engine/render/render.h"
#include "engine/engine.h"

namespace VKEngine {
  
  GUIText::GUIText(const GUITextCreateInfo& info)
  {
    RawTexture tex = Engine::get()->render()._fontHandler.render_static_string(info.text);
    Rectangle rect = info.pos;
    recalculate_position(
       &rect,
       tex.width, tex.height,
       Engine::get()->window().width(), Engine::get()->window().height()
     );
    rect.x1 += tex.width;
    rect.x2 += tex.width;
    
    _text = std::make_unique<RenderObject>();
    _text->mesh = Engine::get()->render()._meshes.create(random_string(16), rectangle(rect));
    _text->material = Engine::get()->render()._materials.create(random_string(16), Engine::get()->render()._pipelines["gui-text-pipeline"]);
    _text->material->texture = Engine::get()->render()._textures.create_from_memory(random_string(16), tex, ImageType::R);
  }
  
  void GUIText::render(VkCommandBuffer cmd)
  {
    _text->material->pipeline->push_constant(cmd, glm::vec4(_position, 0.0f, 0.0f));
    Engine::get()->render().draw_objects(cmd, _text.get(), 1);
  }
}
