#pragma once

#include <iostream>

#include <SDL2/SDL.h>

#define BUTTONS_AMOUNT 4

namespace VKEngine {
  
  class Mouse
  {
  private:
  public:
    bool isPressed[BUTTONS_AMOUNT]{false};  // true only once per click
    bool isPressing[BUTTONS_AMOUNT]{false}; // true every frame when button is down
    int32_t
      scrolledX = 0,
      scrolledY = 0,
      movedX = 0,
      movedY = 0,
      posX = 0,
      posY = 0;
    
    void handle(SDL_MouseMotionEvent motion)
    {
      movedX = motion.xrel;
      movedY = motion.yrel;
      posX = motion.x;
      posY = motion.y;
    }
    
    void handle(SDL_MouseButtonEvent button)
    {
      if (button.button >= BUTTONS_AMOUNT)
        return;
      isPressed[button.button] = isPressing[button.button] = button.state == SDL_PRESSED;
    }
    
    void handle(SDL_MouseWheelEvent wheel)
    {
      scrolledX = wheel.x;
      scrolledY = wheel.y;
    }
    
    void reset()
    {
      scrolledX = 0;
      scrolledY = 0;
      movedX = 0;
      movedY = 0;
      
      zero_memory(isPressed, sizeof(bool) * BUTTONS_AMOUNT);
    }
  };
  
}
