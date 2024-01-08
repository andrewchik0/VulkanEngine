#pragma once

#include "../window.h"

#ifdef PLATFORM_WINDOWS

namespace VKEngine {

  class WindowsWindow : public Window
  {
  public:
    void init(std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction) override;
    void run() override;
    void cleanup() override;
    void create_surface(VkInstance instance, VkSurfaceKHR* surface) override;
    void hide_cursor() override;

  private:
    HWND _hWnd;
    HINSTANCE _hInstance;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT msg_handle(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
  };
}

#endif // PLATFORM_WINDOWS
