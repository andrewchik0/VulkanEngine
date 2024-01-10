#pragma once

#include "resource_manager.h"
#include "vertex.h"

namespace VKEngine {
  
  struct Mesh : public IResource
  {
    std::vector<Vertex> vertices;
    AllocatedBuffer vertexBuffer;
    
    bool load_from_obj(const std::string& filename);
    
    void destroy(Render *render) override;
  };
  
  
  class Meshes : public ResourceManager<std::string, Mesh>
  {
  public:
    Mesh* create(const std::string& name, const std::string& filename);
    Mesh* create(const std::string& name, const std::vector<Vertex>& vertices);

  private:
    void upload_mesh(Mesh& mesh);
  };
}
