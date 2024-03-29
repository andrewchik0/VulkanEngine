#pragma once

#include "os_level/window.h"

#ifdef USE_SDL

struct SDL_Window;

namespace VKEngine {

  class SDLWindow : public Window
  {
  public:
    void init(ApplicationSpecs specs, std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction) override;
    void run() override;
    void cleanup() override;
    void create_surface(VkInstance instance, VkSurfaceKHR* surface) override;
    void hide_cursor() override;
    void go_fullscreen() override;

  private:
    SDL_Window* _window{ nullptr };
  };
}

#endif // USE_SDL
