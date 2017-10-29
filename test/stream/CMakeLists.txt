message(STATUS "Building test")

set(target stream-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(stream "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(stream PROPERTIES LABELS "all;stream")