cmake_minimum_required(VERSION 3.12)
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set(CMAKE_SUPPRESS_REGENERATION true)

project(Engine LANGUAGES C CXX)

add_subdirectory(engine-core)

add_subdirectory(samples/example)
