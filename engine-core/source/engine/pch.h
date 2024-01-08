#pragma once

#include <iostream>
#include <memory>
#include <algorithm>
#include <functional>
#include <string>
#include <vector>
#include <unordered_map>
#include <map>

#include <vulkan/vulkan.hpp>

#include <vk_mem_alloc/vk_mem_alloc.h>

#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>

#include "utils/math/math.h"
#include "utils/file_watcher.h"
#include "utils/time_handler.h"
#include "utils/macros.h"

// Handle mamory leaks in debug mode
#if defined(_DEBUG) & defined(_WIN32)
# define _CRTDBG_MAP_ALLOC
# include <crtdbg.h>
# define SetDbgMemHooks() \
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
# ifdef _CRTDBG_MAP_ALLOC 
//#   define new new(_NORMAL_BLOCK, __FILE__, __LINE__)
# endif // _CRTDBG_MAP_ALLOC
#endif // _DEBUG

#ifdef _WIN32
# define PLATFORM_WINDOWS
# include <Windows.h>
#endif // _WIN32
#ifdef __unix__
# define PLATFORM_UNIX
# define USE_SDL
# include <SDL2/SDL.h>
# include <SDL2/SDL_vulkan.h>
#endif // __unix__
