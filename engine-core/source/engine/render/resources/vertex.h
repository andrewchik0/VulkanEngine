#pragma once

#include "engine/types.h"

namespace VKEngine {
  
  struct VertexInputDescription
  {
    std::vector<VkVertexInputBindingDescription> bindings;
    std::vector<VkVertexInputAttributeDescription> attributes;
    
    VkPipelineVertexInputStateCreateFlags flags = 0;
  };
  
  class VertexInputDescriptionBuilder
  {
  private:
    struct VertexInput
    {
      enum Type
      {
        Uint   = 0,
        Int    = 1,
        Float  = 2,
      };
      
      Type type;
      uint8_t size;
      
      VertexInput(Type vType, uint8_t count)
      {
        type = vType;
        switch (type)
        {
        case Float:
          size = sizeof(float);
          break;
        case Int:
          size = sizeof(int32_t);
          break;
        case Uint:
          size = sizeof(uint32_t);
          break;
        default:
          break;
        }
        size *= count;
      }
      
      VkFormat get_format()
      {
        constexpr uint8_t baseSize = sizeof(float);
        
        VkFormat format;
        switch (size)
        {
        case baseSize:
          format = VK_FORMAT_R32_UINT;
          break;
        case baseSize * 2:
          format = VK_FORMAT_R32G32_UINT;
          break;
        case baseSize * 3:
          format = VK_FORMAT_R32G32B32_UINT;
          break;
        case baseSize * 4:
          format = VK_FORMAT_R32G32B32A32_UINT;
          break;
        default:
          return VK_FORMAT_UNDEFINED;
        }
        format = (VkFormat)(format + type);
        return format;
      }
    };
    
  public:
    VertexInputDescriptionBuilder float1()
    {
      _inputs.push_back({VertexInput::Float, 1});
      return *this;
    }
    
    VertexInputDescriptionBuilder float2()
    {
      _inputs.push_back({VertexInput::Float, 2});
      return *this;
    }
    
    VertexInputDescriptionBuilder float3()
    {
      _inputs.push_back({VertexInput::Float, 3});
      return *this;
    } 
    
    VertexInputDescriptionBuilder float4()
    {
      _inputs.push_back({VertexInput::Float, 4});
      return *this;
    }
    
    VertexInputDescription build()
    {
      uint32_t size = 0;
      VertexInputDescription description;
      
      for (uint8_t i = 0; i < _inputs.size(); i++)
        size += _inputs[i].size;
      
      VkVertexInputBindingDescription mainBinding = {};
      mainBinding.binding = 0;
      mainBinding.stride = size;
      mainBinding.inputRate = VK_VERTEX_INPUT_RATE_VERTEX;
      
      description.bindings.push_back(mainBinding);
      
      uint32_t offset = 0;
      for (uint8_t i = 0; i < _inputs.size(); i++)
      {
        VkVertexInputAttributeDescription attribute = {};
        attribute.binding = 0;
        attribute.location = i;
        attribute.format = _inputs[i].get_format();
        attribute.offset = offset;
        
        offset += _inputs[i].size;
        description.attributes.push_back(attribute);
      }
      return description;
    }
    
  private:
    std::vector<VertexInput> _inputs;
  };
  
  struct Vertices
  {
    std::vector<glm::vec3> positions;
    std::vector<glm::vec3> normals;
    std::vector<glm::vec2> texuvs;
    std::vector<glm::vec3> colors;
    
    uint32_t size()
    {
      return (uint32_t)positions.size();
    }
    
    VertexInputDescription build_vertex_desc()
    {
      VertexInputDescriptionBuilder builder;
      
      if (!positions.empty())
        builder.float3();
      if (!normals.empty())
        builder.float3();
      if (!texuvs.empty())
        builder.float2();
      if (!colors.empty())
        builder.float3();
      
      return builder.build();
    }
    
    RawBuffer pack()
    {
      RawBuffer packed;
      
      if ((normals.size() != 0 && normals.size() != positions.size()) ||
           (texuvs.size() != 0 &&  texuvs.size() != positions.size()) ||
           (colors.size() != 0 &&  colors.size() != positions.size()))
        return packed;
      
      uint32_t vertexSize = 3 + !normals.empty() * 3 + !texuvs.empty() * 2 + !colors.empty() * 3;
      size_t size = positions.size() * vertexSize;
      packed.data = new float[size * sizeof(float)];
      packed.size = size * sizeof(float);
      
      float* ptr = (float*)packed.data;
      for (size_t i = 0; i < size; i += vertexSize)
      {
        uint32_t offset = 0;
        
        ptr[i + 0] = positions[i / vertexSize].x;
        ptr[i + 1] = positions[i / vertexSize].y;
        ptr[i + 2] = positions[i / vertexSize].z;
        offset += 3;
        
        if (!normals.empty())
        {
          ptr[i + offset + 0] = normals[i / vertexSize].x;
          ptr[i + offset + 1] = normals[i / vertexSize].y;
          ptr[i + offset + 2] = normals[i / vertexSize].z;
          offset += 3;
        }
        if (!texuvs.empty())
        {
          ptr[i + offset + 0] = texuvs[i / vertexSize].x;
          ptr[i + offset + 1] = texuvs[i / vertexSize].y;
          offset += 2;
        }
        if (!colors.empty())
        {
          ptr[i + offset + 0] = colors[i / vertexSize].x;
          ptr[i + offset + 1] = colors[i / vertexSize].y;
          ptr[i + offset + 2] = colors[i / vertexSize].z;
          offset += 3;
        }
      }
      return packed;
    }
  };
  
}
