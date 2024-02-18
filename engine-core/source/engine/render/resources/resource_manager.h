#pragma once

#include "engine/types.h"

namespace VKEngine {
  inline std::string random_string(size_t length)
  {
      auto randchar = []() -> char {
          const char charset[] =
            "0123456789"
            "ABCDEFGHIJKLMNOPQRSTUVWXYZ"
            "abcdefghijklmnopqrstuvwxyz";
          const size_t max_index = (sizeof(charset) - 1);
          return charset[rand() % max_index];
      };
      std::string str(length, 0);
      std::generate_n(str.begin(), length, randchar);
      return str;
  }
  
  struct IResource
  {
    virtual void destroy(Render* render) = 0;
  };
  
  template<typename Key, class Val>
  class ResourceManager
  {
  public:
    void init(Render* render)
    {
      _render = render;
    }
    
    void destroy()
    {
      for (auto& it : _resources)
      {
        it.second.destroy(_render);
      }
    }
    
    void destroy(const Key& key)
    {
      const auto& it = _resources.find(key);
      if (it != _resources.end())
        it->second.destroy(_render);
    }
    
    Val* get(const Key& key)
    {
      const auto& it = _resources.find(key);
      if (it != _resources.end())
        return &it->second;
      return nullptr;
    }
    
    inline Val* operator[](const Key& key) { return get(key); }

  protected:
    static_assert(std::is_base_of<IResource, Val>::value, "Val must be derived from IResource");
    
    Render* _render = nullptr;
    
    std::unordered_map<Key, Val> _resources;
  };
}

