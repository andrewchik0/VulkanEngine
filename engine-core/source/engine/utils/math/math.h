#pragma once

#include <glm/glm.hpp>

#include "engine/utils/math/spherical_vec.h"

namespace VKEngine::math {
  
  struct Point
  {
    int32_t x = 0;
    int32_t y = 0;
  };

  struct UPoint
  {
    uint32_t x = 0;
    uint32_t y = 0;
  };

  inline glm::vec3 getNormalizedProjection(glm::vec3 v)
  {
    return glm::normalize(glm::vec3(v.x, 0.0, v.z));
  }
  
  inline float max(float a, float b)
  {
    return a > b ? a : b;
  }
  
  inline float min(float a, float b)
  {
    return a > b ? b : a;
  }
  
  inline float clamp(float v, float min, float max)
  {
    return v < min ?
    min :
    v > max ?
    max :
    v;
  }
  
  inline int32_t sign(float x)
  {
    return x > 0 ? 1 : -1;
  }
  
  inline float lerp(float a, float b, float t)
  {
    return a * (1 - t) + b * t;
  }
  
  inline float cos_interp(float a, float b, float t)
  {
    t = 1 - t;
    t = cos(t * PI) / 2.0f + 1;
    return a * (1 - t) + b * t;
  }
  
}
