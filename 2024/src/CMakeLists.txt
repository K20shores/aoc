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
create_standard_test(NAME 2024_day11 SOURCES day11.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day12 SOURCES day12.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day13 SOURCES day13.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day14 SOURCES day14.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day15 SOURCES day15.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day16 SOURCES day16.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day17 SOURCES day17.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day18 SOURCES day18.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day19 SOURCES day19.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day20 SOURCES day20.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day21 SOURCES day21.cpp LIBRARIES aoc2024)
create_standard_test(NAME 2024_day22 SOURCES day22.cpp LIBRARIES aoc2024)

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