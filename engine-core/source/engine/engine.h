#pragma once

#include "free_camera.h"
#include "../os_level/window.h"
#include "render/render.h"

namespace VKEngine {
  
  struct ApplicationSpecs
  {
    std::string name;
  };
  
  class Engine {
  public:
    
    void init(ApplicationSpecs specs);
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

  private:
    static Engine* s_instance;
    
    bool _bIsInitialized{ false };
    
    FreeCamera _camera;
    
    std::unique_ptr<Window> _window;
    Utils::TimeHandler _timeHandler;
    Utils::FileWatchers _fileWatchers;
    Render _render;
    
    void init_scene();
  };
  
}
