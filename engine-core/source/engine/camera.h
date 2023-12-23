#pragma once

#include "types.h"

namespace VKEngine {
  
  class Camera
  {
  protected:
    glm::mat4 _view, _proj, _viewProj;
    glm::vec3 _pos, _lookAt, _up;
    uint32_t _width, _height;
    float _fov, _far, _near;
    
    void recalculate()
    {
      _view = glm::lookAt(_pos, _pos + _lookAt, _up);
      _proj = glm::perspective(glm::radians(_fov), (float)_width / (float)_height, _near, _far);
      _viewProj = _proj * _view;
    }
  public:
    Camera() :
    _pos(0, 0, 0),
    _lookAt(1, 0, 0),
    _up(0, -1.0, 0),
    _width(16), _height(9),
    _fov(80.0), _near(0.1), _far(1000.0)
    {
      recalculate();
    }
    
    void resize(uint32_t width, uint32_t height)
    {
      _width = width;
      _height = height;
      recalculate();
    }
    
    glm::vec3 pos() { return _pos; }
    glm::vec3 lookAt() { return _lookAt; }
    glm::vec3 up() { return _up; }
    glm::mat4 view() { return _view; }
    glm::mat4 proj() { return _proj; }
    glm::mat4 viewProj() { return _viewProj; }
    
    void set_fov(float fov) { _fov = fov; }
    
    void move_back(float deltaSpeed) { move_forward(-deltaSpeed); }
    void move_forward(float deltaSpeed)
    {
      _pos += glm::normalize(math::getNormalizedProjection(_lookAt)) * deltaSpeed;
    }
    
    void move_left(float deltaSpeed) { move_right(-deltaSpeed); }
    void move_right(float deltaSpeed)
    {
      _pos += glm::normalize(glm::cross(glm::vec3(0, 1, 0), _lookAt)) * deltaSpeed;
    }
    
    void move_down(float deltaSpeed) { move_up(-deltaSpeed); }
    void move_up(float deltaSpeed)
    {
      _pos.y += deltaSpeed;
    }
    
    void rotate(float pitch, float yaw)
    {
      glm::mat4 rotateX = glm::rotate(glm::mat4(1.0), pitch, glm::vec3(0, 1, 0));
      glm::mat4 rotateY = glm::rotate(glm::mat4(1.0), yaw, glm::cross(glm::vec3(0, 1, 0), _lookAt));
      
      glm::vec3 checkLookAt = glm::normalize(rotateY * glm::vec4(_lookAt, 1.0));
      float angleCos = (glm::dot(checkLookAt, math::getNormalizedProjection(_lookAt)));
      float threeshold = 1e-4;
      
      if (angleCos <= 0)
      {
        int8_t sign = math::sign(_lookAt.y);
        
        _lookAt = math::getNormalizedProjection(_lookAt);
        _lookAt = rotateX * glm::vec4(_lookAt.x * threeshold, sign * (1 - threeshold), _lookAt.z * threeshold, 1);
      }
      else
        _lookAt = rotateY * rotateX * glm::vec4(_lookAt, 1.0);
    }
    
    virtual void handle() = 0;
  };
  
}
