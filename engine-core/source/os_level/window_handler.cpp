#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

#include "window_handler.h"

namespace VKEngine {
  
  void WindowHandler::init(std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction)
  {
    _mainloopFunction = mainloopFunction;
    _recreateFunction = recreateFunction;
    SDL_Init(SDL_INIT_VIDEO);
    
    SDL_WindowFlags window_flags = (SDL_WindowFlags)(SDL_WINDOW_VULKAN | SDL_WINDOW_RESIZABLE);
    
    _window = SDL_CreateWindow(
                               "Vulkan Engine",
                               SDL_WINDOWPOS_UNDEFINED,
                               SDL_WINDOWPOS_UNDEFINED,
                               _windowExtent.width,
                               _windowExtent.height,
                               window_flags
                               );
  }
  
  void WindowHandler::run()
  {
    SDL_Event e;
    bool bQuit = false;
    
    while (!bQuit)
    {
      while (SDL_PollEvent(&e) != 0)
      {
        switch(e.type)
        {
          case SDL_QUIT:
            bQuit = true;
            break;
          case SDL_WINDOWEVENT:
            if (e.window.event == SDL_WINDOWEVENT_RESIZED || e.window.event == SDL_WINDOWEVENT_EXPOSED)
            {
              int w, h;
              SDL_GetWindowSize(_window, &w, &h);
              _windowExtent.width = w;
              _windowExtent.height = h;
              
              _recreateFunction();
            }
            break;
          case SDL_KEYDOWN:
          case SDL_KEYUP:
            _keyboard.handle(e.key);
            break;
          case SDL_MOUSEMOTION:
            _mouse.handle(e.motion);
            break;
          case SDL_MOUSEBUTTONDOWN:
          case SDL_MOUSEBUTTONUP:
            _mouse.handle(e.button);
            break;
          case SDL_MOUSEWHEEL:
            _mouse.handle(e.wheel);
            break;
        }
      }
      
      // if (_isCursorHided && !(SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED))
      //   SDL_WarpMouseInWindow(_window, _windowExtent.width / 2, _windowExtent.height / 2);
      
#ifndef DEBUG
      if (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED)
        continue;
#endif
      _mainloopFunction();
      
      _mouse.reset();
      _keyboard.reset();
    }
  }
  
  void WindowHandler::cleanup()
  {
    SDL_DestroyWindow(_window);
  }
  
  void WindowHandler::create_surface(VkInstance instance, VkSurfaceKHR* surface)
  {
    SDL_Vulkan_CreateSurface(_window, instance, surface);
  }
  
  uint32_t& WindowHandler::height()
  {
    return _windowExtent.height;
  }
  
  uint32_t& WindowHandler::width()
  {
    return _windowExtent.width;
  }
  
  void WindowHandler::hide_cursor()
  {
    _isCursorHided = !_isCursorHided;
    
    SDL_ShowCursor(_isCursorHided ? SDL_DISABLE : SDL_ENABLE);
  }
  
}
