#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#include "../engine/types.h"

#define KEY_AMOUNT 256

namespace VKEngine {
  
  class Keyboard
  {
  private:
  public:
    bool isPressed[KEY_AMOUNT]{false};  // true only once per click
    bool isPressing[KEY_AMOUNT]{false}; // true every frame when button is down
    
    void handle(SDL_KeyboardEvent key)
    {
      if (key.keysym.scancode > KEY_AMOUNT - 1)
        return;
      isPressed[key.keysym.scancode] = isPressing[key.keysym.scancode] = key.type == SDL_KEYDOWN;
    }
    
    void reset()
    {
      zero_memory(isPressed, sizeof(bool) * KEY_AMOUNT);
    }
  };
  
}
