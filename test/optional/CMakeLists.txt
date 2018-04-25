message(STATUS "Building test")

set(target optional-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(optional "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(optional PROPERTIES LABELS "all;optional")