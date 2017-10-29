message(STATUS "Building test")

set(target common-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(common "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(common PROPERTIES LABELS "all;common")