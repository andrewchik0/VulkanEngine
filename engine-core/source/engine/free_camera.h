#pragma once

#include "camera.h"
#include "../os_level/input.h"

namespace VKEngine {
  
  class FreeCamera : public Camera
  {
  private:
    Keyboard* _keyboard;
    Utils::TimeHandler* _time;
    Mouse* _mouse;
    
    float speed = 10.0;
    float sensitivity = 1;
  public:
    
    void init(Engine* engine, uint32_t width, uint32_t height);
    void init(uint32_t width, uint32_t height) { resize(width, height); }
    void handle() override;
  };
  
}
