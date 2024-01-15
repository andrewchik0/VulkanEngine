#pragma once

#include "../window.h"

#ifdef PLATFORM_WINDOWS

namespace VKEngine {

  class WindowsWindow : public Window
  {
  public:
    void init(ApplicationSpecs specs, std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction) override;
    void run() override;
    void cleanup() override;
    void create_surface(VkInstance instance, VkSurfaceKHR* surface) override;
    void hide_cursor() override;
    void go_fullscreen() override;

  private:
    HWND _hWnd;
    HINSTANCE _hInstance;
    RECT _fullScreenSaveRect;

    static LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

    LRESULT msg_handle(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
    void handle_wParam_mouse(WPARAM wParam);
    void handle_extended_keycodes(WPARAM wParam, LPARAM lParam, bool isPressed);
  };
}

#endif // PLATFORM_WINDOWS
