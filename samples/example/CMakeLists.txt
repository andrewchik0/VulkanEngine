cmake_minimum_required(VERSION 3.12)

set(CMAKE_CXX_STANDARD 20)
add_executable(Example source/main.cpp)

# Include Engine-Core source
target_include_directories(Example PRIVATE ${PROJECT_SOURCE_DIR}/engine-core/source)
target_link_libraries(Example Engine-Core)

# Set default working directory for Xcode IDE
set_target_properties(Example
  PROPERTIES
    XCODE_GENERATE_SCHEME TRUE
    XCODE_SCHEME_WORKING_DIRECTORY ${CMAKE_CURRENT_LIST_DIR}
)
