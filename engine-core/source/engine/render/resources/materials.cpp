#include "engine/render/resources/materials.h"

namespace VKEngine {
  
  Material* Materials::create(const std::string& name, Pipeline* pipeline)
  {
    Material mat;
    mat.pipeline = pipeline;
    
    _resources[name] = mat;
    return &_resources[name];
  }
  
}
