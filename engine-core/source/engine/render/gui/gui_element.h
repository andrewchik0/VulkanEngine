#pragma once

#include "engine/types.h"
#include "engine/render/resources/vertex.h"

namespace VKEngine {
  
  class IGUIElement
  {
  public:
    
    virtual void render(VkCommandBuffer cmd) = 0;
  protected:
    
    void recalculate_position(Rectangle* rect, uint32_t width, uint32_t height, uint32_t screenWidth, uint32_t screenHeight)
    {
      if (rect->y1 != -1)
      {
        rect->y1 = rect->y1;
        rect->y2 = rect->y1 + height;
      }
      else if (rect->y2 != -1)
      {
        rect->y2 = screenHeight - rect->y2;
        rect->y1 = rect->y2 - height;
      }
      if (rect->x1 != -1)
      {
        rect->x1 = rect->x1;
        rect->x2 = rect->x1 + width;
      }
      else if (rect->x2 != -1)
      {
        rect->x2 = screenWidth - rect->x2;
        rect->x1 = rect->x2 - width;
      }
    }
    
    Vertices rectangle(Rectangle rect)
    {
      Vertices vertices;
      vertices.positions2f =
      {
        {rect.x1, rect.y1},
        {rect.x1, rect.y2},
        {rect.x2, rect.y1},
        {rect.x2, rect.y2},
        {rect.x1, rect.y2},
        {rect.x2, rect.y1},
      };
      vertices.texuvs =
      {
        {-1, -1},
        {-1,  0},
        { 0, -1},
        { 0,  0},
        {-1,  0},
        { 0, -1},
      };
      return vertices;
    }
  };
  
}
