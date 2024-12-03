################################################################################
# Preamble
cmake_minimum_required(VERSION 3.21)

project(
  aoc
  VERSION 0.0.2024
  LANGUAGES CXX
)

message(STATUS "CMake build configuration for ${PROJECT_NAME} (${CMAKE_BUILD_TYPE}) ${PROJECT_VERSION}")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${PROJECT_SOURCE_DIR}/cmake")

# Set the minimum required C++ standard to C++20
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

################################################################################
# options

option(AOC_2023 "Build the AoC 2023 project" OFF)
option(AOC_2024 "Build the AoC 2023 project" ON)

################################################################################
# dependencies

include(cmake/dependencies.cmake)

################################################################################
# targets

enable_testing()
add_subdirectory(src)

################################################################################