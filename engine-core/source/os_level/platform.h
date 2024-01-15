#pragma once

#ifdef _WIN32
  #define PLATFORM_WINDOWS
  #include <Windows.h>
#elif defined(__APPLE__)
  #include <TargetConditionals.h>
  #if TARGET_IPHONE_SIMULATOR == 1
    #error "IOS simulator is not supported!"
  #elif TARGET_OS_IPHONE == 1
    #error "IOS is not supported!"
  #elif TARGET_OS_MAC == 1
    #define PLATFORM_MACOS
    #define USE_SDL
  #else
    #error "Unknown Apple platform!"
  #endif
#elif defined(__linux__)
  #define PLATFORM_LINUX
  #define USE_SDL
#else
  #error "Unknown platform"
#endif

#if defined(USE_SDL)
  #include <SDL2/SDL.h>
#endif // USE_SDL
