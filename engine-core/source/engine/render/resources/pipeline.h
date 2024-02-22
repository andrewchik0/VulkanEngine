#pragma once

#include <shaderc/shaderc.hpp>

#include "engine/render/resources/resource_manager.h"
#include "engine/render/resources/vertex.h"
#include "engine/render/gpu_data.h"

#define SHADER_VERTEX shaderc_glsl_vertex_shader
#define SHADER_FRAGMENT shaderc_glsl_fragment_shader

namespace VKEngine {
  
  typedef shaderc_shader_kind ShaderKind;
  
  struct ShaderInfo
  {
    std::string filename;
    ShaderKind kind;
    VertexInputDescription description;
    
    std::string get_full_filename() const
    {
      std::filesystem::path path = std::filesystem::current_path() / "shaders";
      std::string ext;

      switch (kind) {
        case SHADER_VERTEX:
          ext = ".vert";
          break;
        case SHADER_FRAGMENT:
          ext = ".frag";
          break;
        default:
          ext = "";
          break;
      }
      if (std::filesystem::exists(path / (filename + ext)))
        return filename + ext;

      path = std::filesystem::path(SHADERS_FOLDER_DEV);
      if (std::filesystem::exists(path /= (filename + ext)))
        return path.string();
      return std::string();
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
    std::vector<ShaderInfo> shaderInfos;
    uint32_t pushConstantSize = 0;
    
    Pipeline() {}
    Pipeline(VkPipeline pip, VkPipelineLayout pipelineLayout, const std::vector<ShaderInfo>& shaderData) :
      pipeline(pip),
      layout(pipelineLayout),
      shaderInfos(shaderData)
    {}
    
    void bind(VkCommandBuffer cmd)
      { vkCmdBindPipeline(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, pipeline); }
    
    template<typename T>
    void push_constant(VkCommandBuffer cmd, T data)
      { vkCmdPushConstants(cmd, layout, VK_SHADER_STAGE_ALL_GRAPHICS, 0, pushConstantSize, &data); }
                       
    void bind_descriptor(VkCommandBuffer cmd, uint32_t set, VkDescriptorSet descriptorSet)
      { vkCmdBindDescriptorSets(cmd, VK_PIPELINE_BIND_POINT_GRAPHICS, layout, set, 1, &descriptorSet, 0, nullptr); }
    
    void destroy(Render *render) override;
  };
  
  class Pipelines : public ResourceManager<std::string, Pipeline>
  {
  public:
    void reload();
    void reload(const std::string& name);
    void destroy();
    
    Pipeline* create(const std::string& name, const std::vector<ShaderInfo>& shaderInfo, uint32_t pushConstantSize);
    Pipeline* create(const std::string& name, const std::string& filename, const VertexInputDescription& description, uint32_t pushConstantSize);

  private:
    std::unordered_map<std::string, VkShaderModule> _loadedShaders;
    
    shaderc::Compiler _compiler;
    
    bool load_shader_module(const ShaderInfo& shaderInfo, VkShaderModule* outShaderModule);
    VkShaderModule* get_shader_module(const ShaderInfo& shaderInfo, bool useCache);
    Pipeline* push(const std::string& name, VkPipeline pipeline, VkPipelineLayout layout, const std::vector<ShaderInfo>& shaderInfos);
    void clean_loaded_shaders();
    void clean_loaded_shader(const std::string& name);
    
    bool build(const std::vector<ShaderInfo>& shaderInfos, uint32_t pushConstantSize, Pipeline& pipeline, bool bUseCache = true);

    std::vector<ShaderInfo> filename_to_shaderdata(const std::string& filename, const VertexInputDescription& description);
  };
  
}
