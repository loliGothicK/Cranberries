message(STATUS "Building test")

set(target unit_test_framework-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)
target_link_libraries(unit_test_framework-test pthread)
add_executable(${target} ${sources})
add_test(unit_test_framework-test "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(unit_test_framework-test PROPERTIES LABELS "all;unit_test_framework")