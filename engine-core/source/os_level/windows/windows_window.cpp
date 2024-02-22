#include "windows_window.h"

#ifdef PLATFORM_WINDOWS

#include <windowsx.h>
#include <vulkan/vulkan_win32.h>

#include "engine/engine.h"

namespace VKEngine {

  void WindowsWindow::init(ApplicationSpecs specs, std::function<void()>&& mainloopFunction, std::function<void()>&& recreateFunction)
  {
    _mainloopFunction = mainloopFunction;
    _recreateFunction = recreateFunction;

    LPCSTR CLASS_NAME{ "Vulkan Engine" };
    _hInstance = GetModuleHandle(NULL);

    WNDCLASS wc = { };

    wc.lpfnWndProc = WindowProc;
    wc.hInstance = _hInstance;
    wc.hCursor = LoadCursor(NULL, IDC_ARROW);
    wc.lpszClassName = CLASS_NAME;

    RegisterClass(&wc);

    // Create the window.

    _hWnd = CreateWindowEx(
      0,
      CLASS_NAME,
      specs.name.c_str(),
      WS_OVERLAPPEDWINDOW,

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

    ShowWindow(_hWnd, SW_NORMAL);
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
        {
          _mainloopFunction();
          _input.reset();
        }
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
    if (_bIsCursorHided)
      ShowCursor(true);
    else
      while (ShowCursor(false) >= 0);
  }

  LRESULT CALLBACK WindowsWindow::WindowProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    return dynamic_cast<WindowsWindow&>(Engine::get()->window()).msg_handle(hWnd, uMsg, wParam, lParam);
  }

  LRESULT WindowsWindow::msg_handle(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
  {
    static POINT s_oldMousePos{};

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
      SetWindowLong(hWnd, 0, (LONG)(UINT_PTR)((CREATESTRUCT*)lParam)->lpCreateParams);
    default:
      if (hWnd != nullptr)
        switch (uMsg)
        {
        case WM_CREATE:
          _hWnd = hWnd;
          return 0;
        case WM_SIZE:
          _oldWindowExtent.width = _windowExtent.width;
          _oldWindowExtent.height = _windowExtent.height;
          _windowExtent = { LOWORD(lParam), HIWORD(lParam) };
          _recreateFunction();
          return 0;
        case WM_MOUSEWHEEL:
          _input.handle_mousewheel(GET_WHEEL_DELTA_WPARAM(wParam), 0);
          return 0;
        case WM_LBUTTONDOWN:
        case WM_RBUTTONDOWN:
        case WM_MBUTTONDOWN:
          SetCapture(hWnd);
          handle_wParam_mouse(wParam);
          return 0;
        case WM_LBUTTONUP:
        case WM_RBUTTONUP:
        case WM_MBUTTONUP:
          ReleaseCapture();
          handle_wParam_mouse(wParam);
          return 0;
        case WM_MOUSEMOVE:
          _input.handle_mousemotion(GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam), GET_X_LPARAM(lParam) - s_oldMousePos.x, GET_Y_LPARAM(lParam) - s_oldMousePos.y);
          s_oldMousePos = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
          return 0;
        case WM_KEYDOWN:
          _input.handle_keys((Key)wParam, true);
          handle_extended_keycodes(wParam, lParam, true);
          return 0;
        case WM_KEYUP:
          _input.handle_keys((Key)wParam, false);
          handle_extended_keycodes(wParam, lParam, false);
          return 0;
        case WM_DESTROY:
          PostQuitMessage(0);
          return 0;
        }
    }
    return DefWindowProc(hWnd, uMsg, wParam, lParam);
  }

  void WindowsWindow::handle_wParam_mouse(WPARAM wParam)
  {
    _input.handle_keys(KeyCodes::MouseLeftButton, wParam & MK_LBUTTON);
    _input.handle_keys(KeyCodes::MouseRightButton, wParam & MK_LBUTTON);
    _input.handle_keys(KeyCodes::MouseMiddleButton, wParam & MK_LBUTTON);
  }

  void WindowsWindow::handle_extended_keycodes(WPARAM wParam, LPARAM lParam, bool isPressed)
  {
    if ((Key)wParam == VK_MENU || (Key)wParam == VK_SHIFT || (Key)wParam == VK_CONTROL)
    {
      UINT scancode = (lParam & 0x00ff0000) >> 16;
      _input.handle_keys((Key)MapVirtualKey(scancode, MAPVK_VSC_TO_VK_EX), isPressed);
    }
  }

  void WindowsWindow::go_fullscreen()
  {
    if (!_bIsFullscreen)
    {
      HMONITOR hMon;
      MONITORINFOEX moninfo{};
      RECT rc;

      _bIsFullscreen = true;

      // Save old window size and position 
      GetWindowRect(_hWnd, &_fullScreenSaveRect);

      // Get closest monitor
      hMon = MonitorFromWindow(_hWnd, MONITOR_DEFAULTTONEAREST);

      // Get monitor information
      moninfo.cbSize = sizeof(moninfo);
      GetMonitorInfo(hMon, (MONITORINFO*)&moninfo);

      rc = moninfo.rcMonitor;
      AdjustWindowRect(&rc, GetWindowLong(_hWnd, GWL_STYLE), FALSE);

      // Expand window to full screen
      SetWindowPos(_hWnd, HWND_TOP,
        rc.left, rc.top,
        rc.right - rc.left, rc.bottom - rc.top,
        SWP_NOOWNERZORDER);
    }
    else
    {
      _bIsFullscreen = false;

      // Restore window size and position
      SetWindowPos(_hWnd, HWND_NOTOPMOST,
        _fullScreenSaveRect.left, _fullScreenSaveRect.top,
        _fullScreenSaveRect.right - _fullScreenSaveRect.left, _fullScreenSaveRect.bottom - _fullScreenSaveRect.top,
        SWP_NOOWNERZORDER);
    }
  }
}

#endif // PLATFORM_WINDOWS
