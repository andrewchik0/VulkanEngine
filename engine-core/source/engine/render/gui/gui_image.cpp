#include "engine/render/gui/gui_image.h"

#include "engine/render/render.h"
#include "engine/engine.h"

namespace VKEngine {
  
  GUIImage::GUIImage(const GUIImageCreateInfo& info)
  {
    Rectangle rect = info.pos;
    
    if (info.width != 0 && info.height != 0)
      recalculate_position(
         &rect,
         info.width, info.height,
         Engine::get()->window().width(), Engine::get()->window().height()
       );
    
    _object = std::make_unique<RenderObject>();
    _object->mesh = Engine::get()->render()._meshes.create(random_string(16), rectangle(rect));
    _object->material = Engine::get()->render()._materials.create(random_string(16), Engine::get()->render()._pipelines["gui-image-pipeline"]);
    if (info.textureBuf->size > 0)
      _object->material->texture = Engine::get()->render()._textures.create_from_memory(random_string(16), *info.textureBuf, info.textureBuf->width, info.textureBuf->height, ImageType::RGBA);
    if (!info.textureFilename.empty())
      _object->material->texture = Engine::get()->render()._textures.create_from_file(random_string(16), info.textureFilename);
  }
  
  void GUIImage::render(VkCommandBuffer cmd)
  {
    _object->material->pipeline->push_constant(cmd, glm::vec4(_position, 0.0f, 0.0f));
    Engine::get()->render().draw_objects(cmd, _object.get(), 1);
  }
}
