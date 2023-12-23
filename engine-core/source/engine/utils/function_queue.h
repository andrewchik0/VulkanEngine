#pragma once

#include <functional>
#include <deque>

namespace VKEngine::Utils {
  
  class FunctionQueue
  {
  private:
    std::deque<std::function<void()>> _functions;
    
  public:
    void push_function(std::function<void()>&& function)
    {
      _functions.push_back(function);
    }
    
    void flush()
    {
      for (auto it = _functions.rbegin(); it != _functions.rend(); it++)
      {
        (*it)();
      }
      _functions.clear();
    }
  };
  
}
