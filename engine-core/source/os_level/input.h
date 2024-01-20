#pragma once

#include "os_level/keycodes.h"
#include "engine/utils/math/math.h"

#ifdef USE_SDL
  #define KEY_MASK(x) ((x) & ~(1 << 30))
#else
  #define KEY_MASK(x) (x)
#endif

constexpr uint32_t KEY_AMOUNT = 256;

namespace VKEngine {
  
  class Input
  {
  public:
    bool _isPressed[KEY_AMOUNT]{};  // true only once per click
    bool _isPressing[KEY_AMOUNT]{}; // true every frame when button is down

    // Mouse data
    math::Point _scrolled;
    math::Point _moved;
    math::UPoint _pos;
    
    void handle_keys(Key key, bool bIsPressed)
    {
      if (KEY_MASK(key) > KEY_AMOUNT - 1)
        return;
      _isPressed[KEY_MASK(key)] = _isPressing[KEY_MASK(key)] = bIsPressed;
    }
    
    void reset()
    {
      _scrolled.x = 0;
      _scrolled.y = 0;
      _moved.x = 0;
      _moved.y = 0;

      zero_memory(_isPressed, sizeof(bool) * KEY_AMOUNT);
    }

    void handle_mousemotion(uint32_t posx, uint32_t posy, int32_t xrel, int32_t yrel)
    {
      _moved.x = xrel;
      _moved.y = yrel;
      _pos.x = posx;
      _pos.y = posy;
    }

    void handle_mousewheel(int32_t x, int32_t y)
    {
      _scrolled.x = x;
      _scrolled.y = y;
    }
  };
  
  bool is_pressed(Key key);
  bool is_pressing(Key key);
  math::UPoint mouse_pos();
  math::Point mouse_delta();
  int32_t mouse_wheel();
}
