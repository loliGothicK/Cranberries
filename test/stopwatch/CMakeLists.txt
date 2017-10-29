message(STATUS "Building test")

set(target stopwatch-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(stopwatch "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(stopwatch PROPERTIES LABELS "all;stopwatch")