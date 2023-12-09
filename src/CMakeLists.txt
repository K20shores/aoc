################################################################################
# Test utilities

include(test_util)

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

################################################################################
# Copy input data

add_custom_target(copy_input_data ALL ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/inputs ${CMAKE_BINARY_DIR}/inputs)