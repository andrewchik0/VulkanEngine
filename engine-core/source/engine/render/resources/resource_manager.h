#pragma once

#include "../../types.h"

namespace VKEngine {
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
