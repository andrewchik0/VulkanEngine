#include "sdl_window.h"

#ifdef PLATFORM_UNIX

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

      if (SDL_GetWindowFlags(_window) & SDL_WINDOW_MINIMIZED)
        continue;

      _mainloopFunction();

      _mouse.reset();
      _keyboard.reset();
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

}

#endif // PLATFORM_UNIX
