#include "font.h"

#include "libschrift/schrift.h"

namespace VKEngine {
  const Point defaultSize = { 64, 64 };
  struct Font
  {
    SFT sft;
    SFT_LMetrics metrics;
  };
  
  struct Symbol
  {
    uint32_t character;
    SFT_Glyph glyph;
    SFT_GMetrics metrics;
    Point kerning{ 0, 0 };
  };
  
  std::unordered_map<std::string, Font> _fonts;
  Font* _currentFont = nullptr;
  
  void get_symbol_texture(Symbol* s, SFT_Image* image)
  {
    image->width = s->metrics.minWidth;
    image->height = s->metrics.minHeight;
    image->pixels = new int8_t[image->width * image->height];
    sft_render(&_currentFont->sft, s->glyph, *image);
  }
  
  void FontHandler::load_font(const std::string& filename, Point size/* = { 0, 0 }*/)
  {
    Font newFont;
    newFont.sft.xScale = size.x == 0 ? defaultSize.x : size.x;
    newFont.sft.yScale = size.y == 0 ? defaultSize.y : size.y;
    newFont.sft.flags = SFT_DOWNWARD_Y;
    newFont.sft.font = sft_loadfile(filename.c_str());
    sft_lmetrics(&newFont.sft, &newFont.metrics);
    
    _currentFont = &(_fonts[filename] = newFont);
  }
  
  void FontHandler::set_current_font(const std::string& filename, Point size/* = { 0, 0 }*/)
  {
    if (_fonts.find(filename) != _fonts.end())
    {
      _currentFont = &_fonts[filename];
      _currentFont->sft.xScale = size.x == 0 ? _currentFont->sft.xScale : size.x;
      _currentFont->sft.yScale = size.y == 0 ? _currentFont->sft.yScale : size.y;
      sft_lmetrics(&_currentFont->sft, &_currentFont->metrics);
    }
    else
      load_font(filename, size);
  }
  
  void FontHandler::free()
  {
    for (auto& font : _fonts)
      sft_freefont(font.second.sft.font);
  }
  
  RawTexture FontHandler::render_static_string(const std::string& text)
  {
    RawTexture tex;
    int32_t totalWidth = 0, numOfLines = 1;
    std::vector<Symbol> symbols;
    symbols.resize(text.size());
    float lineHeight = _currentFont->metrics.ascender - _currentFont->metrics.descender + _currentFont->metrics.lineGap;
    
    // Get metrics for all symbols
    int64_t lineWidth = 0;
    for (uint32_t i = 0; i < text.size(); ++i)
    {
      if (text[i] == '\n')
      {
        numOfLines++;
        totalWidth = fmax(totalWidth, lineWidth);
        lineWidth = 0;
      }
      symbols[i].character = text[i];
      sft_lookup(&_currentFont->sft, symbols[i].character, &symbols[i].glyph);
      sft_gmetrics(&_currentFont->sft, symbols[i].glyph, &symbols[i].metrics);
      
      if (i != text.size() - 1)
      {
        symbols[i].kerning = get_kerning(text[i], text[i + 1]);
        lineWidth += std::lround(symbols[i].kerning.x);
      }
      lineWidth += std::lround(symbols[i].metrics.advanceWidth);
    }
    totalWidth = fmax(totalWidth, lineWidth);
    
    // Allocate memory for texture
    tex.width = totalWidth;
    tex.height = lineHeight * numOfLines;
    tex.size = tex.width * tex.height * sizeof(int8_t);
    tex.data = new int8_t[tex.size];
    zero_memory(tex.data, tex.size);
    
    // Fill the texture
    Point offset{ 0, 0 };
    uint32_t lineNumber = 0;
    for (auto& symb : symbols)
    {
      if (symb.character == '\n')
      {
        offset.x = 0;
        lineNumber++;
        continue;
      }
      offset.y = fmax(0, _currentFont->metrics.ascender + symb.metrics.yOffset) + lineNumber * lineHeight;
      
      SFT_Image img;
      get_symbol_texture(&symb, &img);
      tex.add_texture(img.pixels, img.width, img.height, 1, (uint32_t)std::lround(offset.x), (uint32_t)std::lround(offset.y));
      delete[] (int8_t *)(img.pixels);
      
      offset.x += symb.metrics.advanceWidth + std::lround(symb.kerning.x);
    }
    return tex;
  }
  
  Point FontHandler::get_kerning(uint32_t charA, uint32_t charB)
  {
    SFT_Glyph glyphA, glyphB;
    SFT_Kerning kerning;
    
    sft_lookup(&_currentFont->sft, charA, &glyphA);
    sft_lookup(&_currentFont->sft, charB, &glyphB);
    
    sft_kerning(&_currentFont->sft, glyphA, glyphB, &kerning);
    
    return Point{ (float)kerning.xShift, (float)kerning.yShift };
  }
}
