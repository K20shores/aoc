################################################################################
# Test utilities

include(test_util)

################################################################################
# Dependencies

create_standard_test(NAME day1 SOURCES day1.cpp)

################################################################################
# Copy input data

add_custom_target(copy_input_data ALL ${CMAKE_COMMAND} -E copy_directory
  ${CMAKE_CURRENT_SOURCE_DIR}/inputs ${CMAKE_BINARY_DIR}/inputs)