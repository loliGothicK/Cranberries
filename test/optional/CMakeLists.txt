message(STATUS "Building test")

set(target optional-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)

find_package (Threads)
add_executable(${target} ${sources})
target_link_libraries (${target} ${CMAKE_THREAD_LIBS_INIT})
add_test(optional "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(optional PROPERTIES LABELS "all;optional")