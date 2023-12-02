################################################################################
# Test utilities

include(test_util)

################################################################################
# Problems

create_standard_test(NAME day1 SOURCES day1.cpp)
create_standard_test(NAME day2 SOURCES day2.cpp)

################################################################################
# Copy input data

add_custom_target(copy_input_data ALL ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/inputs ${CMAKE_BINARY_DIR}/inputs)