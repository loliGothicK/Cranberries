message(STATUS "Building test")

set(target func_util-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(func_util "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(func_util PROPERTIES LABELS "all;func_util")