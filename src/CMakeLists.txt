################################################################################
# Test utilities

include(test_util)

################################################################################
# Library

add_subdirectory(lib)

################################################################################
# Problems

create_standard_test(NAME day1 SOURCES day1.cpp)
create_standard_test(NAME day2 SOURCES day2.cpp)
create_standard_test(NAME day3 SOURCES day3.cpp)
create_standard_test(NAME day4 SOURCES day4.cpp)
create_standard_test(NAME day5 SOURCES day5.cpp)
create_standard_test(NAME day6 SOURCES day6.cpp)
create_standard_test(NAME day7 SOURCES day7.cpp)
create_standard_test(NAME day8 SOURCES day8.cpp)
create_standard_test(NAME day9 SOURCES day9.cpp)
create_standard_test(NAME day10 SOURCES day10.cpp)
create_standard_test(NAME day11 SOURCES day11.cpp)
create_standard_test(NAME day12 SOURCES day12.cpp)
create_standard_test(NAME day13 SOURCES day13.cpp)
create_standard_test(NAME day14 SOURCES day14.cpp)
create_standard_test(NAME day15 SOURCES day15.cpp)
create_standard_test(NAME day16 SOURCES day16.cpp)
create_standard_test(NAME day17 SOURCES day17.cpp)
create_standard_test(NAME day18 SOURCES day18.cpp)
create_standard_test(NAME day19 SOURCES day19.cpp)
create_standard_test(NAME day20 SOURCES day20.cpp)
create_standard_test(NAME day21 SOURCES day21.cpp)
create_standard_test(NAME day22 SOURCES day22.cpp)

################################################################################
# Copy input data

add_custom_target(copy_input_data ALL ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/inputs ${CMAKE_BINARY_DIR}/inputs)