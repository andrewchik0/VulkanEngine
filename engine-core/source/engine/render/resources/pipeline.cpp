#include <fstream>
#include <sstream>

#include "engine/render/resources/pipeline.h"
#include "engine/engine.h"

namespace VKEngine {
  
  VkPipeline PipelineBuilder::build(VkDevice& device, VkRenderPass& pass)
  {
    VkPipelineViewportStateCreateInfo viewportState = {};
    viewportState.sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO;
    viewportState.pNext = nullptr;
    viewportState.viewportCount = 1;
    viewportState.pViewports = &_viewport;
    viewportState.scissorCount = 1;
    viewportState.pScissors = &_scissor;
    
    VkPipelineColorBlendStateCreateInfo colorBlending = {};
    colorBlending.sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO;
    colorBlending.pNext = nullptr;
    colorBlending.logicOpEnable = VK_FALSE;
    colorBlending.logicOp = VK_LOGIC_OP_COPY;
    colorBlending.attachmentCount = 1;
    colorBlending.pAttachments = &_colorBlendAttachment;
    
    VkGraphicsPipelineCreateInfo pipelineInfo = {};
    pipelineInfo.sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO;
    pipelineInfo.pNext = nullptr;
    pipelineInfo.stageCount = (uint32_t)_shaderStages.size();
    pipelineInfo.pStages = _shaderStages.data();
    pipelineInfo.pVertexInputState = &_vertexInputInfo;
    pipelineInfo.pInputAssemblyState = &_inputAssembly;
    pipelineInfo.pViewportState = &viewportState;
    pipelineInfo.pRasterizationState = &_rasterizer;
    pipelineInfo.pMultisampleState = &_multisampling;
    pipelineInfo.pColorBlendState = &colorBlending;
    pipelineInfo.pDepthStencilState = &_depthStencil;
    pipelineInfo.layout = _pipelineLayout;
    pipelineInfo.renderPass = pass;
    pipelineInfo.subpass = 0;
    pipelineInfo.basePipelineHandle = VK_NULL_HANDLE;
    
    VkPipeline newPipeline;
    if (vkCreateGraphicsPipelines(device, VK_NULL_HANDLE, 1, &pipelineInfo, nullptr, &newPipeline) != VK_SUCCESS)
    {
      std::cout << "failed to create pipline\n";
      return VK_NULL_HANDLE;
    }
    
    return newPipeline;
  }
  
  void Pipeline::destroy(Render *render)
  {
    vkDestroyPipeline(render->get_vk_state().device, pipeline, nullptr);
    vkDestroyPipelineLayout(render->get_vk_state().device, layout, nullptr);
  }
  
  Pipeline* Pipelines::push(const std::string& name, VkPipeline pipeline, VkPipelineLayout layout, const std::vector<ShaderInfo>& shaderData)
  {
    return &(_resources[name] = Pipeline{pipeline, layout, shaderData});
  }
  
