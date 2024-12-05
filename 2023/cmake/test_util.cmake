################################################################################
# build and add a standard test

function(create_standard_test)
  set(prefix TEST)
  set(singleValues NAME WORKING_DIRECTORY YEAR)
  set(multiValues SOURCES LIBRARIES)

  include(CMakeParseArguments)
  cmake_parse_arguments(${prefix} " " "${singleValues}" "${multiValues}" ${ARGN})

  if(NOT DEFINED TEST_YEAR)
    message(FATAL_ERROR "TEST_YEAR must be specified")
  endif()

  add_executable(test_${TEST_NAME} ${TEST_SOURCES})

  set_target_properties(test_${TEST_NAME} PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/${TEST_YEAR}"
  )

  target_link_libraries(test_${TEST_NAME} PUBLIC benchmark ${TEST_LIBRARIES})

  if(NOT DEFINED TEST_WORKING_DIRECTORY)
    set(TEST_WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/${TEST_YEAR}")
  endif()

  add_aoc_test(${TEST_NAME} test_${TEST_NAME} "" ${TEST_WORKING_DIRECTORY})
endfunction(create_standard_test)

################################################################################
# Add a test

function(add_aoc_test test_name test_binary test_args working_dir)
    add_test(NAME ${test_name}
            COMMAND ${test_binary} ${test_args}
            WORKING_DIRECTORY ${working_dir})
endfunction(add_aoc_test)

################################################################################