#pragma once

// Memory set with value of zero
#define zero_memory(ptr, size) (memset((ptr), 0, (size)))

// Macro for getting last element of std::vector
#define last_element(vector) ((vector)[(vector).size() - 1])

// Custom assertion
#include <cassert>
#include <iostream>
void __engine_assert(bool bExpr, const char* message, const char* file, uint32_t line);
#ifndef NDEBUG
  #define assert_if_not(condition, message) (__engine_assert(condition, message, __FILE__, __LINE__));
  #define assert_msg(message) assert_if_not(false, message);
#else
  #define assert_if_not(condition, message)
  #define assert_msg(message)
#endif
