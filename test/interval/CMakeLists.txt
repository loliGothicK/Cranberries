message(STATUS "Building test")

set(target interval-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(interval "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(interval PROPERTIES LABELS "all;interval")