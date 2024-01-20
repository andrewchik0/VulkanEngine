#pragma once

#include <fstream>
#include <filesystem>
#include <functional>
#include <chrono>
#include <unordered_set>

#include "engine/utils/thread_pool.h"
#include "engine/utils/macros.h"
#include "engine/utils/math/math.h"

namespace VKEngine::Utils {
  
  class FileWatcher
  {
  public:
    
    FileWatcher(const std::string& filename, const std::function<void()>& onModifiedFunction)
      : _onModifiedFunction(onModifiedFunction), _path(filename)
    {
      update();
    }
    
    FileWatcher(const FileWatcher& watcher)
    {
      _onModifiedFunction = watcher._onModifiedFunction;
      _path = watcher._path;
      update();
    }
    
    FileWatcher(FileWatcher&& watcher)
    {
      _onModifiedFunction = std::move(watcher._onModifiedFunction);
      _path = std::move(watcher._path);
      update();
    }
    
    void tick() const
    {
      auto modified = std::filesystem::last_write_time(_path);
      if (modified != _lastTimeModified)
      {
        _lastTimeModified = modified;
        _onModifiedFunction();
      }
    }

    bool operator==(const FileWatcher& watcher) const
    {
      return _path == watcher._path;
    }
    
    struct HashFunction
    {
      std::size_t operator()(const FileWatcher& watcher) const
      {
        return std::hash<std::string>()(watcher._path);
      }
    };
    
  private:
    std::filesystem::path _path;
    std::function<void()> _onModifiedFunction;
    mutable std::filesystem::file_time_type _lastTimeModified;
    
    void update() const
    {
      _lastTimeModified = std::filesystem::last_write_time(_path);
    }
  };
  
  
  class FileWatchers
  {
  public:
    FileWatchers() {}
    
    FileWatchers(ThreadPool* threadPool)
      : _threadPool(threadPool)
    {
    }
    
    ~FileWatchers()
    {
      terminate();
    }
    
    void terminate()
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _bShouldTerminate = true;
    }
    
    void add_watcher(const std::string& filename, const std::function<void()>& onModifiedFunction)
    {
      std::unique_lock<std::mutex> lock(_mutex);
      _watchers.insert({filename, onModifiedFunction});
    }
    
    template<typename T>
    void set_delay(T duration)
    {
      using namespace std::chrono_literals;
      _delay = std::chrono::duration_cast<std::chrono::milliseconds>(duration);
      _delay = math::clamp(_delay, 0ms, 1000ms);
    }
    
    void tick()
    {
      for (auto &watcher : _watchers)
        watcher.tick();
    }
    
    void run_on_separate_thread()
    {
      if (_threadPool != nullptr)
      {
        _threadPool->send_task([&]{
          while (true)
          {
            std::this_thread::sleep_for(_delay);
            if (_watchers.size() == 0)
              continue;
            
            {
              std::unique_lock<std::mutex> lock(_mutex);
              if (_bShouldTerminate)
                return;
              tick();
            }
          }
        });
      }
      else
        assert_msg("Tried to run file watchers on separate thread, but no thread pool was specified!");
    }

  private:
    
    std::unordered_set<FileWatcher, FileWatcher::HashFunction> _watchers;
    
    // For running in separate thread
    ThreadPool* _threadPool = nullptr;
    std::chrono::milliseconds _delay = std::chrono::milliseconds(100);
    bool _bShouldTerminate = false;
    std::mutex _mutex;
  };
}
