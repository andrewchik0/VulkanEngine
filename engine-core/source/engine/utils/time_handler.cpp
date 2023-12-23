#include <iostream>
#include <chrono>
#include <iomanip>

#include "time_handler.h"

namespace VKEngine::Utils {
  
  uint64_t TimeHandler::get_time_in_ms()
  {
    return get_time<std::chrono::milliseconds>();
  }
  
  uint64_t TimeHandler::get_time_in_ns()
  {
    return get_time<std::chrono::nanoseconds>();
  }
  
  template<typename T>
  uint64_t TimeHandler::get_time()
  {
    using namespace std::chrono;
    return duration_cast<T>(high_resolution_clock::now().time_since_epoch()).count();
  }
  
  TimeHandler::TimeHandler()
  {
    _startTime_ns = get_time_in_ns();
    _fpsCounterTime = 0.0;
    _deltaFrameCount = 0;
    _fps = 0.0;
    _timeSinceStart_ns = 0;
    _timeSinceStart = 0.0;
    _allZonesTime = 0;
    
    _zones.resize(ZONES_COUNT);
  }
  
  void TimeHandler::tick()
  {
    _deltaTime_ns = get_time_in_ns() - _timeSinceStart_ns - _startTime_ns;
    _timeSinceStart_ns = get_time_in_ns() - _startTime_ns;
    _fpsCounterTime += (double)_deltaTime_ns / 1e9;
    _deltaFrameCount++;
    _deltaTime = (double)_deltaTime_ns / 1e9;
    _timeSinceStart = _timeSinceStart_ns / 1.0e9;
    
    if (_fpsCounterTime >= _fpsCounterInterval)
    {
      _fps = _deltaFrameCount / (double)_fpsCounterTime;
      _fpsCounterTime = 0.0;
      _deltaFrameCount = 0;
    }
    
    static int counter = 0;
    counter++;
    if (counter >= 100)
    {
      // std::cout << _fps << '\n';
      size_t i = 0;
      for (auto& zone : _zones)
      {
        if (i != 0)
        {
          std::cout <<
            zone.name << ": " <<
            zone.consumedTime / 1.e6f << "ms (" <<
            std::fixed << std::setprecision(1) << zone.consumedTime / (float)_deltaTime_ns * 100.0f << "%)\n";
        }
        i++;
      }
      std::cout << "whole frame: " << _deltaTime_ns / 1.e6f << "ms\n";
      std::cout << std::endl;
      counter = 0;
    }
    
    _allZonesTime = 0;
    _zones.clear();
    _zones.push_back(TimeZone{"start", 0});
  }
  
  void TimeHandler::mark_zone(const char* zoneName)
  {
    _zones.push_back({zoneName, get_time_in_ns() - _startTime_ns - _timeSinceStart_ns - _allZonesTime});
    _allZonesTime += last_element(_zones).consumedTime;
  }
}
