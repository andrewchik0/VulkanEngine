#include "input.h"
#include "engine/engine.h"

namespace VKEngine {
  bool is_pressed(Key key)
  {
    return Engine::get()->window()._input._isPressed[key];
  }

  bool is_pressing(Key key)
  {
    return Engine::get()->window()._input._isPressing[key];
  }

  math::UPoint mouse_pos()
  {
    return Engine::get()->window()._input._pos;
  }

  math::Point mouse_delta()
  {
    return Engine::get()->window()._input._moved;
  }

  int32_t mouse_wheel()
  {
    return Engine::get()->window()._input._scrolled.x;
  }
}