################################################################################
# Preamble
cmake_minimum_required(VERSION 3.21)

project(
  aoc2023
  VERSION 0.0.2023
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
# dependencies

include(cmake/dependencies.cmake)

# on ubuntu with clang, an incorrect version of the c++ standard library was being linked
if (${CMAKE_HOST_SYSTEM_NAME} MATCHES "Linux" AND "${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang")
    # If the compiler is Clang, use libc++
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
endif()

################################################################################
# targets

enable_testing()
add_subdirectory(src)

################################################################################