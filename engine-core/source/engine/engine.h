#pragma once

#include "free_camera.h"
#include "../os_level/window_handler.h"
#include "render/render.h"

namespace VKEngine {
  
  class Engine {
  private:
    static Engine* s_instance;
    
    bool _isInitialized{ false };
    
    FreeCamera _camera;
    
    WindowHandler _windowHandler;
    Utils::TimeHandler _timeHandler;
    Utils::FileWatchers _fileWatchers;
    Render _render{_windowHandler.width(), _windowHandler.height()};
    
    void init_scene();
  public:
    
    void init();
    void cleanup();
    void recreate();
    void draw();
    void run();
    
    inline static Engine* get() { return s_instance; }
    
    inline Camera& camera() { return _camera; }
    inline WindowHandler& window() { return _windowHandler; }
    inline Utils::TimeHandler& time() { return _timeHandler; }
    inline Utils::FileWatchers& file_watchers() { return _fileWatchers; }
    inline Render& render() { return _render; }
  };
  
}
