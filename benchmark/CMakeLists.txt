
add_subdirectory("${PROJECT_SOURCE_DIR}/extern/benchmark" "extern/benchmark")

set(EXEC_BENCHMARK  lexic_unit_benchmark)

add_executable(${EXEC_BENCHMARK})

target_sources(${EXEC_BENCHMARK} PRIVATE find_lexic_unit.cpp)
target_link_libraries(${EXEC_BENCHMARK} template::sample benchmark::benchmark)