#pragma once

#include "pipeline.h"
#include "textures.h"

namespace VKEngine {
  
  struct Material : IResource
  {
    Texture* texture = nullptr;
    Pipeline* pipeline;
    
    void destroy(Render *render) override {}
  };
  
  class Materials : public ResourceManager<std::string, Material>
  {
  private:
  public:
    Material* create(const std::string& name, Pipeline* pipeline);
  };
  
}
