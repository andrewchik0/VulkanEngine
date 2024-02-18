#pragma once

#include "engine/types.h"

namespace VKEngine {
  
  constexpr uint32_t SCENE_DATA_BINDING = 0;
  struct GPUSceneData
  {
    glm::vec4 fogColorExponent;
    glm::vec4 fogDistancesMinMax;
    glm::vec4 ambientColor;
    glm::vec4 sunlightDirectionPower;
    glm::vec4 sunlightColor;
  };
  
  constexpr uint32_t CAMERA_DATA_BINDING = 0;
  struct GPUCameraData
  {
    glm::mat4 view;
    glm::mat4 proj;
    glm::mat4 viewproj;
    glm::vec4 pos;
    glm::vec4 lookAt;
    glm::vec4 up;
    glm::vec4 screenSize;
  };
  
  constexpr uint32_t OBJECT_DATA_BINDING = 0;
  struct GPUObjectData
  {
    glm::mat4 modelMatrix;
  };
  
}