  bool Pipelines::load_shader_module(const ShaderInfo& shaderData, VkShaderModule* outShaderModule)
  {
    std::ifstream file(shaderData.get_full_filename(), std::ios::in);
    
    if (!file.is_open())
      return false;
    
    std::stringstream stream;
    stream << file.rdbuf();
    file.close();
    
    auto shader = _compiler.CompileGlslToSpv(stream.str(), shaderData.kind, shaderData.filename.c_str());
    
    if (shader.GetNumErrors())
    {
      std::cout << shader.GetErrorMessage();
      return false;
    }
    
    VkShaderModuleCreateInfo createInfo = {};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO;
    createInfo.pNext = nullptr;
    
    createInfo.codeSize = (shader.end() - shader.begin()) * sizeof(uint32_t);
    createInfo.pCode = &*shader.begin();
    
    VkShaderModule shaderModule;
    if (vkCreateShaderModule(_render->get_vk_state().device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
      return false;
    }
    *outShaderModule = shaderModule;
    return true;
  }
  
  VkShaderModule* Pipelines::get_shader_module(const ShaderInfo& shaderData, bool bUseCache)
  {
    if (_loadedShaders.find(shaderData.get_full_filename()) != _loadedShaders.end() && bUseCache)
      return &_loadedShaders[shaderData.get_full_filename()];
    clean_loaded_shader(shaderData.get_full_filename());
    
    VkShaderModule newModule;
    if (load_shader_module(shaderData, &newModule))
      return &(_loadedShaders[shaderData.get_full_filename()] = newModule);
    
    std::cout << "Couldn't load a shader: " << shaderData.filename << '\n';
    return nullptr;
  }
  
  bool Pipelines::build(const std::vector<ShaderInfo>& shaderInfos, uint32_t pushConstantSize, Pipeline& pipeline, bool bUseCache /*= true*/)
  {
    PipelineBuilder pipelineBuilder;
    VertexInputDescription vertexDescription;
    
    for (auto &shaderInfo : shaderInfos)
    {
      auto shader = get_shader_module(shaderInfo, bUseCache);
      if (shader == nullptr)
        return false;
      
      switch (shaderInfo.kind)
      {
        case SHADER_VERTEX:
          vertexDescription = shaderInfo.description;
          pipelineBuilder._shaderStages.push_back(VKInit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_VERTEX_BIT, *shader));
          break;
        case SHADER_FRAGMENT:
          pipelineBuilder._shaderStages.push_back(VKInit::pipeline_shader_stage_create_info(VK_SHADER_STAGE_FRAGMENT_BIT, *shader));
          break;
        default:
          break;
      }
    }
    
    VkPipelineLayoutCreateInfo pipeline_layout_info = VKInit::pipeline_layout_create_info();
    
    VkDescriptorSetLayout setLayouts[] =
    {
      _render->_descriptors._globalLayout,
      _render->_descriptors._cameraLayout,
      _render->_descriptors._objectLayout,
      _render->_descriptors._textureLayout,
    };
    
    pipeline_layout_info.setLayoutCount = 4;
    pipeline_layout_info.pSetLayouts = setLayouts;
    
    VkPushConstantRange pushConstantRange;
    pushConstantRange.offset = 0;
    pushConstantRange.size = pushConstantSize;
    pushConstantRange.stageFlags = VK_SHADER_STAGE_ALL_GRAPHICS;
    
    if (pushConstantSize > 0)
    {
      pipeline_layout_info.pPushConstantRanges = &pushConstantRange;
      pipeline_layout_info.pushConstantRangeCount = 1;
    }
    
    VkPipelineLayout pipLayout;
    VK_CHECK(vkCreatePipelineLayout(_render->get_vk_state().device, &pipeline_layout_info, nullptr, &pipLayout));
    
    pipelineBuilder._pipelineLayout = pipLayout;
    pipelineBuilder._vertexInputInfo = VKInit::vertex_input_state_create_info();
    pipelineBuilder._inputAssembly = VKInit::input_assembly_create_info(VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST);
    pipelineBuilder._viewport.x = 0.0f;
    pipelineBuilder._viewport.y = 0.0f;
    pipelineBuilder._viewport.width = (float)_render->width();
    pipelineBuilder._viewport.height = (float)_render->height();
    pipelineBuilder._viewport.minDepth = 0.0f;
    pipelineBuilder._viewport.maxDepth = 1.0f;
    pipelineBuilder._scissor.offset = { 0, 0 };
    pipelineBuilder._scissor.extent = { _render->width(), _render->height() };
    pipelineBuilder._rasterizer = VKInit::rasterization_state_create_info(VK_POLYGON_MODE_FILL);
    pipelineBuilder._multisampling = VKInit::multisampling_state_create_info();
    pipelineBuilder._colorBlendAttachment = VKInit::color_blend_attachment_state();
    pipelineBuilder._depthStencil = VKInit::depth_stencil_create_info(true, true, VK_COMPARE_OP_LESS_OR_EQUAL);
    
    pipelineBuilder._vertexInputInfo.pVertexAttributeDescriptions = vertexDescription.attributes.data();
    pipelineBuilder._vertexInputInfo.vertexAttributeDescriptionCount = (uint32_t)vertexDescription.attributes.size();
    pipelineBuilder._vertexInputInfo.pVertexBindingDescriptions = vertexDescription.bindings.data();
    pipelineBuilder._vertexInputInfo.vertexBindingDescriptionCount = (uint32_t)vertexDescription.bindings.size();
    
    VkPipeline vkPipeline = pipelineBuilder.build(_render->get_vk_state().device, _render->_renderPasses._default);
    pipeline.pipeline = vkPipeline;
    pipeline.layout = pipLayout;
    pipeline.shaderInfos = shaderInfos;
    pipeline.pushConstantSize = pushConstantSize;
    
    return true;
  }

