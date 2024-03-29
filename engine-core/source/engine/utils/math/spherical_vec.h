#pragma once

#include <cmath>

#include <glm/vec3.hpp>

namespace VKEngine::math {
  
  constexpr float PI = 3.14159265359f;
  
  class sph_vec3
  {
  public:
    float theta, phi, radius;
    
    sph_vec3() : theta(0.0f), phi(0.0f), radius(0.0f) {}
    sph_vec3(glm::vec3 v)
    {
      radius = glm::length(v);
      theta = acos(v.z / radius);
      if (v.x == 0)
      {
        phi = (PI / 2.0f) * ((v.y > 0) * 2 - 1);
        return;
      }
      phi = v.x > 0 ? atan(v.y / v.x) :
        v.y >= 0 ? atan(v.y / v.x) + PI : atan(v.y / v.x) - PI;
    }
    
    glm::vec3 to_vec3()
    {
      return radius * glm::vec3(
        sin(theta) * cos(phi),
        sin(theta) * sin(phi),
        cos(theta));
    }

  private:

  };
  
}
