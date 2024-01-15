cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
add_executable(Example WIN32 source/main.cpp)

# Necessary includes for example project
target_include_directories(Example PRIVATE ${PROJECT_SOURCE_DIR}/engine-core/source)
target_include_directories(Example SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/engine-core/third_party/)
target_include_directories(Example SYSTEM PRIVATE ${PROJECT_SOURCE_DIR}/engine-core/third_party/glm)
find_package(Vulkan REQUIRED)
target_include_directories(Example SYSTEM PRIVATE ${Vulkan_INCLUDE_DIRS})

# Linking with core
target_link_libraries(Example PUBLIC EngineCore)

# Setup for IDE
set_target_properties(Example
  PROPERTIES
    XCODE_GENERATE_SCHEME TRUE
    XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    VS_DEBUGGER_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
    MSVC_RUNTIME_LIBRARY "MultiThreadedDLL"
    BUILD_RPATH "/Library/Frameworks"
)
