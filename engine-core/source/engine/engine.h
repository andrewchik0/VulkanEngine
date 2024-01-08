#pragma once

#include "free_camera.h"
#include "../os_level/window.h"
#include "render/render.h"

namespace VKEngine {
  
  class Engine {
  private:
    static Engine* s_instance;
    
    bool _isInitialized{ false };
    
    FreeCamera _camera;
    
    std::unique_ptr<Window> _window;
    Utils::TimeHandler _timeHandler;
    Utils::FileWatchers _fileWatchers;
    Render _render{_window.get()->width(), _window.get()->height()};
    
    void init_scene();
  public:
    
    void init();
    void cleanup();
    void recreate();
    void draw();
    void run();
    
    inline static Engine* get() { return s_instance; }
    
    inline Camera& camera() { return _camera; }
    inline Window& window() { return *_window.get(); }
    inline Utils::TimeHandler& time() { return _timeHandler; }
    inline Utils::FileWatchers& file_watchers() { return _fileWatchers; }
    inline Render& render() { return _render; }
  };
  
}
