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
)

add_compile_definitions(GLSLC_EXECUTABLE=${Vulkan_GLSLC_EXECUTABLE})

# Add static library
add_library(${ENGINE_CORE} SHARED ${ENGINE_FILES} ${THIRD_PARTY_FILES})

target_include_directories(${ENGINE_CORE} PUBLIC ${CMAKE_CURRENT_LIST_DIR}/source/)

target_precompile_headers(${ENGINE_CORE} PUBLIC 
  ${CMAKE_CURRENT_LIST_DIR}/source/engine/pch.h
)

#
# Link and include external dependencies
#

target_include_directories(${ENGINE_CORE} SYSTEM PUBLIC ${CMAKE_CURRENT_LIST_DIR}/third_party/)

# Add glm git submodule
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/glm)
target_link_libraries(${ENGINE_CORE} PUBLIC glm::glm)

# Vulkan
find_package(Vulkan REQUIRED OPTIONAL_COMPONENTS shaderc_combined)
target_link_libraries(${ENGINE_CORE} PUBLIC Vulkan::Vulkan Vulkan::shaderc_combined)

# SDL2
find_package(SDL2 REQUIRED)
target_link_libraries(${ENGINE_CORE} PUBLIC SDL2::SDL2)

#
# Configure IDE environment
#

if (APPLE)
  find_library(SDL2_PATH SDL2)
  # string(REPLACE "/Headers" "" SDL2_FRAMEWORK ${SDL2_INCLUDE_DIR})
  # message(STATUS ${SDL2_PATH})
  
  # target_link_libraries(${ENGINE_CORE} PUBLIC "-framework ${SDL2_PATH}")
  
  # Set default working directory for Xcode IDE
  set_target_properties(${ENGINE_CORE} PROPERTIES
    # XCODE_EMBED_FRAMEWORKS ${SDL2_PATH}
    # XCODE_LINK_BUILD_PHASE_MODE KNOWN_LOCATION
    XCODE_GENERATE_SCHEME TRUE
    XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    # LINK_FLAGS "-Wl,-F/Library/Frameworks"
  )
endif()
