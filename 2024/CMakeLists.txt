################################################################################
# Preamble
cmake_minimum_required(VERSION 3.21)

project(
  aoc
  VERSION 0.0.2024
  LANGUAGES CXX
)

# default to release build
if (NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE Release)
endif()

message(STATUS "CMake build configuration for ${PROJECT_NAME} (${CMAKE_BUILD_TYPE}) ${PROJECT_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
# dependencies

include(cmake/dependencies.cmake)

################################################################################
# targets

enable_testing()
add_subdirectory(src)

################################################################################