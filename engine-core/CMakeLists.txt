cmake_minimum_required(VERSION 3.24)
set(ENGINE_CORE EngineCore)
set(CMAKE_CXX_STANDARD 20)

# Engine Core source files
file(GLOB_RECURSE ENGINE_FILES 
  "source/*.cpp" "source/*.h"
)

# Third-party source files
file(GLOB THIRD_PARTY_FILES 
  "third_party/VkBootstrap/*"
  "third_party/tiny_obj_loader/*"
  "third_party/vk_mem_alloc/*"
  "third_party/stb_image/*"
  "third_party/libschrift/*"
)

file(GLOB_RECURSE SHADER_FILES 
  "shaders/*.vert" "shaders/*.frag" "shaders/*.comp" "shaders/*.geom" "shaders/*.tese" "shaders/*.tesc"
)

# Add static library
set(ALL_SOURCES ${ENGINE_FILES} ${THIRD_PARTY_FILES} ${SHADER_FILES})
add_library(${ENGINE_CORE} STATIC ${ALL_SOURCES})

target_include_directories(${ENGINE_CORE} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/source/)

# Set filters to the target
foreach(SOURCE IN ITEMS ${ALL_SOURCES})
  get_filename_component(SOURCE_PATH "${SOURCE}" PATH)
  file(RELATIVE_PATH SOURCE_PATH_RELATIVE "${CMAKE_CURRENT_LIST_DIR}/" "${SOURCE_PATH}")
  string(REPLACE "/" "\\" GROUP_PATH "${SOURCE_PATH_RELATIVE}")
  source_group("${GROUP_PATH}" FILES "${SOURCE}")
endforeach()

target_compile_definitions(${ENGINE_CORE}
  PUBLIC GLSLC_EXECUTABLE=${Vulkan_GLSLC_EXECUTABLE}
  PUBLIC SHADERS_FOLDER_DEV="${CMAKE_CURRENT_LIST_DIR}/shaders"
)

# Configure precompiled headers
target_precompile_headers(${ENGINE_CORE} PRIVATE ${CMAKE_CURRENT_LIST_DIR}/source/engine/pch.h)
source_group("generated" FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${ENGINE_CORE}.dir/cmake_pch.cxx)
foreach(config_type ${CMAKE_CONFIGURATION_TYPES})
  source_group("generated" FILES ${CMAKE_CURRENT_BINARY_DIR}/CMakeFiles/${ENGINE_CORE}.dir/${config_type}/cmake_pch.hxx)
endforeach()

# Include third party dependencies
target_include_directories(${ENGINE_CORE} SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/third_party/)
target_include_directories(${ENGINE_CORE} SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/third_party/glm)

set_source_files_properties(${CMAKE_CURRENT_LIST_DIR}/third_party/libschrift/shcrift.c PROPERTIES SKIP_PRECOMPILE_HEADERS ON)

# Vulkan
find_package(Vulkan REQUIRED OPTIONAL_COMPONENTS shaderc_combined)
target_link_libraries(${ENGINE_CORE} PUBLIC Vulkan::Vulkan Vulkan::shaderc_combined)
target_include_directories(${ENGINE_CORE} PUBLIC ${Vulkan_INCLUDE_DIRS})

# SDL2
if(NOT WIN32)
  find_package(SDL2 REQUIRED)
  target_link_libraries(${ENGINE_CORE} PUBLIC SDL2::SDL2)
endif()

# Configure IDE environment
set_target_properties(${ENGINE_CORE} PROPERTIES
  XCODE_GENERATE_SCHEME TRUE
  XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
  MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
  FOLDER "Engine-Core"
)
set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded")
