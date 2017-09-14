message(STATUS "Building test")

set(target unit_test_framework-test)
message(STATUS "Building library ${target}")

set(sources
	test.cpp
)
add_executable(${target} ${sources})
set(THREADS_PREFER_PTHREAD_FLAG ON)
find_package(Threads REQUIRED)
target_link_libraries(unit_test_framework-test Threads::Threads)
add_test(unit_test_framework-test "${EXECUTABLE_OUTPUT_PATH}/${target}")
set_tests_properties(unit_test_framework-test PROPERTIES LABELS "all;unit_test_framework")