#pragma once

#include "engine/engine.h"

namespace VKEngine {
  
  ApplicationSpecs create_app(CommandLineArgs args);
  
  int32_t Main(size_t argc, char* argv[])
  {
    ApplicationSpecs specs = create_app({ argc, argv });
    Engine engine;
    
    engine.init(specs);
    engine.run();
    engine.cleanup();

    return 0;
  }
}

#ifdef PLATFORM_WINDOWS

INT WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, CHAR* CmdLine, INT ShowCmd)
{
  return VKEngine::Main(__argc, __argv);
}

#else

int main(int argc, char* argv[])
{
  return VKEngine::Main(argc, argv);
}

#endif // PLATFORM_WINDOWS
