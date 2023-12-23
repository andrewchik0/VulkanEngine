#pragma once

#include "engine/engine.h"

namespace VKEngine {
  int32_t Main(size_t argc, char* argv[])
  {
    Engine engine;

    engine.init();
    engine.run();
    engine.cleanup();

    return 0;
  }
}

int main(int argc, char* argv[])
{
  return VKEngine::Main(argc, argv);
}

/*
 private и public поменять местами
 логгер с макросами файла
 добавить m и убрать эту конвенцию у публичных переменных
 */