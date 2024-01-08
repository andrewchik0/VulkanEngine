#include "window.h"

#ifdef USE_SDL
#include "../os_level/sdl/sdl_window.h"
#endif // USE_SDL

#ifdef PLATFORM_WINDOWS
#include "../os_level/windows/windows_window.h"
#endif // PLATFORM_WINDOWS

namespace VKEngine
{
  std::unique_ptr<Window> Window::create_window()
  {
#if   defined(PLATFORM_WINDOWS)
    return std::make_unique<WindowsWindow>();
#elif defined(USE_SDL)
    return std::make_unique<SDLWindow>();
#else
    static_assert(true, "No suitable window API");
#endif
  }
}
