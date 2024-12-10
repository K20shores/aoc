################################################################################
# Test utilities

include(test_util)

################################################################################
# Library

add_subdirectory(lib)

################################################################################
# Problems

create_standard_test(NAME 2024_day1 SOURCES day1.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day2 SOURCES day2.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day3 SOURCES day3.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day4 SOURCES day4.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day5 SOURCES day5.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day6 SOURCES day6.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day7 SOURCES day7.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day8 SOURCES day8.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day9 SOURCES day9.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day10 SOURCES day10.cpp LIBRARIES aoc2024)

################################################################################
# Copy input data


# only copy inputs if input directory exists
if(EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/inputs)
  add_custom_target(copy_2024_input_data ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_SOURCE_DIR}/inputs
      ${CMAKE_BINARY_DIR}/inputs
  )
  add_custom_target(copy_2024_input_data_config ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
      ${CMAKE_CURRENT_SOURCE_DIR}/inputs
      ${CMAKE_BINARY_DIR}/$<CONFIG>/inputs
  )
endif()