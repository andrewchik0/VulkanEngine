#pragma once

#include <functional>

#include "input.h"
#include "../engine/types.h"

namespace VKEngine {

  class WindowHandler
  {
  private:
    std::function<void()> _mainloopFunction;
    std::function<void()> _recreateFunction;
    
    bool _isCursorHided = false;
    
    VkExtent2D _windowExtent{1700, 900};
  public:
    struct SDL_Window* _window{ nullptr };
    Keyboard _keyboard;
    Mouse _mouse;
    
    void init(std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction);
    void run();
    void cleanup();
    void create_surface(VkInstance instance, VkSurfaceKHR* surface);
    uint32_t& height();
    uint32_t& width();
    VkExtent2D ext() { return _windowExtent; }
    void hide_cursor();
  };
  
}
