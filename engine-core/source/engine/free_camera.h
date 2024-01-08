#pragma once

#include "camera.h"
#include "../os_level/input.h"

namespace VKEngine {
  
  class FreeCamera : public Camera
  {
  public:

    void init(Engine* engine, uint32_t width, uint32_t height);
    void init(uint32_t width, uint32_t height) { resize(width, height); }
    void handle() override;

  private:

    Utils::TimeHandler* _time;
    float speed = 10.0;
    float sensitivity = 1;
  };
  
}
