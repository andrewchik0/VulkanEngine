#pragma once

#include "engine/free_camera.h"
#include "os_level/window.h"
#include "engine/render/render.h"

namespace VKEngine {
  
  struct ApplicationSpecs
  {
    std::string name;
  };
  
  class Engine {
  public:
    
    Engine() : _fileWatchers(&this->_threadPool) {}
    
    void init(ApplicationSpecs specs);
    void cleanup();
    void recreate();
    void draw();
    void run();
    
    inline static Engine* get()
      { return s_instance; }
    
    inline Camera& camera()
      { return _camera; }
    
    inline Window& window() 
      { return *_window.get(); }
    
    inline Render& render() 
      { return _render; }
    
    inline GUI& gui()
      { return _render._gui; }
    
    inline Utils::TimeHandler& time()
      { return _timeHandler; }
    
    inline Utils::FileWatchers& file_watchers()
      { return _fileWatchers; }
    
    inline Utils::ThreadPool& thread_pool()
      { return _threadPool; }

  private:
    static Engine* s_instance;
    
    bool _bIsInitialized{ false };
    
    FreeCamera _camera;
    
    std::unique_ptr<Window> _window;
    Render _render;
    
    Utils::TimeHandler _timeHandler;
    Utils::FileWatchers _fileWatchers;
    Utils::ThreadPool _threadPool;
    
    void init_scene();
  };
  
}
