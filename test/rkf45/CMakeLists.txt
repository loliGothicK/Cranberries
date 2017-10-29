message(STATUS "Building test")

set(target rkf45-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(rkf45 "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(rkf45 PROPERTIES LABELS "all;rkf45")