#pragma once

#include "../window.h"

#ifdef USE_SDL

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
    SDL_Window* _window;
  };
}

#endif // USE_SDL
