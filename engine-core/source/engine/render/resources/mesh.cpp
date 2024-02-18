#include <iostream>

#include <tiny_obj_loader/tiny_obj_loader.h>

#include "engine/render/resources/mesh.h"
#include "engine/engine.h"

namespace VKEngine {
  
  bool Mesh::load_from_obj(const std::string& filename, bool bUseTexture /*= false*/)
  {
    tinyobj::attrib_t attrib;
    std::vector<tinyobj::shape_t> shapes;
    std::vector<tinyobj::material_t> materials;
    
    std::string warn;
    std::string err;
    
    tinyobj::LoadObj(&attrib, &shapes, &materials, &warn, &err, filename.c_str(), nullptr);
    
    if (!err.empty())
    {
      std::cerr << err << std::endl;
      return false;
    }
    
    for (size_t s = 0; s < shapes.size(); s++)
    {
      size_t index_offset = 0;
      for (size_t f = 0; f < shapes[s].mesh.num_face_vertices.size(); f++)
      {
        for (size_t v = 0; v < 3; v++) {
          tinyobj::index_t idx = shapes[s].mesh.indices[index_offset + v];
          
          tinyobj::real_t vx = attrib.vertices[3 * idx.vertex_index + 0];
          tinyobj::real_t vy = attrib.vertices[3 * idx.vertex_index + 1];
          tinyobj::real_t vz = attrib.vertices[3 * idx.vertex_index + 2];
       
          tinyobj::real_t nx = attrib.normals[3 * idx.normal_index + 0];
          tinyobj::real_t ny = attrib.normals[3 * idx.normal_index + 1];
          tinyobj::real_t nz = attrib.normals[3 * idx.normal_index + 2];
          
          tinyobj::real_t ux = attrib.texcoords[2 * idx.texcoord_index + 0];
          tinyobj::real_t uy = attrib.texcoords[2 * idx.texcoord_index + 1];
          
          tinyobj::real_t r = attrib.colors[3 * idx.vertex_index + 0];
          tinyobj::real_t g = attrib.colors[3 * idx.vertex_index + 1];
          tinyobj::real_t b = attrib.colors[3 * idx.vertex_index + 2];
          
          vertices.positions3f.push_back({vx, vy, vz});
          vertices.normals.push_back({nx, ny, nz});
          
          bUseTexture ?
            vertices.texuvs.push_back({ux, 1 - uy}) :
            vertices.colors.push_back({r, g, b});
        }
        index_offset += 3;
      }
    }
    return true;
  }
  
  void Mesh::destroy(Render *render)
  {
    vmaDestroyBuffer(render->get_vk_state().allocator, vertexBuffer.buffer, vertexBuffer.allocation);
  }

  void Meshes::upload_mesh(Mesh& mesh)
  {
    RawBuffer packedVertices = mesh.vertices.pack();
    VkBufferCreateInfo stagingBufferInfo = {};
    stagingBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    stagingBufferInfo.pNext = nullptr;
    stagingBufferInfo.size = packedVertices.size;
    stagingBufferInfo.usage = VK_BUFFER_USAGE_TRANSFER_SRC_BIT;
    
    VmaAllocationCreateInfo vmaallocInfo = {};
    vmaallocInfo.usage = VMA_MEMORY_USAGE_CPU_ONLY;
    
    AllocatedBuffer stagingBuffer;
    
    VK_CHECK(vmaCreateBuffer(_render->get_vk_state().allocator, &stagingBufferInfo, &vmaallocInfo,
                             &stagingBuffer.buffer,
                             &stagingBuffer.allocation,
                             nullptr));
    
    stagingBuffer.map(packedVertices);
    
    VkBufferCreateInfo vertexBufferInfo = {};
    vertexBufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    vertexBufferInfo.pNext = nullptr;
    vertexBufferInfo.size = packedVertices.size;
    vertexBufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT | VK_BUFFER_USAGE_TRANSFER_DST_BIT;
    
    vmaallocInfo.usage = VMA_MEMORY_USAGE_GPU_ONLY;
    
    VK_CHECK(vmaCreateBuffer(_render->get_vk_state().allocator, &vertexBufferInfo, &vmaallocInfo,
                             &mesh.vertexBuffer.buffer,
                             &mesh.vertexBuffer.allocation,
                             nullptr));
    
    _render->_uploadContext.immediate_submit([=, &packedVertices](VkCommandBuffer cmd) {
      VkBufferCopy copy;
      copy.dstOffset = 0;
      copy.srcOffset = 0;
      copy.size = packedVertices.size;
      vkCmdCopyBuffer(cmd, stagingBuffer.buffer, mesh.vertexBuffer.buffer, 1, &copy);
    });
    
    vmaDestroyBuffer(_render->get_vk_state().allocator, stagingBuffer.buffer, stagingBuffer.allocation);
  }
  
  Mesh* Meshes::create(const std::string &name, const std::string &filename, bool bUseTexture /*= false*/)
  {
    Mesh mesh;
    auto future = Engine::get()->thread_pool().enqueue([&mesh, &filename, bUseTexture]() {
      return mesh.load_from_obj(filename, bUseTexture);
    });
    future.wait();
    if (!future.get())
      return nullptr;
    upload_mesh(mesh);
    return &(_resources[name] = mesh);
  }
  
  Mesh* Meshes::create(const std::string &name, const Vertices& vertices)
  {
    Mesh mesh;
    mesh.vertices = vertices;
    upload_mesh(mesh);
    return &(_resources[name] = mesh);
  }
}
