#pragma once

#define zero_memory(ptr, size) (memset((ptr), 0, (size)))
#define last_element(vector) ((vector)[(vector).size() - 1])
