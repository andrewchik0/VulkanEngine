#pragma once

#include "engine/render/resources/resource_manager.h"
#include "engine/render/resources/vertex.h"

namespace VKEngine {

  struct Mesh : public IResource
  {
    Vertices vertices;
    AllocatedBuffer vertexBuffer;
    
    bool load_from_obj(const std::string& filename, bool bUseTexture = false);
    
    void destroy(Render *render) override;
  };
  
  
  class Meshes : public ResourceManager<std::string, Mesh>
  {
  public:
    Mesh* create(const std::string& name, const std::string& filename, bool bUseTexture = false);
    Mesh* create(const std::string &name, const Vertices& vertices);

  private:
    void upload_mesh(Mesh& mesh);
  };
}
