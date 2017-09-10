message(STATUS "Building test")

set(target ranges-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(ranges "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(ranges PROPERTIES LABELS "all;ranges")