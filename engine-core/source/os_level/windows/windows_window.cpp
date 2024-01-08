#include "windows_window.h"

#ifdef PLATFORM_WINDOWS

#include "../../engine/engine.h"

namespace VKEngine {

  void WindowsWindow::init(std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction)
  {
    _mainloopFunction = mainloopFunction;
    _recreateFunction = recreateFunction;

    LPCSTR CLASS_NAME{ "Vulkan Engine" };
    _hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = _hInstance;
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    _hWnd = CreateWindowEx(
      0,                              // Optional window styles.
      CLASS_NAME,                     // Window class
      "Learn to Program Windows",     // Window text
      WS_OVERLAPPEDWINDOW,            // Window style

      // Size and position
      CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

      NULL,       // Parent window    
      NULL,       // Menu
      _hInstance, // Instance handle
      NULL        // Additional application data
    );

    if (_hWnd == NULL)
    {
      return;
    }

#if _DEBUG
    ShowWindow(_hWnd, true);
#else
    ShowWindow(_hWnd, false);
#endif // _DEBUG
    UpdateWindow(_hWnd);
  }

  void WindowsWindow::run()
  {
    MSG msg;
    bool bQuit = false;

    while (!bQuit)
    {
      if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
      {
        if (msg.message == WM_QUIT)
        {
          bQuit = true;
          break;
        }
        else
        {
          TranslateMessage(&msg);
          DispatchMessage(&msg);
        }
      }
      else
      {
        if (!IsIconic(_hWnd))
          _mainloopFunction();
      }
    }
  }

  void WindowsWindow::cleanup()
  {
  }

  void WindowsWindow::create_surface(VkInstance instance, VkSurfaceKHR* surface)
  {
    VkWin32SurfaceCreateInfoKHR info;
    info.hinstance = _hInstance;
    info.hwnd = _hWnd;
    info.pNext = nullptr;
    info.sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR;

    VK_CHECK(vkCreateWin32SurfaceKHR(instance, &info, nullptr, surface));
  }

  void WindowsWindow::hide_cursor()
  {
  }

  LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    dynamic_cast<WindowsWindow&>(Engine::get()->window()).msg_handle(hWnd, uMsg, wParam, lParam);
  }

  LRESULT WindowsWindow::msg_handle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    switch (uMsg)
    {
    case WM_GETMINMAXINFO:
      ((MINMAXINFO*)lParam)->ptMaxTrackSize.y =
        GetSystemMetrics(SM_CYMAXTRACK) +
        GetSystemMetrics(SM_CYCAPTION) +
        GetSystemMetrics(SM_CYMENU) +
        GetSystemMetrics(SM_CYBORDER) * 2;
      return 0;
    case WM_CREATE:
      SetWindowLong(hWnd, 0, (UINT_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
    default:
      if (hWnd != nullptr)
        switch (uMsg)
        {
        case WM_CREATE:
          _hWnd = hWnd;
          return 0;
        case WM_SIZE:
          _windowExtent = { LOWORD(lParam), HIWORD(lParam) };
          _recreateFunction();
          return 0;
        case WM_MOUSEWHEEL:
          // Handle mouse wheel
          return 0;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
          SetCapture(hWnd);

          INT X = (INT)(SHORT)LOWORD(lParam);
          INT Y = (INT)(SHORT)HIWORD(lParam);
          UINT Keys = (UINT)(SHORT)LOWORD(wParam);

          // Handle mouse buttons down

        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
          ReleaseCapture();

          // Handle mouse buttons up
          return 0;
        case WM_DESTROY:
          PostQuitMessage(0);
          return 0;
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }
}

#endif // PLATFORM_WINDOWS
