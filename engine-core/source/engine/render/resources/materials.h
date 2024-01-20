#pragma once

#include "engine/render/resources/pipeline.h"
#include "engine/render/resources/textures.h"

namespace VKEngine {
  
  struct Material : IResource
  {
    Texture* texture = nullptr;
    Pipeline* pipeline;
    
    void destroy(Render *render) override {}
  };
  
  class Materials : public ResourceManager<std::string, Material>
  {
  public:
    Material* create(const std::string& name, Pipeline* pipeline);

  private:
  };
  
}
