message(STATUS "Building test")

set(target stream-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

find_package (Threads)
add_executable(${target} ${sources})
target_link_libraries (${target} ${CMAKE_THREAD_LIBS_INIT})
add_test(stream "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(stream PROPERTIES LABELS "all;stream")