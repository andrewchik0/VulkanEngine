#include "free_camera.h"
#include "engine.h"

namespace VKEngine {
  
  void FreeCamera::init(Engine* engine, uint32_t width, uint32_t height)
  {
    _time = &engine->time();
    resize(width, height);

    // engine->_windowHandler.hide_cursor();
  }
  
  void FreeCamera::handle()
  {/*
    static float t = 0.0f;
    const float animationTime = .2f;
    
    t += float(_time->_deltaTime) * (_keyboard->isPressing[SDL_SCANCODE_LCTRL] * 2 - 1);
    t = math::clamp(t, 0.0f, animationTime);
    set_fov(math::cos_interp(70, 77, t / animationTime));
    
    float deltaSpeed = float(_time->_deltaTime) * speed * (_keyboard->isPressing[SDL_SCANCODE_LCTRL] + 1);
    
    move_up((_keyboard->isPressing[SDL_SCANCODE_SPACE] - _keyboard->isPressing[SDL_SCANCODE_LSHIFT]) * deltaSpeed);
    move_forward((_keyboard->isPressing[SDL_SCANCODE_W] - _keyboard->isPressing[SDL_SCANCODE_S]) * deltaSpeed);
    move_right((_keyboard->isPressing[SDL_SCANCODE_D] - _keyboard->isPressing[SDL_SCANCODE_A]) * deltaSpeed);
    
    if (_mouse->isPressing[SDL_BUTTON_LEFT])
      rotate(_mouse->movedX / 250.0f * sensitivity, _mouse->movedY / 250.0f * sensitivity);
    */
    recalculate();
  }
  
}
