message(STATUS "Building test")

set(target dual_number-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(dual_number "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(dual_number PROPERTIES LABELS "all;dual_number")