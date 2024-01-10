#pragma once

#include <fstream>
#include <filesystem>
#include <functional>

namespace VKEngine::Utils {
  
  class FileWatcher
  {
  public:
    
    FileWatcher(const std::string& filename, const std::function<void()>& onModifiedFunction)
      : _onModifiedFunction(onModifiedFunction), _path(filename)
    {
      _lastTimeModified = std::filesystem::last_write_time(_path);
    }
    
    void tick()
    {
      auto modified = std::filesystem::last_write_time(_path);
      if (modified != _lastTimeModified)
      {
        std::cout << _path << " has been modified" << std::endl;
        _lastTimeModified = modified;
        _onModifiedFunction();
      }
    }

  private:
    std::filesystem::path _path;
    std::function<void()> _onModifiedFunction;
    std::filesystem::file_time_type _lastTimeModified;
  };
  
  
  class FileWatchers
  {
  public:
    
    void add_watcher(const std::string& filename, const std::function<void()>& onModifiedFunction)
    {
      _watchers.push_back({filename, onModifiedFunction});
    }
    
    void tick()
    {
      for (auto &watcher : _watchers)
      {
        watcher.tick();
      }
    }

  private:
    std::vector<FileWatcher> _watchers;
  };
}
