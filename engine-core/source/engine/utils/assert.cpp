#include "engine/utils/macros.h"

void __engine_assert(bool bExpr, const char* message, const char* file, uint32_t line)
{
  if (!bExpr)
  {
    std::cerr
      << "Assertion failed:\t" << message << "\n"
      << file << ", line " << line << "\n";
    abort();
  }
}
