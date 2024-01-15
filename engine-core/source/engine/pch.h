#pragma once

#include "../os_level/platform.h"

#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include <vulkan/vulkan.hpp>
#if defined(USE_SDL)
  #include <SDL2/SDL_vulkan.h>
#endif // USE_SDL
#ifdef PLATFORM_WINDOWS
  #include <vulkan/vulkan_win32.h>
#endif // PLATFORM_WINDOWS

#include <vk_mem_alloc/vk_mem_alloc.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "utils/math/math.h"
#include "utils/file_watcher.h"
#include "utils/time_handler.h"
#include "utils/macros.h"

// Handle mamory leaks in debug mode
#if defined(_DEBUG) & defined(PLATFORM_WINDOWS)
  #define _CRTDBG_MAP_ALLOC
  #include <crtdbg.h>
  #define SetDbgMemHooks() \
    _CrtSetDbgFlag(_CRTDBG_LEAK_CHECK_DF | _CRTDBG_CHECK_ALWAYS_DF | \
    _CRTDBG_ALLOC_MEM_DF | _CrtSetDbgFlag(_CRTDBG_REPORT_FLAG))

static class __Dummy
{
public:
  __Dummy()
  {
    SetDbgMemHooks();
  }
} __ooppss;
#endif // _DEBUG

#ifdef _DEBUG
  #ifdef _CRTDBG_MAP_ALLOC
//    #define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
  #endif // _CRTDBG_MAP_ALLOC
#endif // _DEBUG
