################################################################################
# Test utilities

include(test_util)

################################################################################
# Library

add_subdirectory(lib)

################################################################################
# Problems

create_standard_test(NAME 2023_day1 SOURCES day1.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day2 SOURCES day2.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day3 SOURCES day3.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day4 SOURCES day4.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day5 SOURCES day5.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day6 SOURCES day6.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day7 SOURCES day7.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day8 SOURCES day8.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day9 SOURCES day9.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day10 SOURCES day10.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day11 SOURCES day11.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day12 SOURCES day12.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day13 SOURCES day13.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day14 SOURCES day14.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day15 SOURCES day15.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day16 SOURCES day16.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day17 SOURCES day17.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day18 SOURCES day18.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day19 SOURCES day19.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day20 SOURCES day20.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day21 SOURCES day21.cpp LIBRARIES aoc2023 YEAR 2023)
create_standard_test(NAME 2023_day22 SOURCES day22.cpp LIBRARIES aoc2023 YEAR 2023)

################################################################################
# Copy input data

add_custom_target(copy_2023_input_data ALL ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/inputs ${CMAKE_BINARY_DIR}/2023/inputs)