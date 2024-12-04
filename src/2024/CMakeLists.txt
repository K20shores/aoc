################################################################################
# Test utilities

include(test_util)

################################################################################
# Library

add_subdirectory(lib)

################################################################################
# Problems

create_standard_test(NAME 2024_day1 SOURCES day1.cpp LIBRARIES aoc2024 YEAR 2024)
create_standard_test(NAME 2024_day2 SOURCES day2.cpp LIBRARIES aoc2024 YEAR 2024)
create_standard_test(NAME 2024_day3 SOURCES day3.cpp LIBRARIES aoc2024 YEAR 2024)
create_standard_test(NAME 2024_day4 SOURCES day4.cpp LIBRARIES aoc2024 YEAR 2024)

################################################################################
# Copy input data

add_custom_target(copy_2024_input_data ALL
  COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/inputs
    ${CMAKE_BINARY_DIR}/2024/$<CONFIG>/inputs
)