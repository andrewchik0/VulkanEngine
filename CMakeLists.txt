cmake_minimum_required(VERSION 3.12)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
cmake_policy(SET CMP0091 NEW)
set(CMAKE_SUPPRESS_REGENERATION true)
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D_DEBUG -DDEBUG")

project(Engine LANGUAGES C CXX)

add_subdirectory(engine-core)

add_subdirectory(samples/example)
