#pragma once

#include <shaderc/shaderc.hpp>

#include "resource_manager.h"
#include "../gpu_data.h"

#define SHADER_VERTEX shaderc_glsl_vertex_shader
#define SHADER_FRAGMENT shaderc_glsl_fragment_shader

namespace VKEngine {
  
  typedef shaderc_shader_kind ShaderKind;
  
  struct ShaderData
  {
    std::string filename;
    ShaderKind kind;
    
    std::string get_full_filename() const
    {
      switch (kind) {
        case SHADER_VERTEX:
          return "shaders/" + filename + ".vert";
        case SHADER_FRAGMENT:
          return "shaders/" + filename + ".frag";
        default:
          return "";
      }
    }
  };
  
  class PipelineBuilder
  {
  public:
    
    std::vector<VkPipelineShaderStageCreateInfo> _shaderStages;
    VkPipelineVertexInputStateCreateInfo _vertexInputInfo;
    VkPipelineInputAssemblyStateCreateInfo _inputAssembly;
    VkViewport _viewport;
    VkRect2D _scissor;
    VkPipelineRasterizationStateCreateInfo _rasterizer;
    VkPipelineColorBlendAttachmentState _colorBlendAttachment;
    VkPipelineMultisampleStateCreateInfo _multisampling;
    VkPipelineLayout _pipelineLayout;
    VkPipelineDepthStencilStateCreateInfo _depthStencil;
    
    VkPipeline build(VkDevice& device, VkRenderPass& pass);
  };
  
  struct Pipeline : public IResource
  {
    VkPipeline pipeline{ VK_NULL_HANDLE };
    VkPipelineLayout layout{ VK_NULL_HANDLE };
    std::vector<ShaderData> shaderData;
    
    Pipeline() {}
    Pipeline(VkPipeline pip, VkPipelineLayout pipelineLayout, const std::vector<ShaderData>& shaderData) :
      pipeline(pip),
      layout(pipelineLayout),
      shaderData(shaderData)
    {}
    
    void bind(VkCommandBuffer cmd) { vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline); }
    void bind_descriptor(VkCommandBuffer cmd, uint32_t set, VkDescriptorSet descriptorSet)
    {
      vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, set, 1, &descriptorSet, 0, nullptr);
    }
    
    void destroy(Render *render) override;
  };
  
  class Pipelines : public ResourceManager<std::string, Pipeline>
  {
  private:
    std::unordered_map<std::string, VkShaderModule> _loadedShaders;
    
    shaderc::Compiler _compiler;
    
    bool load_shader_module(const ShaderData& shaderData, VkShaderModule* outShaderModule);
    VkShaderModule* get_shader_module(const ShaderData& shaderData, bool useCache);
    Pipeline* push(const std::string& name, VkPipeline pipeline, VkPipelineLayout layout, const std::vector<ShaderData>& shaderData);
    void clean_loaded_shaders();
    void clean_loaded_shader(const std::string& name);
    
    bool build(const std::vector<ShaderData>& shaderData, Pipeline& pipeline, bool bUseCache = true);

    std::vector<ShaderData> filename_to_shaderdata(const std::string& filename);
  public:
    void reload();
    void reload(const std::string& name);
    void destroy();
    
    Pipeline* create(const std::string& name, const std::vector<ShaderData>& shaderData);
    Pipeline* create(const std::string& name, const std::string& filename);
  };
  
}
