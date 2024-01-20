#include "engine/free_camera.h"
#include "engine/engine.h"

namespace VKEngine {
  
  void FreeCamera::init(Engine* engine, uint32_t width, uint32_t height)
  {
    _time = &engine->time();
    resize(width, height);

    // engine->_windowHandler.hide_cursor();
  }
  
  void FreeCamera::handle()
  {
    static float t = 0.0f;
    const float animationTime = .2f;
    
    t += float(_time->_deltaTime) * (is_pressing(KeyCodes::LeftControl) * 2 - 1);
    t = math::clamp(t, 0.0f, animationTime);
    set_fov(math::cos_interp(70, 77, t / animationTime));
    
    float deltaSpeed = float(_time->_deltaTime) * speed * (is_pressing(KeyCodes::LeftControl) + 1);
    
    move_up((is_pressing(KeyCodes::Space) - is_pressing(KeyCodes::LeftShift)) * deltaSpeed);
    move_forward((is_pressing(KeyCodes::W) - is_pressing(KeyCodes::S)) * deltaSpeed);
    move_right((is_pressing(KeyCodes::D) - is_pressing(KeyCodes::A)) * deltaSpeed);

    if (is_pressing(KeyCodes::MouseLeftButton))
      rotate(mouse_delta().x / 250.0f * sensitivity, mouse_delta().y / 250.0f * sensitivity);

    recalculate();
  }
  
}
