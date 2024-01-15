#pragma once

#include <functional>

#include "input.h"
#include "keycodes.h"
#include "../engine/types.h"

namespace VKEngine {

  class Window
  {
  public:
    Input _input;
    
    ~Window()
    {
      
    }

    virtual void init(std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction) = 0;
    virtual void run() = 0;
    virtual void cleanup() = 0;
    virtual void create_surface(VkInstance instance, VkSurfaceKHR* surface) = 0;
    virtual void hide_cursor() = 0;
    virtual void go_fullscreen() = 0;

    uint32_t& height() { return _windowExtent.height; }
    uint32_t& width() { return _windowExtent.width; }
    VkExtent2D ext() { return _windowExtent; }

    static std::unique_ptr<Window> create_window();
    static void delete_window(std::unique_ptr<Window>& window);

  protected:
    std::function<void()> _mainloopFunction;
    std::function<void()> _recreateFunction;

    bool _bIsCursorHided = false;
    bool _bIsFullscreen = false;

    VkExtent2D _windowExtent{1700, 900};
  };

}