  std::vector<ShaderInfo> Pipelines::filename_to_shaderdata(const std::string& filename, const VertexInputDescription& description)
  {
    ShaderInfo shaderInfo{filename, SHADER_VERTEX, description};
    std::vector<ShaderInfo> result;

    if (std::filesystem::exists(shaderInfo.get_full_filename()))
      result.push_back(shaderInfo);
    else
      assert_msg("No such file: " + shaderInfo.get_full_filename());
    
    shaderInfo.kind = SHADER_FRAGMENT;
    if (std::filesystem::exists(shaderInfo.get_full_filename()))
      result.push_back(shaderInfo);
    else
      assert_msg("No such file: " + shaderInfo.get_full_filename());
    return result;
  }
  
  void Pipelines::clean_loaded_shaders()
  {
    for (auto& it : _loadedShaders)
    {
      vkDestroyShaderModule(_render->get_vk_state().device, it.second, nullptr);
    }
  }
  
  void Pipelines::clean_loaded_shader(const std::string& name)
  {
    if (_loadedShaders.find(name) != _loadedShaders.end())
      vkDestroyShaderModule(_render->get_vk_state().device, _loadedShaders[name], nullptr);
  }
  
  void Pipelines::reload()
  {
    struct PipInfo
    {
      std::vector<ShaderInfo> infos;
      uint32_t pushConstants;
    };
    std::unordered_map<std::string, PipInfo> pipelinesToReload;
    
    for (auto& it : _resources)
    {
      pipelinesToReload[it.first].infos = it.second.shaderInfos;
      pipelinesToReload[it.first].pushConstants = it.second.pushConstantSize;
    }
    
    ResourceManager::destroy();
    
    for (auto& it : pipelinesToReload)
    {
      Pipeline pipeline;
      if (build(it.second.infos, it.second.pushConstants, pipeline, false))
        _resources[it.first] = pipeline;
    }
  }
  
  void Pipelines::reload(const std::string& name)
  {
    auto pipeline = get(name);
    
    if (pipeline == nullptr)
      return;
    
    Pipeline newPipeline;
    if (build(pipeline->shaderInfos, 0, newPipeline, false))
    {
      ResourceManager::destroy(name);
      _resources[name] = newPipeline;
    }
  }
  
  void Pipelines::destroy()
  {
    ResourceManager::destroy();
    clean_loaded_shaders();
  }
 
  Pipeline* Pipelines::create(const std::string& name, const std::vector<ShaderInfo>& shaderInfos, uint32_t pushConstantSize)
  {
    if (shaderInfos.size() == 0)
      return nullptr;
    
    for (auto& info : shaderInfos)
    {
      if (!std::filesystem::exists(info.get_full_filename()))
      {
        assert_msg("Shader file: \"" + info.filename + "\" does not exist!");
        return nullptr;
      }
      
      add_file_watcher_debug_only(info.get_full_filename(), [=]{
        _render->get_vk_state().wait();
        reload(name);
      });
    }
    Pipeline pipeline;
    if (build(shaderInfos, pushConstantSize, pipeline))
      return &(_resources[name] = pipeline);
    return nullptr;
  }
  
  Pipeline* Pipelines::create(const std::string& name, const std::string& filename, const VertexInputDescription& description, uint32_t pushConstantSize)
  {
    return create(name, filename_to_shaderdata(filename, description), pushConstantSize);
  }
}
