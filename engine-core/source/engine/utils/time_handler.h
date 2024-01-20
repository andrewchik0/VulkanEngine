#pragma once

#include "engine/utils/macros.h"

namespace VKEngine::Utils {
  
  struct TimeZone
  {
    std::string name;
    uint64_t consumedTime;
  };
  
  class TimeHandler
  {
  public:
    double _deltaTime = 0, _fps = 0, _timeSinceStart = 0;
    
    TimeHandler();
    
    void tick();
    void mark_zone(const char* zoneName);

  private:
    // Time in nanoseconds
    uint64_t _timeSinceStart_ns, _startTime_ns, _deltaTime_ns;
    
    // Time in seconds
    double _fpsCounterTime;
    
    const double _fpsCounterInterval = 1.0;
    
    // Frame counter between measurements
    int _deltaFrameCount;
    
    const size_t ZONES_COUNT = 256;
    std::vector<TimeZone> _zones;
    uint64_t _allZonesTime;
    
    uint64_t get_time_in_ms();
    uint64_t get_time_in_ns();
    
    template<typename T>
    uint64_t get_time();
  };

}
