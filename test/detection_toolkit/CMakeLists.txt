message(STATUS "Building test")

set(target detection_toolkit-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

add_executable(${target} ${sources})
add_test(detection_toolkit "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(detection_toolkit PROPERTIES LABELS "all;detection_toolkit")