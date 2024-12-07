add_library(aoc2024)

target_sources(aoc2024 
  PRIVATE 
    file.cpp
)

target_include_directories(aoc2024 PUBLIC ${PROJECT_SOURCE_DIR}/include)

target_link_libraries(aoc2024 PUBLIC OpenMP::OpenMP_CXX)
