#include "sdl_window.h"

#ifdef USE_SDL

namespace VKEngine {

  void SDLWindow::init(std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction)
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

  void SDLWindow::run()
  {
    SDL_Event e;
    bool bQuit = false;

    while (!bQuit)
    {
      while (SDL_PollEvent(&e) != 0)
      {
        switch (e.type)
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
          _input.handle_keys(e.key.keysym.sym, e.key.type == SDL_KEYDOWN);
          break;
        case SDL_MOUSEMOTION:
          _input.handle_mousemotion(e.motion.x, e.motion.y, e.motion.xrel, e.motion.yrel);
          break;
        case SDL_MOUSEBUTTONDOWN:
        case SDL_MOUSEBUTTONUP:
          _input.handle_keys(e.button.button, e.button.state == SDL_PRESSED);
          break;
        case SDL_MOUSEWHEEL:
          _input.handle_mousewheel(e.wheel.x, e.wheel.y);
          break;
        }
      }

      if (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED)
        continue;

      _mainloopFunction();

      _input.reset();
    }
  }

  void SDLWindow::cleanup()
  {
    SDL_DestroyWindow(_window);
  }

  void SDLWindow::create_surface(VkInstance instance, VkSurfaceKHR* surface)
  {
    SDL_Vulkan_CreateSurface(_window, instance, surface);
  }

  void SDLWindow::hide_cursor()
  {
    _bIsCursorHided = !_bIsCursorHided;

    SDL_ShowCursor(_bIsCursorHided ? SDL_DISABLE : SDL_ENABLE);
  }
  
  void SDLWindow::go_fullscreen()
  {
  }

}

#endif // USE_SDL
