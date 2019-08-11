cmake_minimum_required(VERSION 2.8)
project(ConeTracer)

# GLFW
option(GLFW_BUILD_DOCS OFF)
option(GLFW_BUILD_EXAMPLES OFF)
option(GLFW_BUILD_TESTS OFF)
add_subdirectory(lib/glfw)

# ASSIMP
option(ASSIMP_BUILD_ASSIMP_TOOLS OFF)
option(ASSIMP_BUILD_SAMPLES OFF)
option(ASSIMP_BUILD_TESTS OFF)
add_subdirectory(lib/assimp)

# GLEW
option(glew-cmake_BUILD_SHARED "" OFF)
add_subdirectory(lib/glew)

if(MSVC)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4")
else()
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic -std=c++11")
endif()

include_directories(
  include/
  lib/assimp/include/
  lib/glfw/include/
  lib/glew/include/
  lib/glm/
  lib/stb/
)

file(GLOB TRACER_SOURCES src/*.cpp)
file(GLOB TRACER_HEADERS include/*.hpp)

add_definitions(-DGLEW_STATIC)
add_definitions(-DGLFW_INCLUDE_NONE -DPROJECT_SOURCE_DIR=\"${PROJECT_SOURCE_DIR}\")

add_executable(${PROJECT_NAME} ${TRACER_SOURCES} ${TRACER_HEADERS})

target_link_libraries(${PROJECT_NAME} assimp glfw ${GLFW_LIBRARIES} libglew_static)
set_target_properties(${PROJECT_NAME} PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${PROJECT_NAME})
