message(STATUS "Building test")

set(target algorithm-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(algorithm "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(algorithm PROPERTIES LABELS "all;algorithm")