#pragma once

#include "engine/types.h"
#include "engine/render/resources/vertex.h"

namespace VKEngine {
  
  class IGUIElement
  {
  public:

    virtual void render(VkCommandBuffer cmd) = 0;
    
    void resize(int32_t deltaWidth, int32_t deltaHeight)
    {
      if ((uint32_t)_alignmentHorizontal > 0)
        _position.x += deltaWidth / (int32_t)_alignmentHorizontal;
      if ((uint32_t)_alignmentVertical > 0)
        _position.y += deltaHeight / (int32_t)_alignmentVertical;
    }
  protected:
    glm::vec2 _position = { 0, 0 };
    
    enum class AlignmentVertical
    {
      Top    = 0,
      Bottom = 1,
      Center = 2,
    } _alignmentVertical;
    
    enum class AlignmentHorizontal
    {
      Left   = 0,
      Right  = 1,
      Center = 2,
    } _alignmentHorizontal;
    
    void recalculate_position(Rectangle* rect, uint32_t width, uint32_t height, uint32_t screenWidth, uint32_t screenHeight)
    {
      if (rect->y1 != -1)
      {
        rect->y1 = rect->y1;
        rect->y2 = rect->y1 + height;
        _alignmentVertical = AlignmentVertical::Top;
      }
      else if (rect->y2 != -1)
      {
        rect->y2 = screenHeight - rect->y2;
        rect->y1 = rect->y2 - height;
        _alignmentVertical = AlignmentVertical::Bottom;
      }
      if (rect->x1 != -1)
      {
        rect->x1 = rect->x1;
        rect->x2 = rect->x1 + width;
        _alignmentHorizontal = AlignmentHorizontal::Left;
      }
      else if (rect->x2 != -1)
      {
        rect->x2 = screenWidth - rect->x2;
        rect->x1 = rect->x2 - width;
        _alignmentHorizontal = AlignmentHorizontal::Right;
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
