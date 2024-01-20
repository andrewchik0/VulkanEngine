#include "window.h"

#ifdef USE_SDL
#include "os_level/sdl/sdl_window.h"
#endif // USE_SDL

#ifdef PLATFORM_WINDOWS
#include "os_level/windows/windows_window.h"
#endif // PLATFORM_WINDOWS

namespace VKEngine
{
  std::unique_ptr<Window> Window::create_window()
  {
#if defined(PLATFORM_WINDOWS)
    return std::make_unique<WindowsWindow>();
#elif defined(USE_SDL)
    return std::make_unique<SDLWindow>();
#else
#error "No suitable Window API"
#endif
  }
  
  void Window::delete_window(std::unique_ptr<Window>& window)
  {
#if defined(PLATFORM_WINDOWS)
    WindowsWindow* windowsWindow = dynamic_cast<WindowsWindow*>(window.get());
    delete windowsWindow;
    window.release();
#elif defined(USE_SDL)
    SDLWindow* sdlWindow = dynamic_cast<SDLWindow*>(window.get());
    delete sdlWindow;
    window.release();
#else
#error "No suitable Window API"
#endif
  }
}
