#pragma once

#include "engine/render/resources/textures.h"
#include "engine/render/resources/mesh.h"

namespace VKEngine {
  
  class FontHandler
  {
  public:
    void load_font(const std::string& filename, Point size = { 0, 0 });
    void set_current_font(const std::string& filename, Point size = { 0, 0 });
    void free();
    
    RawTexture render_static_string(const std::string& text);
    Point get_kerning(uint32_t charA, uint32_t charB);
  private:
  };
}
